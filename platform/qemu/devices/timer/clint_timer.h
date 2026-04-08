#ifndef __QEMU_VIRT_TIMER_H__
#define __QEMU_VIRT_TIMER_H__

#include <platform/qemu/memmap.h>

namespace ClintTimer
{
    //CPU时钟频率10Mhz
    static constexpr uint64_t INTERVAL_S = 10000000ULL; //每秒中断一次

   static inline uint64_t read_mtime() {
    return *reinterpret_cast<volatile uint64_t*>(platform::memmap::CLINT_MTIME.base);
   } 

   static inline void write_mtimecmp0(uint64_t value) {
    *reinterpret_cast<volatile uint64_t*>(platform::memmap::CLINT_MTIMECMP.base) = value;
   }

   static inline void interrupt_after(uint64_t value) {
    write_mtimecmp0(read_mtime() + value);
   }
} // namespace ClintTimer


#endif