#ifndef __KERNEL_CORE_SCHED_RUN_QUEUE_H__
#define __KERNEL_CORE_SCHED_RUN_QUEUE_H__

#include <cstddef>

#include <kernel/core/task/task.h>
#include <kernel/lib/ds/queue.h>

namespace kernel::sched {

class RunQueue final {
public:
    static void init() noexcept;
    static bool push(kernel::task::Task* task) noexcept;
    static kernel::task::Task* current() noexcept;
    static kernel::task::Task* next() noexcept;

private:
    static inline kernel::task::Task* s_current = nullptr;
    static inline kernel::ds::Queue s_ready_queue {};
};

} // namespace kernel::sched

#endif
