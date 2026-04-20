#ifndef __KERNEL_LIB_DS_LIST_H__
#define __KERNEL_LIB_DS_LIST_H__

#include <cstddef>

namespace kernel::ds {

struct ListNode {
    ListNode* prev;
    ListNode* next;
};

class List final {
public:
    void init() noexcept;
    bool empty() const noexcept;
    void push_front(ListNode* node) noexcept;
    void push_back(ListNode* node) noexcept;
    ListNode* pop_front() noexcept;
    ListNode* pop_back() noexcept;
    void remove(ListNode* node) noexcept;

private:
    // 哨兵节点，上述链表操作过程不涉及内存操作
    ListNode m_head {};
};

} // namespace kernel::ds

#endif
