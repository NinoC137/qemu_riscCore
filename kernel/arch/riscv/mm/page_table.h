#ifndef __ARCH_RISCV_PAGE_TABLE_H__
#define __ARCH_RISCV_PAGE_TABLE_H__
#include <stdint.h>

namespace kernel::arch::riscv::mm {
    // One Sv39 page table page: 512 entries * 8 bytes = 4 KiB.
    struct PageTable {
        uint64_t entries[512];
    };

    // Allocate and zero one page table page from PMM.
    PageTable* alloc_page_table() noexcept;

    // Map one 4 KiB virtual page to one physical page with Sv39 PTE flags.
    bool map_page(PageTable* root, uintptr_t va, uintptr_t pa, uint64_t flags) noexcept;

    // Map a page-aligned virtual range to a contiguous physical range.
    bool map_range(PageTable* root, uintptr_t va, uintptr_t pa, uintptr_t size, uint64_t flags) noexcept;

    // Remove a single 4 KiB leaf mapping. First version does not reclaim empty page tables.
    bool unmap_page(PageTable* root, uintptr_t va) noexcept;

    // Software page-table walk used for tests/debug; returns 0 on unmapped addresses.
    uintptr_t virt_to_phys(PageTable* root, uintptr_t va) noexcept;
}

#endif
