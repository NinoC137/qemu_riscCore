#include <kernel/lib/ds/queue.h>

namespace kernel::ds {

void Queue::init() noexcept {
    // TODO: initialize underlying intrusive list container
}

bool Queue::empty() const noexcept {
    // TODO: forward to underlying list empty state
    return true;
}

void Queue::push(ListNode* node) noexcept {
    // TODO: enqueue node at logical tail
    (void)node;
}

ListNode* Queue::pop() noexcept {
    // TODO: dequeue node from logical head, return nullptr if empty
    return nullptr;
}

} // namespace kernel::ds
