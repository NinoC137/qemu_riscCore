#ifndef __KERNEL_ADDRESS_SPACE_H__
#define __KERNEL_ADDRESS_SPACE_H__

#include <kernel/mm/pmm.h>
#include <kernel/arch/riscv/mm/mmu.h>

namespace kernel::mm {
    // Build the boot kernel address space with identity mappings for DRAM and MMIO.
    void kernel_address_space_init() noexcept;

    // Return the root page table used by the kernel address space.
    kernel::arch::riscv::mm::PageTable* kernel_page_table() noexcept;
}

#endif
