#include <iterator>
#include <run_queue.h>
#include <switch.h>
#include <task_manager.h>
#include <kernel/core/sched/scheduler.h>

namespace kernel::sched {

void Scheduler::init() noexcept {
    // initialize run queue, current task state, and scheduler policy state
    RunQueue::init();
}

void Scheduler::start() noexcept {
    // start first runnable task; should not return in final design
    task::Task* next = RunQueue::next();
    if (next == nullptr) {
        next = task::TaskManager::idle_task();
    }

    RunQueue::set_current(next);
    next->state = task::State::Running;

    task::Context boot_ctx{};
    task::context_switch(&boot_ctx, &next->context);

    for(;;) {}
}

void Scheduler::yield() noexcept {
    // voluntarily yield current task and trigger reschedule
    reschedule();
}

void Scheduler::on_tick() noexcept {
    // TODO: account timer tick and decide whether preemption/reschedule is needed
}

void Scheduler::reschedule() noexcept {
    // TODO: switch from current task to next runnable task
    task::Task* prev = RunQueue::current();
    task::Task* next = RunQueue::next();

    if (next == nullptr) {
        next = task::TaskManager::idle_task();
    }

    if(next == nullptr || prev == nullptr) {
        return;
    }

    if(prev != nullptr && prev != task::TaskManager::idle_task() &&prev->state == task::State::Running) {
        prev->state = task::State::Ready;
        RunQueue::push(prev);
    }

    next->state = task::State::Running;
    RunQueue::set_current(next);

    context_switch(&prev->context, &next->context);
}

} // namespace kernel::sched
