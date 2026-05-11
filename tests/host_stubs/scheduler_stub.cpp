#include <kernel/core/sched/run_queue.h>
#include <kernel/core/sched/scheduler.h>

namespace kernel::sched {

task::Task* RunQueue::current() noexcept {
    return nullptr;
}

void Scheduler::reschedule() noexcept {
}

} // namespace kernel::sched
