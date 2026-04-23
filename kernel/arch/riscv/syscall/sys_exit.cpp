#include <scheduler.h>
#include <kernel/arch/riscv/syscall/syscall.h>

#include <drivers/uart/uart.h>
#include <kernel/core/sched/run_queue.h>

extern const UART g_uart0;

namespace syscall
{
    uintptr_t sys_exit(uintptr_t code) noexcept
    {
        g_uart0.puts("sys_exit, code = ");
        g_uart0.put_dec(code);
        g_uart0.putc('\n');

        auto current_task = kernel::sched::RunQueue::current();
        if(current_task != nullptr) {
            current_task->state = kernel::task::State::Dead;
        }

        kernel::sched::Scheduler::reschedule();

        for (;;)
        {
            asm volatile("wfi");
        }
    }
}
