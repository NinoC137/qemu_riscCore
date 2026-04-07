# 从 `ebreak` 之后到最小 OS 闭环的分阶段开发清单

## 0. 当前阶段判断

你现在已经具备：
- UART 基础输出
- `mtvec` 安装
- trap entry 保存/恢复现场
- `ebreak` 能进入 trap handler
- trap 后能通过修改 `mepc` 恢复执行

这说明你已经跨过了“能不能进异常”这个门槛，接下来最优路线不是扩外设，而是把它演进成：

**trap → syscall → 用户程序返回 → timer → 调度**

目标是尽快形成一个最小 OS 闭环：

1. 用户代码触发 `ecall`
2. trap handler 识别 syscall
3. 内核分发 syscall
4. 返回值写回寄存器
5. `mret` 返回继续执行
6. 最终 `exit`

---

## 1. 总体阶段划分

建议按下面顺序推进：

### 阶段 A：trap 子系统标准化
目标：把现在“能处理 `ebreak`”提升到“统一异常/中断入口框架”。

### 阶段 B：syscall 最小闭环
目标：让 `ecall` 成为正式的内核入口，至少跑通 `write/exit`。

### 阶段 C：最小任务模型
目标：建立 task/process 基础抽象，哪怕先只支持单任务。

### 阶段 D：timer interrupt + 调度
目标：建立时钟 tick 和最简 round-robin 调度器。

### 阶段 E：用户态与地址空间雏形
目标：为未来 `fork/exec` 和分页铺路，但先不追求完整 VM。

---

# 阶段 A：trap 子系统标准化

## A.1 模块目标

把 trap 从“打印 + 特判 `ebreak`”提升为统一框架：
- 统一 trap cause 解析
- 统一异常/中断分类
- 支持独立的异常处理策略
- 为 syscall/timer 预留稳定接口

## A.2 建议模块拆分

建议新增或重构为：

- `kernel/arch/riscv/trap/trap_frame.h`
  - `TrapFrame` 定义
  - 寄存器布局常量

- `kernel/arch/riscv/trap/trap_cause.h`
  - `TrapKind`
  - `ExceptionCode`
  - `InterruptCode`
  - cause decode 工具

- `kernel/arch/riscv/trap/trap_dispatch.cpp`
  - `dispatch_exception(tf)`
  - `dispatch_interrupt(tf)`

- `kernel/arch/riscv/trap/trap_policy.cpp`
  - `breakpoint` 策略
  - `illegal instruction` 策略
  - `page fault` 策略
  - `ecall` 策略

- `kernel/arch/riscv/trap/trap_debug.cpp`
  - trap dump
  - 寄存器打印

你现在也可以先不拆太细，但逻辑上要分清这几层。

## A.3 关键数据结构

### 1) TrapFrame
已有，建议确认：
- 顺序和 `trap_entry.S` 完全一致
- 明确注释“这个布局是 ABI 契约，改一边必须改另一边”

建议补充辅助访问接口：
- `syscall_num()` -> `a7`
- `arg0..arg5()` -> `a0..a5`
- `set_retval(x)` -> 写回 `a0`

### 2) TrapInfo
建议加一个纯解码结构，避免 handler 里反复解析：

- `bool is_interrupt`
- `uintptr_t raw_mcause`
- `uintptr_t code`
- `uintptr_t epc`
- `uintptr_t tval`

作用：
- 让 trap 入口和业务处理分离
- 单元测试更容易写

### 3) TrapAction
建议让每个处理器返回明确动作，而不是到处直接改 `tf`：

可取值例如：
- `ResumeNextInstruction`
- `ResumeAtCurrentEpc`
- `EnterPanic`
- `Yield`
- `KillCurrentTask`

早期哪怕只实现前两个，也值得先抽象出来。

## A.4 本阶段必须完成的功能

### A-1. 统一 cause 解码
输出：
- 识别 interrupt / exception
- 识别 `Breakpoint`
- 识别 `IllegalInstruction`
- 识别 `EcallFromMMode`
- 识别 `MachineTimer`

### A-2. `ebreak` 标准行为
建议定义：
- 当前阶段 `ebreak` 默认行为：打印日志 + `mepc += 4` + 返回

说明：
- 你现在 `.option norvc`，固定 32 位指令没问题
- 未来若支持压缩指令，再考虑 `2/4` 字节长度问题

### A-3. `ecall` 不再只是打印
行为应改为：
- 进入 syscall 分发函数
- 分发后推进 `mepc += 4`
- 把返回值写入 `a0`

### A-4. 非法指令/页错误有明确策略
当前建议：
- 打印错误
- dump `mepc/mcause/mtval`
- 进入 panic loop

## A.5 本阶段测试点

### 正向测试
- `ebreak` 触发后能恢复执行
- `ecall` 触发后能进入 syscall dispatch
- `mcause` 解码正确
- `interrupt` 与 `exception` 区分正确

