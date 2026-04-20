#include <assert.h>

#include <kernel/lib/ds/bitmap.h>

namespace {

constexpr size_t kBitsPerWord = sizeof(uint64_t) * 8;

size_t word_count_for_bits(size_t bit_count) noexcept
{
    return (bit_count + kBitsPerWord - 1) / kBitsPerWord;
}

size_t word_index_for_bit(size_t bit) noexcept
{
    return bit / kBitsPerWord;
}

size_t bit_offset_for_bit(size_t bit) noexcept
{
    return bit % kBitsPerWord;
}

} // namespace

namespace kernel::ds {

Bitmap::Bitmap(uint64_t* words, size_t bit_count) noexcept
    : m_words(words), m_bit_count(bit_count) {
    assert(m_words != nullptr);
}

void Bitmap::clear_all() noexcept {
    const size_t word_count = word_count_for_bits(m_bit_count);
    for(size_t i = 0; i < word_count; ++i) {
        m_words[i] = 0;
    }
}

void Bitmap::set(size_t bit) noexcept {
    assert(bit < m_bit_count);
    const size_t word_index = word_index_for_bit(bit);
    const size_t bit_offset = bit_offset_for_bit(bit);
    const uint64_t mask = 1ULL << bit_offset;
    m_words[word_index] |= mask;
}

void Bitmap::clear(size_t bit) noexcept {
    assert(bit < m_bit_count);
    const size_t word_index = word_index_for_bit(bit);
    const size_t bit_offset = bit_offset_for_bit(bit);
    const uint64_t mask = 1ULL << bit_offset;
    m_words[word_index] &= ~mask;
}

bool Bitmap::test(size_t bit) const noexcept {
    assert(bit < m_bit_count);
    const size_t word_index = word_index_for_bit(bit);
    const size_t bit_offset = bit_offset_for_bit(bit);
    const uint64_t mask = 1ULL << bit_offset;
    return (m_words[word_index] & mask) != 0;
}

} // namespace kernel::ds
