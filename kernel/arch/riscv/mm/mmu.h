#ifndef __ARCH_RISCV_MMU_H__
#define __ARCH_RISCV_MMU_H__

#include <kernel/arch/riscv/mm/page_table.h>

namespace kernel::arch::riscv::mm {
    // Switch CPU address translation to the given Sv39 root page table.
    void enable_sv39(PageTable* root) noexcept;

    // Flush local address-translation cache after page-table or satp changes.
    void sfence_vma() noexcept;

    // Build the satp value for Sv39 mode using the root page table's physical page number.
    uintptr_t make_satp(PageTable* root) noexcept;
}

#endif
