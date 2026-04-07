#include <kernel/arch/riscv/syscall/syscall.h>

#include <kernel/arch/riscv/syscall/syscall_numbers.h>

namespace syscall
{
    namespace
    {
        constexpr uintptr_t kErrUnknownSyscall = static_cast<uintptr_t>(-38);
    }

    uintptr_t dispatch(trap::TrapFrame &tf) noexcept
    {
        switch (static_cast<Number>(tf.a7))
        {
        case Number::Write:
            return sys_write(tf.a0, reinterpret_cast<const void *>(tf.a1), tf.a2);

        case Number::Exit:
            return sys_exit(tf.a0);

        case Number::Yield:
            return sys_yield();

        default:
            return kErrUnknownSyscall;
        }
    }
}
