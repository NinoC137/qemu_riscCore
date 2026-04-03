//
// Created by Nino Zhu on 26-4-3.
//

#ifndef MEMMAP_H
#define MEMMAP_H

#include <stdint.h>

struct MemoryRegion {
    uintptr_t base;
    uintptr_t size;
};

namespace platform::memmap {
    inline constexpr MemoryRegion kDram = {0x80000000UL, 128 * 1024 * 1024UL};

    inline constexpr MemoryRegion kUart0 = {0x10000000UL, 0x100UL};

    inline constexpr MemoryRegion kClint = {0x02000000UL, 0x10000UL};

    inline constexpr MemoryRegion kPlic = {0x0C000000UL, 0x4000000UL};
}

#endif //MEMMAP_H
