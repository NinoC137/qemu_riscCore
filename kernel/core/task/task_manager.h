#ifndef __KERNEL_CORE_TASK_TASK_MANAGER_H__
#define __KERNEL_CORE_TASK_TASK_MANAGER_H__

#include <cstddef>
#include <cstdint>

#include <kernel/core/task/task.h>
#include <kernel/core/task/switch.h>

namespace kernel::task {

// 简单的静态任务表
namespace
{
    constexpr size_t kMaxTasks = 32;
    alignas(16) static Task s_tasks[kMaxTasks];
    static Task* s_idle_task = nullptr;
    static TaskId s_next_id = 1;
}

class TaskManager final {
public:
    static void init() noexcept;
    static Task* create_kernel_task(Entry entry, void* stack_base, size_t stack_size) noexcept;
    static Task* idle_task() noexcept;
};

} // namespace kernel::task

#endif
