#ifndef __KERNEL_TIMER_H__
#define __KERNEL_TIMER_H__

#include <cstdint>

class KernelTimer final {
public:
    static void init(uint64_t interval) noexcept;
    static void on_interrupt() noexcept;
    static uint64_t ticks() noexcept;
private:
    static inline uint64_t s_interval = 0;
    static inline uint64_t s_ticks = 0;
};

extern KernelTimer g_cpu0timer;
#endif