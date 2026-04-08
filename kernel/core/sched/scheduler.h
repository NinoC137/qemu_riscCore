#ifndef __KERNEL_CORE_SCHED_SCHEDULER_H__
#define __KERNEL_CORE_SCHED_SCHEDULER_H__

#include <cstdint>

namespace kernel::sched {

class Scheduler final {
public:
    static void init() noexcept;
    static void start() noexcept;
    static void yield() noexcept;
    static void on_tick() noexcept;
    static void reschedule() noexcept;
};

} // namespace kernel::sched

#endif
