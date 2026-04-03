//
// Created by Nino Zhu on 26-4-3.
//

#ifndef UART_H
#define UART_H

#include <stdint.h>

class UART final {
public:
    explicit constexpr UART(uintptr_t base) noexcept
        : base_(base){}

    void init() const noexcept;
    void putc(char ch) const noexcept;
    void puts(const char* s) const noexcept;
    void put_hex(uint64_t value) const noexcept;
    void put_dec(uint64_t value) const noexcept;

private:
    enum class Reg : uintptr_t {
        RBR_THR_DLL = 0x00,
        IER_DLM = 0x01,
        FCR_IIR = 0x02,
        LCR = 0x03,
        MCR = 0x04,
        LSR = 0x05,
    };

    volatile uint8_t& reg(Reg r) const noexcept;
    void write_reg(Reg r, uint8_t value) const noexcept;
    uint8_t read_reg(Reg r) const noexcept;
    void wait_tx_ready() const noexcept;
    void putc_raw(char ch) const noexcept;

    uintptr_t base_;
};

extern const UART g_uart0;

#endif //UART_H
