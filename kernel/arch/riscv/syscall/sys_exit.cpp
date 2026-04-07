#include <kernel/arch/riscv/syscall/syscall.h>

#include <drivers/uart/uart.h>

extern const UART g_uart0;

namespace syscall
{
    uintptr_t sys_exit(uintptr_t code) noexcept
    {
        g_uart0.puts("sys_exit, code = ");
        g_uart0.put_dec(code);
        g_uart0.putc('\n');

        for (;;)
        {
            asm volatile("wfi");
        }
    }
}
