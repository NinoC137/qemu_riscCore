#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

#include <kernel/mm/page.h>
#include <kernel/mm/pmm.h>

namespace
{
    using kernel::mm::PhysicalRange;
    using kernel::mm::ReservedRange;

    void init_small_pmm() noexcept
    {
        constexpr PhysicalRange managed{0x80000000, 16 * kernel::mm::kPageSize};
        const ReservedRange reserved[] = {
            {0x80000000, kernel::mm::kPageSize},
            {0x80003010, 100},
            {0x90000000, kernel::mm::kPageSize},
        };

        kernel::mm::pmm_init(managed, reserved, 3);
    }

    void test_init_stats_and_reserved_pages() noexcept
    {
        init_small_pmm();

        assert(kernel::mm::total_pages() == 16);
        assert(kernel::mm::free_pages() == 14);
        assert(!kernel::mm::is_page_free(0x80000000));
        assert(!kernel::mm::is_page_free(0x80003000));
        assert(kernel::mm::is_page_free(0x80001000));
        assert(kernel::mm::is_page_free(0x80002000));
    }

    void test_alloc_returns_aligned_unreserved_pages() noexcept
    {
        init_small_pmm();

        auto* p0 = kernel::mm::alloc_page();
        auto* p1 = kernel::mm::alloc_page();

        assert(reinterpret_cast<uintptr_t>(p0) == 0x80001000);
        assert(reinterpret_cast<uintptr_t>(p1) == 0x80002000);
        assert(kernel::mm::is_page_aligned(reinterpret_cast<uintptr_t>(p0)));
        assert(kernel::mm::free_pages() == 12);
    }

    void test_free_reuses_page() noexcept
    {
        init_small_pmm();

        auto* p0 = kernel::mm::alloc_page();
        auto* p1 = kernel::mm::alloc_page();
        assert(p0 != nullptr);
        assert(p1 != nullptr);

        assert(kernel::mm::free_page(p0));
        assert(kernel::mm::free_pages() == 13);

        auto* p2 = kernel::mm::alloc_page();
        assert(p2 == p0);
        assert(kernel::mm::free_pages() == 12);
    }

    void test_reject_invalid_free() noexcept
    {
        init_small_pmm();

        assert(!kernel::mm::free_page(reinterpret_cast<void*>(0x80000001)));
        assert(!kernel::mm::free_page(reinterpret_cast<void*>(0x70000000)));

        auto* p = kernel::mm::alloc_page();
        assert(kernel::mm::free_page(p));
        assert(!kernel::mm::free_page(p));
    }

    void test_exhaustion() noexcept
    {
        init_small_pmm();

        std::vector<void*> pages;
        while (auto* page = kernel::mm::alloc_page())
        {
            pages.push_back(page);
        }

        assert(pages.size() == 14);
        assert(kernel::mm::free_pages() == 0);
        assert(kernel::mm::alloc_page() == nullptr);

        for (auto* page : pages)
        {
            assert(kernel::mm::free_page(page));
        }
        assert(kernel::mm::free_pages() == 14);
    }
} // namespace

int main()
{
    test_init_stats_and_reserved_pages();
    test_alloc_returns_aligned_unreserved_pages();
    test_free_reuses_page();
    test_reject_invalid_free();
    test_exhaustion();

    std::cout << "pmm_unit_tests: all tests passed\n";
    return 0;
}
