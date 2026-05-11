#include <cassert>
#include <cstdint>
#include <iostream>

#include <kernel/mm/page.h>

namespace
{
    void test_page_constants() noexcept
    {
        static_assert(kernel::mm::kPageShift == 12);
        static_assert(kernel::mm::kPageSize == 4096);
        static_assert(kernel::mm::kPageMask == 4095);
    }

    void test_alignment() noexcept
    {
        using namespace kernel::mm;

        assert(is_page_aligned(0));
        assert(is_page_aligned(0x1000));
        assert(!is_page_aligned(0x1001));
        assert(!is_page_aligned(0x1fff));

        assert(align_down(0x1000) == 0x1000);
        assert(align_down(0x1001) == 0x1000);
        assert(align_down(0x1fff) == 0x1000);
        assert(align_down(0x2000) == 0x2000);

        assert(align_up(0x1000) == 0x1000);
        assert(align_up(0x1001) == 0x2000);
        assert(align_up(0x1fff) == 0x2000);
        assert(align_up(0x2000) == 0x2000);
    }

    void test_page_numbers() noexcept
    {
        using namespace kernel::mm;

        assert(pa_to_pfn(0x80000000) == 0x80000);
        assert(pfn_to_pa(0x80000) == 0x80000000);
        assert(page_offset(0x80001234) == 0x234);
        assert(pages_offset(0x80005678) == 0x678);
    }

    void test_bytes_to_pages() noexcept
    {
        using namespace kernel::mm;

        assert(bytes_to_pages(0) == 0);
        assert(bytes_to_pages(1) == 1);
        assert(bytes_to_pages(4095) == 1);
        assert(bytes_to_pages(4096) == 1);
        assert(bytes_to_pages(4097) == 2);
    }
} // namespace

int main()
{
    test_page_constants();
    test_alignment();
    test_page_numbers();
    test_bytes_to_pages();

    std::cout << "page_unit_tests: all tests passed\n";
    return 0;
}
