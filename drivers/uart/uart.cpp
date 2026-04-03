//
// Created by Nino Zhu on 26-4-3.
//

#include "uart.h"
#include <stdint.h>
#include <platform/memmap.h>

static inline void uart_putc(char c) {
    *(volatile uint8_t *)platform::memmap::kUart0.base= (uint8_t)c;
}

void uart_init() {

}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}