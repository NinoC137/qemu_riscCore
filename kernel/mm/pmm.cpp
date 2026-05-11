#include <kernel/mm/pmm.h>
#include <kernel/mm/page.h>

namespace kernel::mm
{
    namespace
    {
        inline constexpr size_t kMaxManagedPages = 32768;
        inline constexpr size_t kBitsPerWord = 64;
        inline constexpr size_t kBitmapWords = (kMaxManagedPages + kBitsPerWord - 1) / kBitsPerWord;

        uint64_t g_used_bitmap[kBitmapWords] {};
        uintptr_t g_managed_base = 0;
        size_t g_total_pages = 0;
        size_t g_free_pages = 0;

        constexpr uint64_t bit_mask(size_t bit) noexcept {
            return uint64_t{1} << (bit % kBitsPerWord);
        }

        constexpr size_t word_index(size_t bit) noexcept {
            return bit / kBitsPerWord;
        }

        size_t pa_to_index(uintptr_t pa) noexcept {
            return static_cast<size_t>(pa - g_managed_base) >> kPageShift;
        }

        bool used_index(size_t index) noexcept {
            return (g_used_bitmap[word_index(index)] & bit_mask(index)) != 0;
        }

        void mark_used_index(size_t index) noexcept {
            const auto word = word_index(index);
            const auto mask = bit_mask(index);
            if((g_used_bitmap[word] & mask) == 0) {
                g_used_bitmap[word] |= mask;
                if(g_free_pages > 0) {
                    --g_free_pages;
                }
            }
        }

        void mark_free_index(size_t index) noexcept {
            const auto word = word_index(index);
            const auto mask = bit_mask(word);
            if((g_used_bitmap[word] & mask) != 0) {
                g_used_bitmap[word] &= ~mask;
                ++g_free_pages;
            }
        }

        void reserve_range(uintptr_t base, uintptr_t size) noexcept {
            if(size == 0 || g_total_pages == 0) {
                return;
            }

            const uintptr_t managed_start = g_managed_base;
            const uintptr_t managed_end = g_managed_base + g_total_pages * kPageSize;
            const uintptr_t reserve_start = align_down(base);
            const uintptr_t reserve_end = align_up(base + size);

            const uintptr_t start = reserve_start < managed_start ? managed_start : reserve_start;
            const uintptr_t end = reserve_end > managed_end ? managed_end : reserve_end;
            if(start >= end) {
                return;
            }

            for(uintptr_t pa = start; pa < end; pa += kPageSize) {
                mark_used_index(pa_to_index(pa));
            }
        }
    }

    void pmm_init(PhysicalRange managed, const ReservedRange* reserved, size_t reserved_count) noexcept {
        const uintptr_t base = align_up(managed.base);
        const uintptr_t end = align_down(managed.base + managed.size);

        for(auto &word : g_used_bitmap) {
            word = ~uint64_t{0};
        }
        g_managed_base = base;
        g_total_pages = 0;
        g_free_pages = 0;

        if(end <= base) {
            return;
        }

        const auto page_count = static_cast<size_t>((end - base) / kPageSize);
        g_total_pages = page_count < kMaxManagedPages ? page_count : kMaxManagedPages;

        for(size_t i = 0; i < g_total_pages; ++i) {
            mark_free_index(i);
        }

        for(size_t i = 0; i < reserved_count; ++i) {
            reserve_range(reserved[i].base, reserved[i].size);
        }
    }

    void* alloc_page() noexcept {
        for(size_t i = 0; i < g_total_pages; ++i) {
            if(!used_index(i)) {
                mark_used_index(i);
                return reinterpret_cast<void*>(g_managed_base + i * kPageSize);
            }
        }
        return nullptr;
    }

    bool free_page(void* page) noexcept {
        const auto pa = reinterpret_cast<uintptr_t>(page);
        if(!is_managed_page(pa) || !is_page_aligned(pa)) {
            return false;
        }

        const auto index = pa_to_index(pa);
        if(!used_index(index)) {
            return false;
        }

        mark_free_index(index);
        return true;
    }

    size_t total_pages() noexcept {
        return g_total_pages;
    }

    size_t free_pages() noexcept {
        return g_free_pages;
    }

    PmmStats get_stats() noexcept {
        return {
            g_total_pages,
            g_free_pages,
            g_total_pages - g_free_pages,
        };
    }

    bool is_managed_page(uintptr_t pa) noexcept {
        if(!is_page_aligned(pa)) {
            return false;
        }

        const uintptr_t start = g_managed_base;
        const uintptr_t end = g_managed_base + g_total_pages * kPageSize;
        return (pa >= start) && (pa < end);
    }

    bool is_page_free(uintptr_t pa) noexcept {
        if(!is_managed_page(pa)) {
            return false;
        }
        return !used_index(pa_to_index(pa));
    }

}