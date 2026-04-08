#include <kernel/lib/ds/bitmap.h>

namespace kernel::ds {

Bitmap::Bitmap(uint64_t* words, size_t bit_count) noexcept
    : m_words(words), m_bit_count(bit_count) {
    // TODO: validate constructor assumptions if needed in debug builds
}

void Bitmap::clear_all() noexcept {
    // TODO: clear all backing words
}

void Bitmap::set(size_t bit) noexcept {
    // TODO: set target bit in backing storage
    (void)bit;
}

void Bitmap::clear(size_t bit) noexcept {
    // TODO: clear target bit in backing storage
    (void)bit;
}

bool Bitmap::test(size_t bit) const noexcept {
    // TODO: return whether target bit is set
    (void)bit;
    return false;
}

} // namespace kernel::ds
