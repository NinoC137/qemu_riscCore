#include <stdint.h>
#include <drivers/uart/uart.h>
#include <kernel/arch/riscv/trap/trap.h>

extern "C" void kmain(void) {
    g_uart0.init();

    //uart tests
    // g_uart0.puts("Hello C++ kernel!");
    // g_uart0.putc('\n');
    // g_uart0.puts("UART ready, val = ");
    // g_uart0.put_hex(0x1234abcdULL);

    //trap tests
    trap::init();
    g_uart0.puts("trap installed\n");

    g_uart0.puts("before ebreak\n");
    asm volatile(".4byte 0x00100073");
    g_uart0.puts("after ebreak\n");

    for (;;) {
        asm volatile("wfi");
    }
}
