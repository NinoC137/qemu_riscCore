#ifndef __KERNEL_CORE_TASK_TASK_MANAGER_H__
#define __KERNEL_CORE_TASK_TASK_MANAGER_H__

#include <cstddef>
#include <cstdint>

#include <kernel/core/task/task.h>

namespace kernel::task {

class TaskManager final {
public:
    static void init() noexcept;
    static Task* create_kernel_task(Entry entry, void* stack_base, size_t stack_size) noexcept;
    static Task* idle_task() noexcept;
};

} // namespace kernel::task

#endif
