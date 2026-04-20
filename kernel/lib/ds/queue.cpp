#include <kernel/lib/ds/queue.h>

namespace kernel::ds {

void Queue::init() noexcept {
    m_list.init();
}

bool Queue::empty() const noexcept {
    return m_list.empty();
}

void Queue::push(ListNode* node) noexcept {
    m_list.push_back(node);
}

ListNode* Queue::pop() noexcept {
    if(m_list.empty()) return nullptr;
    return m_list.pop_front();
}

} // namespace kernel::ds
