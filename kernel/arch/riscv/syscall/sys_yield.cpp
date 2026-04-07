#include <kernel/arch/riscv/syscall/syscall.h>

namespace syscall
{
    uintptr_t sys_yield() noexcept
    {
        return 0;
    }
}
