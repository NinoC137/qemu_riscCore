#include <cassert>
#include <cstdint>
#include <iostream>

#include <kernel/lib/ds/bitmap.h>

namespace
{
    void test_clear_all_zeros_all_words()
    {
        uint64_t words[3] = {
            0xffffffffffffffffULL,
            0xaaaaaaaaaaaaaaaaULL,
            0x1234567890abcdefULL,
        };

        kernel::ds::Bitmap bitmap(words, 130);
        bitmap.clear_all();

        assert(words[0] == 0);
        assert(words[1] == 0);
        assert(words[2] == 0);
    }

    void test_set_and_test_bits_within_single_word()
    {
        uint64_t words[2] = {0, 0};
        kernel::ds::Bitmap bitmap(words, 100);

        bitmap.set(0);
        bitmap.set(5);
        bitmap.set(63);

        assert(bitmap.test(0));
        assert(bitmap.test(5));
        assert(bitmap.test(63));
        assert(!bitmap.test(1));
        assert(!bitmap.test(62));
    }

    void test_set_and_clear_bits_across_word_boundary()
    {
        uint64_t words[3] = {0, 0, 0};
        kernel::ds::Bitmap bitmap(words, 130);

        bitmap.set(63);
        bitmap.set(64);
        bitmap.set(129);

        assert(bitmap.test(63));
        assert(bitmap.test(64));
        assert(bitmap.test(129));

        bitmap.clear(64);
        assert(bitmap.test(63));
        assert(!bitmap.test(64));
        assert(bitmap.test(129));
    }
}

int main()
{
    test_clear_all_zeros_all_words();
    test_set_and_test_bits_within_single_word();
    test_set_and_clear_bits_across_word_boundary();

    std::cout << "bitmap_unit_tests: all tests passed\n";
    return 0;
}
