#include <cassert>
#include <cstdint>
#include <iostream>

#include <kernel/arch/riscv/mm/page_table.h>
#include <kernel/arch/riscv/mm/sv39.h>
#include <kernel/mm/page.h>
#include <kernel/mm/pmm.h>

namespace
{
    using namespace kernel::arch::riscv::mm;

    void init_test_pmm() noexcept
    {
        constexpr kernel::mm::PhysicalRange managed{
            0x80000000ull,
            64 * kernel::mm::kPageSize,
        };

        kernel::mm::pmm_init(managed, nullptr, 0);
    }

    void clear_page_table(PageTable& table) noexcept
    {
        for (auto& entry : table.entries)
        {
            entry = 0;
        }
    }

    void test_map_page_and_translate() noexcept
    {
        init_test_pmm();

        PageTable root{};
        clear_page_table(root);

        constexpr uintptr_t va = 0x0000000040000000ull;
        constexpr uintptr_t pa = 0x0000000080000000ull;

        assert(map_page(&root, va, pa, PteR | PteW | PteA | PteD));
        assert(virt_to_phys(&root, va) == pa);
        assert(virt_to_phys(&root, va + 0x123) == pa + 0x123);
    }

    void test_reject_duplicate_mapping() noexcept
    {
        init_test_pmm();

        PageTable root{};
        clear_page_table(root);

        constexpr uintptr_t va = 0x0000000040000000ull;
        constexpr uintptr_t pa = 0x0000000080000000ull;

        assert(map_page(&root, va, pa, PteR | PteW | PteA | PteD));
        assert(!map_page(&root, va, pa + kernel::mm::kPageSize, PteR | PteW | PteA | PteD));
    }

    void test_map_range() noexcept
    {
        init_test_pmm();

        PageTable root{};
        clear_page_table(root);

        constexpr uintptr_t va = 0x0000000040000000ull;
        constexpr uintptr_t pa = 0x0000000080000000ull;
        constexpr uintptr_t size = 3 * kernel::mm::kPageSize;

        assert(map_range(&root, va, pa, size, PteR | PteW | PteA | PteD));
        assert(virt_to_phys(&root, va) == pa);
        assert(virt_to_phys(&root, va + kernel::mm::kPageSize) == pa + kernel::mm::kPageSize);
        assert(virt_to_phys(&root, va + 2 * kernel::mm::kPageSize + 0x77) ==
               pa + 2 * kernel::mm::kPageSize + 0x77);
    }

    void test_unmap_page() noexcept
    {
        init_test_pmm();

        PageTable root{};
        clear_page_table(root);

        constexpr uintptr_t va = 0x0000000040000000ull;
        constexpr uintptr_t pa = 0x0000000080000000ull;

        assert(map_page(&root, va, pa, PteR | PteW | PteA | PteD));
        assert(virt_to_phys(&root, va) == pa);
        assert(unmap_page(&root, va));
        assert(virt_to_phys(&root, va) == 0);
        assert(!unmap_page(&root, va));
    }

    void test_reject_unaligned_mapping() noexcept
    {
        init_test_pmm();

        PageTable root{};
        clear_page_table(root);

        assert(!map_page(&root, 0x40000001ull, 0x80000000ull, PteR | PteW));
        assert(!map_page(&root, 0x40000000ull, 0x80000001ull, PteR | PteW));
    }
} // namespace

int main()
{
    test_map_page_and_translate();
    test_reject_duplicate_mapping();
    test_map_range();
    test_unmap_page();
    test_reject_unaligned_mapping();

    std::cout << "page_table_unit_tests: all tests passed\n";
    return 0;
}
