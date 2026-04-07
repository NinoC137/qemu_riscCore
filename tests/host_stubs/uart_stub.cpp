#include <drivers/uart/uart.h>

const UART g_uart0(0);

void UART::init() const noexcept {}
void UART::putc(char) const noexcept {}
void UART::puts(const char*) const noexcept {}
void UART::put_hex(uint64_t) const noexcept {}
void UART::put_dec(uint64_t) const noexcept {}
