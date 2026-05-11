#ifndef _PAGE_H
#define _PAGE_H

#include <stddef.h>
#include <stdint.h>

namespace kernel::mm {
    inline constexpr uintptr_t kPageShift = 12;
    inline constexpr uintptr_t kPageSize = uintptr_t{1} << kPageShift;
    inline constexpr uintptr_t kPageMask = kPageSize - 1;

    constexpr bool is_page_aligned(uintptr_t addr) noexcept {
        return (addr & kPageMask) == 0;
    }

    constexpr uintptr_t align_down(uintptr_t addr) noexcept {
        return addr & ~kPageMask;
    }

    constexpr uintptr_t align_up(uintptr_t addr) noexcept {
        return (addr + kPageMask) & ~kPageMask;
    }

    constexpr uintptr_t pa_to_pfn(uintptr_t pa) noexcept {
        return pa >> kPageShift;
    }

    constexpr uintptr_t pfn_to_pa(uintptr_t pfn) noexcept {
        return pfn << kPageShift;
    }

    constexpr size_t bytes_to_pages(size_t bytes) noexcept {
        return static_cast<size_t>(bytes + kPageMask) >> kPageShift;
    }

    constexpr uintptr_t page_offset(uintptr_t addr) noexcept {
        return addr & kPageMask;
    }
}

#endif