#include <kernel/arch/riscv/syscall/syscall.h>

#include <drivers/uart/uart.h>

extern const UART g_uart0;

namespace syscall
{
    namespace
    {
        constexpr uintptr_t kFdStdout = 1;
        constexpr uintptr_t kFdStderr = 2;
        constexpr uintptr_t kErrBadFd = static_cast<uintptr_t>(-1);
        constexpr uintptr_t kErrBadPtr = static_cast<uintptr_t>(-2);
    }

    uintptr_t sys_write(uintptr_t fd, const void *buf, uintptr_t len) noexcept
    {
        if (fd != kFdStdout && fd != kFdStderr)
        {
            return kErrBadFd;
        }

        if (buf == nullptr)
        {
            return kErrBadPtr;
        }

        const char *p = static_cast<const char *>(buf);
        for (uintptr_t i = 0; i < len; ++i)
        {
            g_uart0.putc(p[i]);
        }

        return len;
    }
}
