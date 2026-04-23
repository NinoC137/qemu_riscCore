#include <scheduler.h>
#include <kernel/core/init/bootstrap.h>
#include <syscall.h>
#include <uart.h>

namespace kernel::init {

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


    // for(;;) {}
}

} // namespace kernel::init
