#include <kernel/core/sched/scheduler.h>

namespace kernel::sched {

void Scheduler::init() noexcept {
    // TODO: initialize run queue, current task state, and scheduler policy state
}

void Scheduler::start() noexcept {
    // TODO: start first runnable task; should not return in final design
}

void Scheduler::yield() noexcept {
    // TODO: voluntarily yield current task and trigger reschedule
}

void Scheduler::on_tick() noexcept {
    // TODO: account timer tick and decide whether preemption/reschedule is needed
}

void Scheduler::reschedule() noexcept {
    // TODO: switch from current task to next runnable task
}

} // namespace kernel::sched
