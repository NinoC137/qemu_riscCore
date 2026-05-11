#include <kernel/arch/riscv/mm/page_table.h>
#include <kernel/arch/riscv/mm/sv39.h>
#include <kernel/mm/page.h>
#include <kernel/mm/pmm.h>

#include <stddef.h>
#include <stdint.h>

namespace kernel::arch::riscv::mm {

	    namespace {

        // Newly allocated page tables must start empty; stale bits may look like valid PTEs.
	        void zero_page(void* page) noexcept {
	            auto* p = reinterpret_cast<uint64_t*>(page);
	            for(size_t i = 0; i < sizeof(PageTable::entries)/sizeof(PageTable::entries[0]); ++i) {
	                p[i] = 0;
	            }
	        }

        // Intermediate PTEs store the physical address of the next-level page table.
	        PageTable* pte_to_table(Pte pte) noexcept {
	            return reinterpret_cast<PageTable*>(pte_to_pa(pte));
	        }
	    }

    // Allocate one page-table page from PMM and clear all entries before use.
	    PageTable* alloc_page_table() noexcept {
	        void* page = kernel::mm::alloc_page();
	        if(page == nullptr) {
            return nullptr;
        }
        zero_page(page);

	        return reinterpret_cast<PageTable*>(page);
	    }

    // Build the missing intermediate tables and install one L0 leaf mapping.
	    bool map_page(PageTable* root, uintptr_t va, uintptr_t pa, uint64_t flags) noexcept {
        if(root == nullptr) {
            return false;
        }

        if(!kernel::mm::is_page_aligned(va) ||
           !kernel::mm::is_page_aligned(pa)) {
            return false;
        }

        // Walk from root level 2 down to level 1. Level 0 is handled as the leaf.
	        PageTable* table = root;

        for(int level = 2; level > 0; --level) {
            const uint64_t index = vpn_index(va, level);
            Pte& pte = table->entries[index];

            // Missing intermediate PTE means the next-level page table does not exist yet.
	            if(!pte_valid(pte)) {
	                PageTable* next = alloc_page_table();
                if(next == nullptr) {
                    return false;
                }

                //中间级 PTE 只设置 V
                pte = make_pte(reinterpret_cast<uintptr_t>(next), PteV);
            }

            // A leaf before level 0 would be a huge-page mapping; not supported here yet.
	            if(pte_leaf(pte)) {
	                return false;
	            }

            table = pte_to_table(pte);
        }

        const uint64_t index = vpn_index(va, 0);
        Pte& leaf = table->entries[index];

        // Keep the first version conservative: do not overwrite existing mappings.
	        if(pte_valid(leaf)) {
	            return false;
	        }

        // 最后一级叶子 PTE 设置 V | R/W/X
        leaf = make_pte(pa, flags | PteV);
	        return true;
	    }

    // Map each page in a range by repeatedly calling map_page().
	    bool map_range(PageTable* root, uintptr_t va, uintptr_t pa, uintptr_t size, uint64_t flags) noexcept {
        if(size == 0) {
            return true;
        }

        if(!kernel::mm::is_page_aligned(va) ||
           !kernel::mm::is_page_aligned(pa)) {
           return false;
        }

        // Treat the range as [va, align_up(va + size)).
	        const uintptr_t end = kernel::mm::align_up(va + size);

        for(uintptr_t cur_va = va, cur_pa = pa;
            cur_va < end;
            cur_va += kernel::mm::kPageSize, cur_pa += kernel::mm::kPageSize) {
            if(!map_page(root, cur_va, cur_pa, flags)) {
                return false;
            }
        }

	        return true;
	    }

    // Walk page tables in software and reconstruct the final physical address.
	    uintptr_t virt_to_phys(PageTable* root, uintptr_t va) noexcept {
        if(root == nullptr) {
            return 0;
        }

        PageTable* table = root;

        for(int level = 2; level >= 0; --level) {
            const uint64_t index = vpn_index(va, level);
            const Pte pte = table->entries[index];

            if(!pte_valid(pte)) {
                return 0;
            }

            // Offset is preserved by page translation; only VPN is replaced by PPN.
	            if(pte_leaf(pte)) {
	                const uintptr_t page_base = pte_to_pa(pte);
                return page_base + kernel::mm::page_offset(va);
            }

            table = pte_to_table(pte);
        }

	        return 0;
	    }

	    //第一版 unmap_page() 只清叶子，不回收空页表页
	    bool unmap_page(PageTable* root, uintptr_t va) noexcept {
        if(root == nullptr || !kernel::mm::is_page_aligned(va)) {
            return false;
        }

        // Only traverse intermediate levels. Level 0 is the leaf table.
	        PageTable* table = root;

        for(int level = 2; level > 0; --level) {
            const Pte pte = table->entries[vpn_index(va, level)];

            if(!pte_valid(pte) || pte_leaf(pte)) {
                return false;
            }

            table = pte_to_table(pte);
        }

        Pte& leaf = table->entries[vpn_index(va, 0)];
        if(!pte_valid(leaf) || !pte_leaf(leaf)) {
            return false;
        }

        leaf = 0;
        return true;
    }

}
