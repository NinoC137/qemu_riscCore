#include <stdint.h>
#include <drivers/uart/uart.h>

extern "C" void kmain(void) {
    g_uart0.init();

    g_uart0.puts("Hello C++ kernel!");
    g_uart0.putc('\n');
    g_uart0.puts("UART ready, val = ");
    g_uart0.put_hex(0x1234abcdULL);

    for (;;) {}
}