### 反向测试
- 非法 `mcause` code 时仍能走默认分支
- `illegal instruction` 时进入 panic 路径

---

# 阶段 B：syscall 最小闭环

## B.1 模块目标

建立最小 syscall 子系统，建议只做三个：
- `write`
- `exit`
- `yield`（可选，但很适合为调度铺路）

## B.2 建议模块拆分

- `kernel/syscall/syscall.h`
- `kernel/syscall/syscall.cpp`
- `kernel/syscall/syscall_numbers.h`
- `kernel/syscall/sys_write.cpp`
- `kernel/syscall/sys_exit.cpp`
- `kernel/syscall/sys_yield.cpp`（可选）

## B.3 建议数据结构

### 1) SyscallContext
可直接复用 `TrapFrame`，也可以包一层：
- syscall 编号：`a7`
- 参数：`a0~a5`
- 返回值：`a0`

### 2) SyscallResult
建议统一返回：
- `value`
- `error`

即使你早期先把 error 直接编码进返回值，也建议逻辑上保留这个概念。

### 3) Syscall number 枚举
建议先自定义，不急着完全对齐 Linux ABI。
例如：
- `1 -> write`
- `2 -> exit`
- `3 -> yield`

后面再映射到你自己的 ABI 或兼容某套 ABI。

## B.4 最小功能定义

### `sys_write(fd, buf, len)`
当前最小实现建议：
- 只支持 `fd == 1` 或 `fd == 2`
- 输出到 UART
- 返回实际写入字节数
- 非法 `fd` 返回错误

### `sys_exit(code)`
当前最小实现建议：
- 记录当前任务状态为 exited
- 如果只有单任务，直接打印并停机/空转
- 如果已有调度器，则切到下一个任务

### `sys_yield()`
当前最小实现建议：
- 返回一个调度请求标记
- trap 返回前进入调度

## B.5 用户态闭环 Demo

建议你尽快做一个最小 demo：

用户程序逻辑：
1. 调用 `write("hello from user")`
2. 调用 `write("still alive")`
3. 调用 `exit(0)`

内核预期行为：
- 正确接收 `ecall`
- 正确识别 syscall 编号和参数
- UART 输出字符串
- 返回用户态继续执行
- 最终任务退出

## B.6 本阶段测试点

### 单元测试
- syscall 编号解析正确
- 参数提取正确
- `write` 返回字节数正确
- 未知 syscall 返回错误码
- `ecall` 后 `mepc` 正确前移 4 字节

### 集成测试
- 用户态连续两次 syscall 都能返回
- `exit` 后任务状态改变

---

# 阶段 C：最小任务模型

## C.1 模块目标

在调度之前，先建立任务抽象。
即使系统里暂时只有一个任务，也建议先把结构立起来。

## C.2 建议模块拆分

- `kernel/task/task.h`
- `kernel/task/task.cpp`
- `kernel/task/task_manager.h`
- `kernel/task/task_manager.cpp`

## C.3 关键数据结构

### 1) TaskState
建议：
- `Ready`
- `Running`
- `Blocked`
- `Exited`

### 2) TaskControlBlock (TCB)
建议最小字段：
- `task_id`
- `state`
- `TrapFrame* context` 或保存的寄存器上下文
- `kernel_stack_top`
- `user_stack_top`（如果已经准备用户态）
- `exit_code`

### 3) TaskManager
建议最小字段：
- 当前任务指针
- 简单任务数组/静态表
- 下一个 task id

## C.4 当前阶段建议实现

先不要一开始就搞复杂链表和动态内存。
建议：
- 固定大小任务数组
- 静态内存
- 单核前提下先不用锁

## C.5 本阶段测试点

- 创建任务后状态为 `Ready`
- 切到运行后状态变 `Running`
- `exit` 后状态变 `Exited`
- 任务管理器能找到“下一个 Ready 任务”

---

# 阶段 D：timer interrupt + 调度

## D.1 模块目标

让系统从“异常驱动”变成“时间驱动 + 调度驱动”。

## D.2 建议模块拆分

- `kernel/timer/timer.h`
- `kernel/timer/timer.cpp`
- `kernel/sched/scheduler.h`
- `kernel/sched/scheduler.cpp`

## D.3 关键数据结构

### 1) TimerEvent / TickState
最小化即可：
- `uint64_t ticks`
- `uint64_t next_deadline`

### 2) Scheduler
最小字段：
- 当前任务索引
- 任务表引用
- 时间片长度

## D.4 功能目标

### D-1. 机器定时器中断
- 配置下一次 timer compare
- 中断到来时 `ticks++`
- 重新设置下一次中断

### D-2. 最小 round-robin
- 时间片到期时选下一个 `Ready`
- 保存当前上下文
- 恢复下一个任务上下文

### D-3. `yield` 与 timer 共用调度路径
建议统一成：
- trap 中断/系统调用只设置“需要调度”标志
- 最终走同一条 `schedule()` 路径

## D.5 本阶段测试点

