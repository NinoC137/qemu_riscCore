//
// Created by Nino Zhu on 26-4-3.
//

#include "uart.h"
#include <platform/memmap.h>
#include <stdlib.h>

namespace {
    constexpr uint8_t kLcr8n1 = 0x03;
    constexpr uint8_t kFcrEnableFifo = 0x01;
    constexpr uint8_t kFcrClearRxTx = 0x06;
    constexpr uint8_t kLsrThrEmpty = 0x20;
}

const UART g_uart0(platform::memmap::kUart0.base);

void UART::init() const noexcept {
    return;
}

volatile uint8_t& UART::reg(Reg r) const noexcept {
    return *reinterpret_cast<volatile uint8_t*>(
        base_ + static_cast<uintptr_t>(r));
}

void UART::write_reg(Reg r, uint8_t value) const noexcept {
    reg(r) = value;
}

uint8_t UART::read_reg(Reg r) const noexcept {
    return reg(r);
}

void UART::wait_tx_ready() const noexcept {
    while(read_reg(Reg::LSR) & kLsrThrEmpty == 0){

    }
}

void UART::putc_raw(char ch) const noexcept {
    wait_tx_ready();
    write_reg(Reg::RBR_THR_DLL, static_cast<uint8_t>(ch));
}

void UART::putc(char ch) const noexcept {
    if(ch == '\n'){
        putc_raw('\r');
    }
    putc_raw(ch);
}

void UART::puts(const char *s) const noexcept {
    if(!s){
        return;
    }
    while(*s){
        putc(*s++);
    }
}

void UART::put_hex(uint64_t value) const noexcept {
    static constexpr char kHex[] = "0123456789abcdef";
    putc('0');
    putc('x');

    for(int shift = 60; shift >= 0; shift -= 4){
        const auto nibble = 
            static_cast<uint8_t>((value >> shift) & 0xFULL);
        putc(kHex[nibble]);       
    }
}

void UART::put_dec(uint64_t value) const noexcept {
    if(value == 0) {
        putc('0');
        return;
    }

    char buf[20];
    int i = 0;

    //依次填充单个数字，从低位到高位排布整个数字
    while(value > 0){
        buf[i++] = static_cast<char>('0' + (value % 10));
        value /= 10;
    }
    //从高到低输出数字，实现方向正确
    while(i > 0){
        putc(buf[--i]);
    }
}