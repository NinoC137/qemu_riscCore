#ifndef __KERNEL_LIB_DS_BITMAP_H__
#define __KERNEL_LIB_DS_BITMAP_H__

#include <cstddef>
#include <cstdint>

namespace kernel::ds {

class Bitmap final {
public:
    Bitmap(uint64_t* words, size_t bit_count) noexcept;

    void clear_all() noexcept;
    void set(size_t bit) noexcept;
    void clear(size_t bit) noexcept;
    bool test(size_t bit) const noexcept;

private:
    uint64_t* m_words;
    size_t m_bit_count;
};

} // namespace kernel::ds

#endif
