#include <kernel/mm/kernel_address_space.h>

#include <kernel/arch/riscv/mm/page_table.h>
#include <kernel/arch/riscv/mm/sv39.h>
#include <kernel/mm/page.h>
#include <platform/qemu/memmap.h>

namespace kernel::mm {
	    namespace
	    {
	        kernel::arch::riscv::mm::PageTable* g_kernel_page_table = nullptr;

        // MMIO regions must be mapped too; drivers still access their physical addresses.
	        bool map_device_region(kernel::arch::riscv::mm::PageTable* root,
            uintptr_t base,
            uintptr_t size) noexcept {
            using namespace kernel::arch::riscv::mm;

            return map_range(
                root,
                align_down(base),
                align_down(base),
                align_up(size),
                PteR | PteW | PteA | PteD);
        }
	    }

    // First-stage kernel page table: identity map DRAM plus the devices used during boot.
	    void kernel_address_space_init() noexcept {
        using namespace kernel::arch::riscv::mm;

        g_kernel_page_table = alloc_page_table();
        if(g_kernel_page_table == nullptr) {
            return;
        }

        // Early bring-up maps all DRAM RWX; permissions can be tightened after paging is stable.
	        map_range(
	            g_kernel_page_table,
            platform::memmap::kDram.base,
            platform::memmap::kDram.base,
            platform::memmap::kDram.size,
            PteR | PteW | PteX | PteA | PteD);

        // Keep UART usable after paging so faults and boot progress can still be printed.
	        map_device_region(
	            g_kernel_page_table,
            platform::memmap::kUart0.base,
            platform::memmap::kUart0.size);

        // Keep machine timer MMIO usable after paging.
	        map_device_region(
	            g_kernel_page_table,
            platform::memmap::kClint.base,
            platform::memmap::kClint.size);
	    }

    // Expose the current kernel root page table to MMU enable and debug code.
	    kernel::arch::riscv::mm::PageTable* kernel_page_table() noexcept {
        return g_kernel_page_table;
    }
}
