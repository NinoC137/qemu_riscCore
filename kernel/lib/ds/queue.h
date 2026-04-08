#ifndef __KERNEL_LIB_DS_QUEUE_H__
#define __KERNEL_LIB_DS_QUEUE_H__

#include <kernel/lib/ds/list.h>

namespace kernel::ds {

class Queue final {
public:
    void init() noexcept;
    bool empty() const noexcept;
    void push(ListNode* node) noexcept;
    ListNode* pop() noexcept;

private:
    List m_list {};
};

} // namespace kernel::ds

#endif
