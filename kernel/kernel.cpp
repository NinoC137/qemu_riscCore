#include <scheduler.h>
#include <stdint.h>
#include <syscall.h>
#include <bootstrap.h>
#include <kernel/arch/riscv/trap/trap.h>
#include <kernel/time/timer.h>
#include <kernel/core/task/task_manager.h>

#include <platform/riscv.h>

#include <drivers/uart/uart.h>

namespace
{
    alignas(16) static uint8_t g_task_A_stack[1024];
    alignas(16) static uint8_t g_task_B_stack[1024];
}

void task_A() {
    for(;;) {
        g_uart0.puts("Hello from task_A\n");
        kernel::sched::Scheduler::yield();
    }
}

void task_B() {
    for(;;) {
        g_uart0.puts("Hello from task_B\n");
        kernel::sched::Scheduler::yield();
    }
}

extern "C" void kmain(void) {
    g_uart0.init();

    //trap tests
    trap::init();

    g_uart0.puts("trap installed\n"); 

    g_cpu0timer.init(10000000ULL);

    g_uart0.puts("current ticks count: "); g_uart0.put_dec(g_cpu0timer.ticks()); g_uart0.putc('\n');

    g_uart0.puts("start tasks scheduler\n");
    kernel::init::bootstrap_scheduler();

    g_uart0.puts("Task manager and scheduler inited\n");
    // auto* task_a_tcb = kernel::task::TaskManager::create_kernel_task(task_A, g_task_A_stack, sizeof(g_task_A_stack));
    // auto* task_b_tcb = kernel::task::TaskManager::create_kernel_task(task_B, g_task_B_stack, sizeof(g_task_B_stack));
    g_uart0.puts("Creating task A&B\n");

    // kernel::sched::RunQueue::push(task_a_tcb);
    // kernel::sched::RunQueue::push(task_b_tcb);

    for (;;) {
        asm volatile("wfi");
    }
}
