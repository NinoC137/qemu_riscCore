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

size_t first_set_bit_offset(uint64_t word) noexcept
{
    for (size_t bit = 0; bit < kBitsPerWord; ++bit) {
        if ((word & (1ULL << bit)) != 0) {
            return bit;
        }
    }

    return kBitsPerWord;
}

} // namespace

namespace kernel::ds {

Bitmap::Bitmap(uint64_t* words, size_t bit_count) noexcept
    : m_words(words), m_bit_count(bit_count) {
}

void Bitmap::clear_all() noexcept {
    const size_t word_count = word_count_for_bits(m_bit_count);
    for(size_t i = 0; i < word_count; ++i) {
        m_words[i] = 0;
    }
}

void Bitmap::set(size_t bit) noexcept {
    if(bit >= m_bit_count) {
        return;
    }

    const size_t word_index = word_index_for_bit(bit);
    const size_t bit_offset = bit_offset_for_bit(bit);
    const uint64_t mask = 1ULL << bit_offset;
    m_words[word_index] |= mask;
}

void Bitmap::clear(size_t bit) noexcept {
    if(bit >= m_bit_count) {
        return;
    }
    const size_t word_index = word_index_for_bit(bit);
    const size_t bit_offset = bit_offset_for_bit(bit);
    const uint64_t mask = 1ULL << bit_offset;
    m_words[word_index] &= ~mask;
}

bool Bitmap::test(size_t bit) const noexcept {
    if(bit >= m_bit_count) {
        return false;
    }
    const size_t word_index = word_index_for_bit(bit);
    const size_t bit_offset = bit_offset_for_bit(bit);
    const uint64_t mask = 1ULL << bit_offset;
    return (m_words[word_index] & mask) != 0;
}

ptrdiff_t Bitmap::find_first_zero() const noexcept
{
    const size_t word_count = word_count_for_bits(m_bit_count);

    for(size_t index = 0; index < word_count; ++index) {
        uint64_t word = m_words[index];

        // 尾部有效位掩码，防止将位数不够的word中的bit位错判为有效的0
        if(index == word_count - 1 && (m_bit_count % kBitsPerWord) != 0) {
            const size_t valid_bits = m_bit_count % kBitsPerWord;
            const uint64_t valid_mask = (1ULL << valid_bits) - 1ULL;
            word |= ~valid_mask;
        }

        if(word == ~0ULL) {
            continue;
        }

        const uint64_t inverted = ~word;
        const size_t bit_offset = first_set_bit_offset(inverted);
        if (bit_offset == kBitsPerWord) {
            continue;
        }

        const size_t global_bit = index * kBitsPerWord + bit_offset;
        if(global_bit < m_bit_count) {
            return static_cast<ptrdiff_t>(global_bit);
        }
    }

    // bit位全部占用时返回-1
    return -1;
}


} // namespace kernel::ds
