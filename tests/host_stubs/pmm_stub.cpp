#include <kernel/mm/pmm.h>

#include <kernel/mm/page.h>

namespace kernel::mm
{
    namespace
    {
        inline constexpr size_t kHostStubPages = 128;

        alignas(kPageSize) unsigned char g_pages[kHostStubPages][kPageSize] {};
        bool g_used[kHostStubPages] {};
        size_t g_total_pages = kHostStubPages;
        size_t g_free_pages = kHostStubPages;

        size_t page_index(uintptr_t pa) noexcept
        {
            const uintptr_t base = reinterpret_cast<uintptr_t>(&g_pages[0][0]);
            return static_cast<size_t>((pa - base) / kPageSize);
        }
    } // namespace

    void pmm_init(PhysicalRange, const ReservedRange*, size_t) noexcept
    {
        for (size_t i = 0; i < kHostStubPages; ++i)
        {
            g_used[i] = false;
            for (size_t j = 0; j < kPageSize; ++j)
            {
                g_pages[i][j] = 0;
            }
        }
        g_total_pages = kHostStubPages;
        g_free_pages = kHostStubPages;
    }

    void pmm_init() noexcept
    {
        pmm_init({}, nullptr, 0);
    }

    void* alloc_page() noexcept
    {
        for (size_t i = 0; i < kHostStubPages; ++i)
        {
            if (!g_used[i])
            {
                g_used[i] = true;
                --g_free_pages;
                return &g_pages[i][0];
            }
        }
        return nullptr;
    }

    bool free_page(void* page) noexcept
    {
        const auto pa = reinterpret_cast<uintptr_t>(page);
        if (!is_managed_page(pa))
        {
            return false;
        }

        const size_t index = page_index(pa);
        if (!g_used[index])
        {
            return false;
        }

        g_used[index] = false;
        ++g_free_pages;
        return true;
    }

    size_t total_pages() noexcept
    {
        return g_total_pages;
    }

    size_t free_pages() noexcept
    {
        return g_free_pages;
    }

    PmmStats get_stats() noexcept
    {
        return {g_total_pages, g_free_pages, g_total_pages - g_free_pages};
    }

    bool is_managed_page(uintptr_t pa) noexcept
    {
        if (!is_page_aligned(pa))
        {
            return false;
        }

        const uintptr_t base = reinterpret_cast<uintptr_t>(&g_pages[0][0]);
        const uintptr_t end = base + kHostStubPages * kPageSize;
        return pa >= base && pa < end;
    }

    bool is_page_free(uintptr_t pa) noexcept
    {
        if (!is_managed_page(pa))
        {
            return false;
        }
        return !g_used[page_index(pa)];
    }
} // namespace kernel::mm
