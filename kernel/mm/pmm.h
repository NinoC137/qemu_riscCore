#ifndef KERNEL_MM_PMM_H
#define KERNEL_MM_PMM_H

#include <stddef.h>
#include <stdint.h>


namespace kernel::mm {
    struct PhysicalRange {
      uintptr_t base;
      uintptr_t size;
    };

    struct ReservedRange {
      uintptr_t base;
      uintptr_t size;
    };

    struct PmmStats {
      size_t total_pages;
      size_t free_pages;
      size_t used_pages;
    };

  void pmm_init(PhysicalRange managed, const ReservedRange* reserved, size_t reserved_count) noexcept;

  void pmm_init() noexcept;

  void* alloc_page() noexcept;
  bool free_page(void* page) noexcept;
  size_t total_pages() noexcept;
  size_t free_pages() noexcept;

  PmmStats get_stats() noexcept;
  bool is_managed_page(uintptr_t pa) noexcept;
  bool is_page_free(uintptr_t pa) noexcept;
}

#endif // KERNEL_MM_PMM_H
