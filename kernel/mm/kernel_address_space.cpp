#include <kernel/mm/kernel_address_space.h>

#include <kernel/arch/riscv/mm/page_table.h>
#include <kernel/arch/riscv/mm/sv39.h>
#include <kernel/mm/page.h>
#include <platform/qemu/memmap.h>

namespace kernel::mm {
    namespace
    {
        kernel::arch::riscv::mm::PageTable* g_kernel_page_table = nullptr;
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

    void kernel_address_space_init() noexcept {
        using namespace kernel::arch::riscv::mm;

        g_kernel_page_table = alloc_page_table();
        if(g_kernel_page_table == nullptr) {
            return;
        }

        map_range(
            g_kernel_page_table,
            platform::memmap::kDram.base,
            platform::memmap::kDram.base,
            platform::memmap::kDram.size,
            PteR | PteW | PteX | PteA | PteD);

        map_device_region(
            g_kernel_page_table,
            platform::memmap::kUart0.base,
            platform::memmap::kUart0.size);

        map_device_region(
            g_kernel_page_table,
            platform::memmap::kClint.base,
            platform::memmap::kClint.size);
    }

    kernel::arch::riscv::mm::PageTable* kernel_page_table() noexcept {
        return g_kernel_page_table;
    }
}