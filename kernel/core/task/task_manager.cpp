#include <kernel/core/task/task_manager.h>

namespace kernel::task {

void TaskManager::init() noexcept {
    // TODO: initialize task table, task id allocator, and idle task metadata
}

Task* TaskManager::create_kernel_task(Entry entry, void* stack_base, size_t stack_size) noexcept {
    // TODO: allocate a task slot and build initial kernel task context
    // TODO: prepare context so first switch lands in task_entry_trampoline
    (void)entry;
    (void)stack_base;
    (void)stack_size;
    return nullptr;
}

Task* TaskManager::idle_task() noexcept {
    // TODO: return idle task pointer once idle task creation exists
    return nullptr;
}

} // namespace kernel::task
