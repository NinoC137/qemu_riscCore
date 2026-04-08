#ifndef __KERNEL_CORE_TASK_TASK_H__
#define __KERNEL_CORE_TASK_TASK_H__

#include <cstddef>
#include <cstdint>

#include <kernel/core/task/context.h>
#include <kernel/core/task/task_types.h>
#include <kernel/lib/ds/list.h>

namespace kernel::task {

struct Task {
    TaskId id;
    State state;
    kernel::ds::ListNode run_node;
    Context context;
    Entry entry;
    uint8_t* stack_base;
    size_t stack_size;
};

} // namespace kernel::task

#endif
