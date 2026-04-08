# qemu-riscv-embedded

A QEMU-based RV64 bare-metal / embedded OS kernel playground. The project is used to incrementally build a small RISC-V kernel stack on top of the `virt` machine in QEMU, with an emphasis on bring-up, observability, trap/syscall infrastructure, and later evolution toward memory management, scheduling, and user mode support.

## What this project does

This repository targets a staged RISC-V kernel development workflow:

- bring up a bootable RV64 kernel on QEMU
- provide basic UART-based observability
- build a minimal trap / exception handling path
- establish a small syscall path for kernel-user boundary experiments
- leave room for future paging, task scheduling, drivers, and multi-domain support

It is currently a kernel experimentation project, not a full operating system distribution.

## Requirements

Recommended host environment:

- macOS or Linux
- CMake 3.20+
- Ninja
- RISC-V GNU embedded toolchain (`riscv64-unknown-elf-*`)
- QEMU with `qemu-system-riscv64`

On the current macOS setup, the toolchain path is expected to be:

- `/opt/homebrew/opt/riscv-gnu-toolchain/bin`

The repository already includes a CMake toolchain file:

- `cmake/riscv64-unknown-elf.cmake`

## Build

From the project root:

```sh
cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_TOOLCHAIN_FILE=cmake/riscv64-unknown-elf.cmake

cmake --build build
```

Expected output artifacts include:

- `build/kernel.elf`
- `build/kernel.map`
- `build/kernel.disasm`

## Run

Run the generated kernel with QEMU:

```sh
qemu-system-riscv64 \
  -machine virt \
  -bios none \
  -kernel build/kernel.elf
```

If you want serial output directly in the terminal, use a no-graphics launch form such as:

```sh
qemu-system-riscv64 \
  -machine virt \
  -bios none \
  -nographic \
  -serial stdio \
  -monitor none \
  -kernel build/kernel.elf
```

## CLion notes

For CLion, open the repository root as a CMake project and configure CMake with:

```text
-DCMAKE_TOOLCHAIN_FILE=/absolute/path/to/qemu-riscv-embedded/cmake/riscv64-unknown-elf.cmake
```

Do not use the RISC-V cross compiler directly as CLion's host toolchain compiler. Let CLion use its normal host environment and pass the cross-compilation setup through the CMake toolchain file.

## Current status

The project currently has the following working baseline:

- boot path from entry code into kernel main path
- custom linker script integration
- QEMU boot and UART output
- early trap / exception path setup
- minimal syscall path experiments
- basic `sys_write` / `sys_exit` style validation
- stable enough build flow for continued kernel iteration

## Roadmap

Planned next steps include:

- strengthen trap dispatch and context save/restore
- add timer interrupt support
- formalize syscall dispatch structure
- introduce early page-table / memory-management foundations
- build minimal task/thread scheduling support
- gradually extend toward user-mode execution and broader driver support

## Project structure

- `boot/` — early startup and assembly entry
- `kernel/` — core kernel logic
- `platform/` — platform-specific definitions and low-level support
- `drivers/` — device drivers and future hardware abstractions
- `user/` — user-mode or test payload experiments
- `secure/` — reserved area for future secure-domain work
- `tests/` — host-side or framework-side tests
- `scripts/` — helper scripts
- `docs/` — design and development notes
- `tools/` — auxiliary tools

## License

Add a project license here if and when you decide to publish one.
