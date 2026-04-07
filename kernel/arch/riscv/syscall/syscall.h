#ifndef KERNEL_ARCH_RISCV_SYSCALL_SYSCALL_H
#define KERNEL_ARCH_RISCV_SYSCALL_SYSCALL_H

#include <stdint.h>
#include <kernel/arch/riscv/trap/trap.h>

namespace syscall
{
    uintptr_t dispatch(trap::TrapFrame &tf) noexcept;

    uintptr_t sys_write(uintptr_t fd, const void *buf, uintptr_t len) noexcept;
    uintptr_t sys_exit(uintptr_t code) noexcept;
    uintptr_t sys_yield() noexcept;
}

#endif
