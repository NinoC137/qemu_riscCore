#ifndef __KERNEL_ADDRESS_SPACE_H__
#define __KERNEL_ADDRESS_SPACE_H__

#include <kernel/mm/pmm.h>
#include <kernel/arch/riscv/mm/mmu.h>

namespace kernel::mm {
    void kernel_address_space_init() noexcept;
    kernel::arch::riscv::mm::PageTable* kernel_page_table() noexcept;
}

#endif