#ifndef __ARCH_RISCV_MMU_H__
#define __ARCH_RISCV_MMU_H__

#include <kernel/arch/riscv/mm/page_table.h>

namespace kernel::arch::riscv::mm {
    void enable_sv39(PageTable* root) noexcept;
    void sfence_vma() noexcept;
    uintptr_t make_satp(PageTable* root) noexcept;
}

#endif