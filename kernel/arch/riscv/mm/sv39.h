#ifndef __ARCH_RISCV_MM_SV39_H__
#define __ARCH_RISCV_MM_SV39_H__

#include <stdint.h>

namespace kernel::arch::riscv::mm {
    using Pte = uint64_t;

    inline constexpr uint64_t kSatpModeSv39 = 8ull << 60;

    inline constexpr uintptr_t kPageShift = 12;    //页内偏移 12 bit，所以 VPN 从 bit 12 开始
    inline constexpr uintptr_t kVpnBits = 9;
    inline constexpr uintptr_t kVpnMask = 0x1ff;    //Sv39 每级索引 9 bit
    inline constexpr uintptr_t kPtePpnShift = 10;    //PTE 的 PPN 从 bit 10 开始

    enum PteFlags : uint64_t {
        PteV = 1ull << 0,
        PteR = 1ull << 1,
        PteW = 1ull << 2,
        PteX = 1ull << 3,
        PteU = 1ull << 4,
        PteG = 1ull << 5,
        PteA = 1ull << 6,
        PteD = 1ull << 7,
    };

    constexpr uint64_t vpn_index(uintptr_t va, int level) noexcept {
        return (va >> (kPageShift + kVpnBits * level)) & kVpnMask;
    }

    constexpr Pte make_pte(uintptr_t pa, uint64_t flags) noexcept {
        return ((pa >> kPageShift) << kPtePpnShift) | flags;
    }

    constexpr uintptr_t pte_to_pa(Pte pte) noexcept {
        return ((pte >> kPtePpnShift) << kPageShift);
    }

    constexpr bool pte_valid(Pte pte) noexcept {
      return (pte & PteV) != 0;
    }

    constexpr bool pte_leaf(Pte pte) noexcept {
      return (pte & (PteR | PteW | PteX)) != 0;
    }
}

#endif