#include <kernel/arch/riscv/mm/mmu.h>
#include <kernel/arch/riscv/mm/sv39.h>

#include <platform/riscv.h>

namespace kernel::arch::riscv::mm {
    // satp stores PPN, not the full physical address; ASID is kept zero for now.
	    uintptr_t make_satp(PageTable* root) noexcept {
	        const uintptr_t root_pa = reinterpret_cast<uintptr_t>(root);
	        return kSatpModeSv39 | (root_pa >> 12);
	    }

    // Enable Sv39 and immediately flush stale translations.
	    void enable_sv39(PageTable* root) noexcept {
	        ::riscv::write_satp(make_satp(root));
	        ::riscv::sfence_vma();
	    }

    // Architecture wrapper kept here so callers do not depend on CSR details.
	    void sfence_vma() noexcept {
	        ::riscv::sfence_vma();
	    }
}
