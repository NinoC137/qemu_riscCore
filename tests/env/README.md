# env 最小验证说明

## 结论

当前环境下，**推荐使用显式路径运行 `pk`**：

    spike /Volumes/1TBWDBlue/文稿/Claw-Workspace/qemu-riscv-embedded/tools/riscv64-pk/bin/pk a.out

不要默认依赖 `spike pk a.out`。

## 原因

`which pk` 能找到，不等于 `spike` 会按 shell 的 `PATH` 去找 `pk`。

`spike` 对 `pk` 的查找有自己的逻辑，可能不会使用你当前 shell 解析出的命令路径。

所以：

- `spike /绝对路径/pk a.out`：最稳
- `spike pk a.out`：可能失败

## 当前建议

统一使用以下路线：

- 工具链：`riscv64-unknown-elf-*`
- `pk`：工程内 `tools/riscv64-pk/bin/pk`
- 测试程序：编译为 **RV64 ELF**
- 运行：显式传入 `pk` 绝对路径

## 编译命令

    riscv64-unknown-elf-g++ -march=rv64imafdc -mabi=lp64d -O3 -static env_test.cpp -o a.out

## 运行命令

    spike /Volumes/1TBWDBlue/文稿/Claw-Workspace/qemu-riscv-embedded/tools/riscv64-pk/bin/pk a.out

## 验证程序位宽

    file a.out

应看到：

- `ELF 64-bit`

## 注意

不要再混用以下旧命令：

    spike --isa=RV32G pk a.out

原因：

- 当前 `pk` 是 `riscv64`
- 当前测试程序也应为 `riscv64`
- 项目当前路线已统一为 **RV64**
