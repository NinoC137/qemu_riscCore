#include <cassert>
#include <iostream>

#include <kernel/lib/ds/intrusive.h>
#include <kernel/lib/ds/queue.h>

namespace
{
    struct Payload
    {
        int value;
        kernel::ds::ListNode node;
    };

    Payload* owner_of(kernel::ds::ListNode* node)
    {
        return KERNEL_CONTAINER_OF(node, Payload, node);
    }

    void test_empty_after_init()
    {
        kernel::ds::Queue queue {};
        queue.init();

        assert(queue.empty());
        assert(queue.pop() == nullptr);
    }

    void test_push_pop_is_fifo()
    {
        kernel::ds::Queue queue {};
        queue.init();

        Payload a {1, {}};
        Payload b {2, {}};
        Payload c {3, {}};

        queue.push(&a.node);
        queue.push(&b.node);
        queue.push(&c.node);

        assert(!queue.empty());
        assert(owner_of(queue.pop())->value == 1);
        assert(owner_of(queue.pop())->value == 2);
        assert(owner_of(queue.pop())->value == 3);
        assert(queue.empty());
    }

    void test_interleaved_push_and_pop()
    {
        kernel::ds::Queue queue {};
        queue.init();

        Payload a {1, {}};
        Payload b {2, {}};
        Payload c {3, {}};

        queue.push(&a.node);
        queue.push(&b.node);
        assert(owner_of(queue.pop())->value == 1);

        queue.push(&c.node);
        assert(owner_of(queue.pop())->value == 2);
        assert(owner_of(queue.pop())->value == 3);
        assert(queue.pop() == nullptr);
        assert(queue.empty());
    }

    void test_queue_can_be_reused_after_draining()
    {
        kernel::ds::Queue queue {};
        queue.init();

        Payload a {1, {}};
        Payload b {2, {}};

        queue.push(&a.node);
        assert(owner_of(queue.pop())->value == 1);
        assert(queue.empty());

        queue.push(&b.node);
        assert(owner_of(queue.pop())->value == 2);
        assert(queue.empty());
    }
}

int main()
{
    test_empty_after_init();
    test_push_pop_is_fifo();
    test_interleaved_push_and_pop();
    test_queue_can_be_reused_after_draining();

    std::cout << "queue_unit_tests: all tests passed\n";
    return 0;
}
