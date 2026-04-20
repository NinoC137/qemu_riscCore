#include <cassert>
#include <iostream>

#include <kernel/lib/ds/intrusive.h>
#include <kernel/lib/ds/list.h>

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
        kernel::ds::List list {};
        list.init();

        assert(list.empty());
        assert(list.pop_front() == nullptr);
        assert(list.pop_back() == nullptr);
    }

    void test_push_front_keeps_lifo_order()
    {
        kernel::ds::List list {};
        list.init();

        Payload a {1, {}};
        Payload b {2, {}};
        Payload c {3, {}};

        list.push_front(&a.node);
        list.push_front(&b.node);
        list.push_front(&c.node);

        assert(!list.empty());
        assert(owner_of(list.pop_front())->value == 3);
        assert(owner_of(list.pop_front())->value == 2);
        assert(owner_of(list.pop_front())->value == 1);
        assert(list.empty());
    }

    void test_push_back_keeps_fifo_order()
    {
        kernel::ds::List list {};
        list.init();

        Payload a {1, {}};
        Payload b {2, {}};
        Payload c {3, {}};

        list.push_back(&a.node);
        list.push_back(&b.node);
        list.push_back(&c.node);

        assert(owner_of(list.pop_front())->value == 1);
        assert(owner_of(list.pop_front())->value == 2);
        assert(owner_of(list.pop_front())->value == 3);
        assert(list.empty());
    }

    void test_pop_back_returns_tail()
    {
        kernel::ds::List list {};
        list.init();

        Payload a {1, {}};
        Payload b {2, {}};
        Payload c {3, {}};

        list.push_back(&a.node);
        list.push_back(&b.node);
        list.push_back(&c.node);

        assert(owner_of(list.pop_back())->value == 3);
        assert(owner_of(list.pop_back())->value == 2);
        assert(owner_of(list.pop_back())->value == 1);
        assert(list.empty());
    }

    void test_remove_middle_node_preserves_neighbors()
    {
        kernel::ds::List list {};
        list.init();

        Payload a {1, {}};
        Payload b {2, {}};
        Payload c {3, {}};

        list.push_back(&a.node);
        list.push_back(&b.node);
        list.push_back(&c.node);

        list.remove(&b.node);

        assert(owner_of(list.pop_front())->value == 1);
        assert(owner_of(list.pop_front())->value == 3);
        assert(list.empty());
    }

    void test_mixed_front_back_operations()
    {
        kernel::ds::List list {};
        list.init();

        Payload a {1, {}};
        Payload b {2, {}};
        Payload c {3, {}};
        Payload d {4, {}};

        list.push_back(&a.node);
        list.push_front(&b.node);
        list.push_back(&c.node);
        list.push_front(&d.node);

        assert(owner_of(list.pop_front())->value == 4);
        assert(owner_of(list.pop_front())->value == 2);
        assert(owner_of(list.pop_front())->value == 1);
        assert(owner_of(list.pop_front())->value == 3);
        assert(list.empty());
    }
}

int main()
{
    test_empty_after_init();
    test_push_front_keeps_lifo_order();
    test_push_back_keeps_fifo_order();
    test_pop_back_returns_tail();
    test_remove_middle_node_preserves_neighbors();
    test_mixed_front_back_operations();

    std::cout << "list_unit_tests: all tests passed\n";
    return 0;
}
