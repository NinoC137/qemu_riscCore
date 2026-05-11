#include <cassert>
#include <cstdint>
#include <iostream>

#include <kernel/arch/riscv/mm/sv39.h>

namespace
{
    using namespace kernel::arch::riscv::mm;

    void test_vpn_index() noexcept
    {
        static_assert(vpn_index(0x0000000080000000ull, 0) == 0);
        static_assert(vpn_index(0x0000000080000000ull, 1) == 0);
        static_assert(vpn_index(0x0000000080000000ull, 2) == 2);

        assert(vpn_index(0x0000000080001000ull, 0) == 1);
        assert(vpn_index(0x0000000080200000ull, 1) == 1);
    }

    void test_pte_encode_decode() noexcept
    {
        constexpr uintptr_t pa = 0x0000000080000000ull;
        constexpr Pte pte = make_pte(pa, PteV | PteR | PteW | PteA | PteD);

        static_assert(pte_valid(pte));
        static_assert(pte_leaf(pte));
        static_assert(pte_to_pa(pte) == pa);

        assert((pte & PteV) != 0);
        assert((pte & PteR) != 0);
        assert((pte & PteW) != 0);
        assert((pte & PteX) == 0);
    }

    void test_non_leaf_pte() noexcept
    {
        constexpr Pte pte = make_pte(0x0000000080001000ull, PteV);

        static_assert(pte_valid(pte));
        static_assert(!pte_leaf(pte));
        static_assert(pte_to_pa(pte) == 0x0000000080001000ull);
    }
} // namespace

int main()
{
    test_vpn_index();
    test_pte_encode_decode();
    test_non_leaf_pte();

    std::cout << "sv39_unit_tests: all tests passed\n";
    return 0;
}