- timer 中断 code 识别正确
- tick 计数递增
- 两个任务之间能轮转
- `yield()` 可主动触发切换
- 只有一个任务时不会错误切换

---

# 阶段 E：用户态与地址空间雏形

## E.1 模块目标

给未来 `fork/exec/分页` 铺路，但当前只做“最小可运行”。

## E.2 建议模块拆分

- `kernel/mm/address_space.h`
- `kernel/mm/address_space.cpp`
- `kernel/mm/page_table.h`
- `kernel/mm/page_table.cpp`
- `kernel/user/user_loader.h`
- `kernel/user/user_loader.cpp`

## E.3 关键数据结构

### 1) AddressSpace
建议字段：
- 根页表地址
- 代码段范围
- 数据段范围
- 用户栈范围

### 2) UserImage
建议字段：
- 入口地址
- 镜像起始地址
- 镜像大小
- BSS 范围

## E.4 当前阶段建议

如果你还没把 S/U 模式完全切干净，可以先做“概念先行”：
- 有独立 user stack
- 有独立 task context
- `ecall` 接口按未来用户态 ABI 设计

先不急着一次做完：
- 完整页错误恢复
- copy-on-write
- 完整 ELF loader

## E.5 本阶段测试点

- 用户入口地址设置正确
- 用户栈初始化正确
- 从用户态进入 `ecall` 再返回
- 地址空间切换后仍能正确运行

---

# 推荐的数据结构总表

## TrapFrame
用途：trap 现场保存
关键字段：
- 通用寄存器
- `mepc`
- `mcause`
- `mtval`
- `mstatus`
- `mhartid`

## TrapInfo
用途：对 `mcause` 等做解析后的只读视图
关键字段：
- `is_interrupt`
- `code`
- `epc`
- `tval`

## SyscallRequest
用途：表达 syscall 编号和参数
关键字段：
- `number`
- `args[6]`

## SyscallResult
用途：表达返回值/错误
关键字段：
- `value`
- `error`

## TaskControlBlock
用途：任务管理
关键字段：
- `task_id`
- `state`
- `context`
- `kernel_stack_top`
- `user_stack_top`
- `exit_code`

## SchedulerState
用途：调度器运行态
关键字段：
- `current_task`
- `tick_count`
- `time_slice`

---

# 推荐开发顺序（非常具体）

## 第 1 步：重构 trap 代码
完成标准：
- `trap_handle()` 不再直接塞满所有逻辑
- cause decode / exception / interrupt 路径分清
- `ebreak` 处理稳定

## 第 2 步：加入 syscall dispatch
完成标准：
- `ecall` 能走进 `syscall_dispatch(tf)`
- `mepc += 4`
- `a0` 可写回返回值

## 第 3 步：实现 `sys_write`
完成标准：
- 能把用户缓冲区字符串输出到 UART
- 返回字节数

## 第 4 步：实现 `sys_exit`
完成标准：
- 单任务下可正确退出并停止
- 为未来多任务保留 `Exited` 状态

## 第 5 步：建立 task 抽象
完成标准：
- 至少有 TCB、TaskState、TaskManager
- 即使只有 1 个任务，也通过任务对象运行

## 第 6 步：加入 timer interrupt
完成标准：
- tick 增长
- 能周期性进中断

## 第 7 步：加入 round-robin 调度
完成标准：
- 两个任务轮转
- `yield` 和 timer 都能触发切换

---

# 你当前最值得立即做的三个文件方向

如果只考虑你现在工程的下一小时开发内容，建议优先做：

1. `kernel/syscall/syscall.h/.cpp`
   - 先把 `ecall` 正式接上

2. `kernel/arch/riscv/trap/trap.cpp`
   - 把 `EcallFromMMode` 改成真正分发，而不是只打印

3. `tests/` 下补宿主机单元测试
   - 先验证 trap cause decode
   - 再验证 `ebreak/ecall` 的 `mepc` 推进规则
   - 再验证 syscall dispatch

---

# 建议的“完成标志”

当下面这个 demo 成功时，你就算完成了“最小 OS 闭环”：

- 用户程序启动
- 调用 `write("hello")`
- 返回
- 再调用 `write("world")`
- 返回
- 调用 `exit(0)`
- 内核记录任务退出并进入 idle

这比“支持更多异常种类”更有阶段价值。

---

# 当前不建议优先投入的方向

在最小闭环跑通前，不建议把主要精力放到：
- 复杂外设驱动
- 完整虚拟内存系统
- `fork/exec` 全实现
- 文件系统
- 多核同步
- secure/non-secure 域切换细节

这些都重要，但它们应该建立在 trap/syscall/task/timer 这条主轴稳定之后。

---

# 一句话结论

你接下来最优开发主线是：

**把 `ebreak` 所在的 trap 机制，扩展成“统一 trap + syscall dispatch + 最小任务抽象 + timer 调度”的骨架，并尽快跑通 `write/exit` 用户态闭环。**
