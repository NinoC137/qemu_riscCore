#include <kernel/time/timer.h>

#include <platform/qemu/devices/timer/clint_timer.h>
#include <platform/riscv.h>
#include <drivers/uart/uart.h>

KernelTimer g_cpu0timer;

void KernelTimer::init(uint64_t interval) noexcept {
    s_interval = interval;
    ClintTimer::interrupt_after(interval);
    //init global interrupt
    //要先配置中断间隔，再开中断，否则会无法正常配置
    riscv::set_mie_bits(1ull << 7); // MTIE
    riscv::set_mstatus_bits(1ull << 3); // MIE
}

void KernelTimer::on_interrupt() noexcept {
    ++s_ticks;
    g_uart0.puts("current timer count: "); g_uart0.put_dec(ClintTimer::read_mtime()); g_uart0.putc('\n');
    g_uart0.puts("current ticks count: "); g_uart0.put_dec(g_cpu0timer.ticks()); g_uart0.putc('\n');
    
    ClintTimer::interrupt_after(s_interval);
}

uint64_t KernelTimer::ticks() noexcept {
    
    return s_ticks;
}