#include <kernel/arch/riscv/mm/mmu.h>
#include <kernel/arch/riscv/mm/sv39.h>

#include <platform/riscv.h>

namespace kernel::arch::riscv::mm {
    uintptr_t make_satp(PageTable* root) noexcept {
        const uintptr_t root_pa = reinterpret_cast<uintptr_t>(root);
        return kSatpModeSv39 | (root_pa >> 12);
    }

    void enable_sv39(PageTable* root) noexcept {
        ::riscv::write_satp(make_satp(root));
        ::riscv::sfence_vma();
    }

    void sfence_vma() noexcept {
        ::riscv::sfence_vma();
    }
}