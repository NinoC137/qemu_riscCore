#include <kernel/core/sched/run_queue.h>

namespace kernel::sched {

void RunQueue::init() noexcept {
    // TODO: initialize ready queue container and clear current task pointer
}

bool RunQueue::push(kernel::task::Task* task) noexcept {
    // TODO: append a READY task into ready queue using intrusive run_node
    (void)task;
    return false;
}

kernel::task::Task* RunQueue::current() noexcept {
    // TODO: return current running task pointer
    return s_current;
}

kernel::task::Task* RunQueue::next() noexcept {
    // TODO: pick next runnable task from ready queue
    return nullptr;
}

} // namespace kernel::sched
