#ifndef KERNEL_ARCH_RISCV_SYSCALL_SYSCALL_NUMBERS_H
#define KERNEL_ARCH_RISCV_SYSCALL_SYSCALL_NUMBERS_H

#include <stdint.h>

namespace syscall
{
    enum class Number : uintptr_t
    {
        Write = 1,
        Exit = 2,
        Yield = 3,
    };
}

#endif
