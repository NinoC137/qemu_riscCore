#include <kernel/lib/ds/list.h>

namespace kernel::ds {

void List::init() noexcept {
    // 初始化哨兵头节点
    m_head.next = &m_head;
    m_head.prev = &m_head;
}

bool List::empty() const noexcept {
    if(m_head.next == &m_head) {
        return true;
    } else {
        return false;
    }
}

void List::push_front(ListNode* node) noexcept
{
    if(node == nullptr) {
        return;
    }
    node->prev = &m_head;
    node->next = m_head.next;
    m_head.next->prev = node;
    m_head.next = node;
}

void List::push_back(ListNode* node) noexcept {
    if(node == nullptr) {
        return;
    }
    node->next = &m_head;
    node->prev = m_head.prev;
    m_head.prev->next = node;
    m_head.prev = node;
}

ListNode* List::pop_front() noexcept {
    if(List::empty()) return nullptr;
    ListNode* node_tmp = m_head.next;
    List::remove(node_tmp);
    return node_tmp;
}

ListNode* List::pop_back() noexcept {
    if(List::empty()) return nullptr;
    ListNode* node_tmp = m_head.prev;
    List::remove(node_tmp);
    return node_tmp;
}

void List::remove(ListNode* node) noexcept {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

} // namespace kernel::ds
