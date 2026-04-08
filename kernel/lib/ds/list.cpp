#include <kernel/lib/ds/list.h>

namespace kernel::ds {

void List::init() noexcept {
    // TODO: initialize sentinel head node for intrusive doubly linked list
}

bool List::empty() const noexcept {
    // TODO: return whether the list has no payload nodes
    return true;
}

void List::push_front(ListNode* node) noexcept {
    // TODO: insert node right after head sentinel
    (void)node;
}

void List::push_back(ListNode* node) noexcept {
    // TODO: insert node right before tail position (head sentinel in circular form)
    (void)node;
}

ListNode* List::pop_front() noexcept {
    // TODO: remove and return first payload node, or nullptr when empty
    return nullptr;
}

ListNode* List::pop_back() noexcept {
    // TODO: remove and return last payload node, or nullptr when empty
    return nullptr;
}

void List::remove(ListNode* node) noexcept {
    // TODO: unlink node from intrusive list without freeing owner object
    (void)node;
}

} // namespace kernel::ds
