#include <kernel/lib/ds/queue.h>

namespace kernel::ds {

void Queue::init() noexcept {
    m_list.init();
}

bool Queue::empty() const noexcept {
    if(m_list.empty()) return true;
    else return false;
}

void Queue::push(ListNode* node) noexcept {
    m_list.push_back(node);
}

ListNode* Queue::pop() noexcept {
    if(m_list.empty()) return nullptr;
    m_list.pop_front();
}

} // namespace kernel::ds
