#include <scheduler.h>
#include <kernel/core/init/bootstrap.h>
#include <syscall.h>
#include <uart.h>

namespace kernel::init {
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

extern "C" void task_bootstrap_entry(void) {
    task::Task* current = sched::RunQueue::current();
    current->entry();

    syscall::sys_exit(255);
    for(;;) {}
}

void bootstrap_scheduler() noexcept {
    // TODO: initialize scheduler and hand over first execution flow
    g_uart0.puts("Hello from bootstrap_scheduler\n");
    task::TaskManager::init();
    sched::Scheduler::init();
    g_uart0.puts("Task manager and scheduler inited\n");

    auto* task_a_tcb = task::TaskManager::create_kernel_task(task_A, g_task_A_stack, sizeof(g_task_A_stack));
    auto* task_b_tcb = task::TaskManager::create_kernel_task(task_B, g_task_B_stack, sizeof(g_task_B_stack));
    g_uart0.puts("Creating task A&B\n");

    sched::RunQueue::push(task_a_tcb);
    sched::RunQueue::push(task_b_tcb);

    g_uart0.puts("Start scheduler\n");
    sched::Scheduler::start();

    for(;;) {}
}

} // namespace kernel::init
