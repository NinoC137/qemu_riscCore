#ifndef __ARCH_RISCV_PAGE_TABLE_H__
#define __ARCH_RISCV_PAGE_TABLE_H__
#include <stdint.h>

namespace kernel::arch::riscv::mm {
    struct PageTable {
        uint64_t entries[512];
    };

    PageTable* alloc_page_table() noexcept;

    bool map_page(PageTable* root, uintptr_t va, uintptr_t pa, uint64_t flags) noexcept;
    bool map_range(PageTable* root, uintptr_t va, uintptr_t pa, uintptr_t size, uint64_t flags) noexcept;

    bool unmap_page(PageTable* root, uintptr_t va) noexcept;
    uintptr_t virt_to_phys(PageTable* root, uintptr_t va) noexcept;
}

#endif