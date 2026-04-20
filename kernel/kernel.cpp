#include <bootstrap.h>
#include <scheduler.h>
#include <stdint.h>
#include <syscall.h>
#include <kernel/arch/riscv/trap/trap.h>
#include <kernel/time/timer.h>
#include <kernel/core/task/task_manager.h>

#include <platform/riscv.h>

#include <drivers/uart/uart.h>


extern "C" void kmain(void) {
    g_uart0.init();

    //trap tests
    trap::init();

    g_uart0.puts("trap installed\n"); 

    g_cpu0timer.init(10000000ULL);

    g_uart0.puts("current ticks count: "); g_uart0.put_dec(g_cpu0timer.ticks()); g_uart0.putc('\n');

    g_uart0.puts("start tasks scheduler\n");
    kernel::init::bootstrap_scheduler();

    for (;;) {
        asm volatile("wfi");
    }
}
