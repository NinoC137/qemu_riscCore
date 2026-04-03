# qemu-riscv-embedded

一个面向 RV64 的 QEMU 裸机/OS 实验工程。

目标：从最小可启动内核出发，逐步扩展到可维护的 RISC-V 嵌入式 OS 平台。

## 当前状态

已完成：
- 最小启动链路打通：`entry.S -> kmain`
- 自定义 linker script 可用
- QEMU 可启动并输出串口日志
- 基础构建流程可复现：`make` / `scripts/build.sh`

当前代码仍属于 **bring-up 阶段**，重点是先把最小内核闭环做稳，再逐步扩展。

## 目录

- `boot/`：启动入口、早期汇编、后续固件/引导适配
- `kernel/`：内核最小主体
- `drivers/`：串口、定时器、中断控制器等驱动
- `platform/`：平台内存映射、设备规格、后续 DTS/板级定义
- `user/`：后续用户态程序
- `secure/`：后续安全域/RTOS 相关内容
- `tests/`：测试与验证
- `scripts/`：构建与辅助脚本
- `configs/`：配置文件
- `docs/`：专题设计文档

## 构建

在项目根目录执行：

    make clean && make

或：

    ./scripts/build.sh

输出：
- `kernel.elf`
- `kernel.map`

## 当前推荐开发顺序

1. 固化最小 bring-up 基线
2. 抽离平台常量与 MMIO 定义
3. 完成 UART 驱动最小封装
4. 接入 timer trap / 中断入口
5. 建立最小任务上下文切换框架
6. 再进入页表、syscall、进程模型

## 近期里程碑

### M1：最小内核基线
- 启动
- 清 BSS
- 串口输出
- 死循环保持
- 构建稳定

### M2：异常与中断
- trap 向量
- timer interrupt
- 基本异常打印

### M3：最小内核基础设施
- 页表初始化
- 物理内存管理雏形
- 简单任务/线程结构

### M4：用户态闭环
- trap 到 syscall
- `write`
- `init` 用户程序

## 设计原则

- 先单核跑通，再扩多核
- 先最小闭环，再加外设
- 先把可观测性做好，再做复杂机制
- 文档服务于实现，不写空泛大段说明
