#include <stdint.h>
#include <drivers/uart/uart.h>

extern "C" void kmain(void);

extern "C" void kmain(void) {
    uart_puts("Hello Kernel!\n");
    for (;;) {}
}
