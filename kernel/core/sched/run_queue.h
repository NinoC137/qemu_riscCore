#ifndef __KERNEL_CORE_SCHED_RUN_QUEUE_H__
#define __KERNEL_CORE_SCHED_RUN_QUEUE_H__

#include <cstddef>

#include <kernel/core/task/task.h>
#include <kernel/lib/ds/queue.h>
#include <kernel/lib/ds/intrusive.h>

namespace kernel::sched {

class RunQueue final {
public:
    static void init() noexcept;
    static bool push(task::Task* task) noexcept;
    static task::Task* current() noexcept;
    static bool set_current(task::Task* task) noexcept;
    static task::Task* next() noexcept;

private:
    static inline task::Task* s_current = nullptr;
    static inline ds::Queue s_ready_queue {};
};

} // namespace kernel::sched

#endif
