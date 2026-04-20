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
    ptrdiff_t find_first_zero() const noexcept;

private:
    uint64_t* m_words;  // map数组总大小
    size_t m_bit_count; // bit资源数
};

} // namespace kernel::ds

#endif
