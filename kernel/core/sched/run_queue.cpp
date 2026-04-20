#include <kernel/core/sched/run_queue.h>

namespace kernel::sched {

void RunQueue::init() noexcept {
    s_ready_queue.init();
    s_current = nullptr;
}

bool RunQueue::push(task::Task* task) noexcept {
    // append a READY task into ready queue using intrusive run_node
    if(task == nullptr) return false;
    task->state = task::State::Ready;
    s_ready_queue.push(&task->run_node);
    return true;
}

task::Task* RunQueue::current() noexcept {
    return s_current;
}

bool RunQueue::set_current(task::Task* task) noexcept {
    if(task == nullptr) return false;
    s_current = task;
    return true;
}

task::Task* RunQueue::next() noexcept {
    // pick next runnable task from ready queue
    auto* node = s_ready_queue.pop();
    if(node == nullptr) return nullptr;

    // 找出链表节点所在的结构体的头部，即task结构体头部
    auto* task = KERNEL_CONTAINER_OF(node, task::Task, run_node);
    return task;
}

} // namespace kernel::sched
