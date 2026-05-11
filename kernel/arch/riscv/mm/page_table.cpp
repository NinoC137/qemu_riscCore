#include <kernel/arch/riscv/mm/page_table.h>
#include <kernel/arch/riscv/mm/sv39.h>
#include <kernel/mm/page.h>
#include <kernel/mm/pmm.h>

#include <stddef.h>
#include <stdint.h>

namespace kernel::arch::riscv::mm {

    namespace {

        void zero_page(void* page) noexcept {
            auto* p = reinterpret_cast<uint64_t*>(page);
            for(size_t i = 0; i < sizeof(PageTable::entries)/sizeof(PageTable::entries[0]); ++i) {
                p[i] = 0;
            }
        }

        PageTable* pte_to_table(Pte pte) noexcept {
            return reinterpret_cast<PageTable*>(pte_to_pa(pte));
        }
    }

    PageTable* alloc_page_table() noexcept {
        void* page = kernel::mm::alloc_page();
        if(page == nullptr) {
            return nullptr;
        }
        zero_page(page);

        return reinterpret_cast<PageTable*>(page);
    }

    bool map_page(PageTable* root, uintptr_t va, uintptr_t pa, uint64_t flags) noexcept {
        if(root == nullptr) {
            return false;
        }

        if(!kernel::mm::is_page_aligned(va) ||
           !kernel::mm::is_page_aligned(pa)) {
            return false;
        }

        PageTable* table = root;

        for(int level = 2; level > 0; --level) {
            const uint64_t index = vpn_index(va, level);
            Pte& pte = table->entries[index];

            if(!pte_valid(pte)) {
                PageTable* next = alloc_page_table();
                if(next == nullptr) {
                    return false;
                }

                //中间级 PTE 只设置 V
                pte = make_pte(reinterpret_cast<uintptr_t>(next), PteV);
            }

            if(pte_leaf(pte)) {
                return false;
            }

            table = pte_to_table(pte);
        }

        const uint64_t index = vpn_index(va, 0);
        Pte& leaf = table->entries[index];

        if(pte_valid(leaf)) {
            return false;
        }

        // 最后一级叶子 PTE 设置 V | R/W/X
        leaf = make_pte(pa, flags | PteV);
        return true;
    }

    bool map_range(PageTable* root, uintptr_t va, uintptr_t pa, uintptr_t size, uint64_t flags) noexcept {
        if(size == 0) {
            return true;
        }

        if(!kernel::mm::is_page_aligned(va) ||
           !kernel::mm::is_page_aligned(pa)) {
           return false;
        }

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