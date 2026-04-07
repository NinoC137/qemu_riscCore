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

    g_uart0.puts("before ecall\n");
    long arg0, arg1, arg2;
    arg0 = 0;
    arg1 = 0;
    arg2 = 0;
    register long a0 asm("a0") = arg0;
    register long a1 asm("a1") = arg1;
    register long a2 asm("a2") = arg2;
    register long a7 asm("a7") = 1;

    asm volatile(
        "ecall"
        : "+r"(a0)
        : "r"(a1), "r"(a2), "r"(a7)
        : "memory");

    g_uart0.puts("after ecall\n");

    for (;;) {
        asm volatile("wfi");
    }
}
