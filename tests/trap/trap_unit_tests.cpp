#include <cassert>
#include <cstdint>
#include <iostream>

#include <kernel/arch/riscv/trap/trap.h>
#include <kernel/arch/riscv/trap/trap_logic.h>
#include <platform/riscv.h>

namespace
{
    trap::TrapFrame make_base_tf()
    {
        trap::TrapFrame tf{};
        tf.mepc = 0x1000;
        tf.mcause = 0;
        tf.mtval = 0;
        tf.mstatus = 0;
        tf.mhartid = 0;
        return tf;
    }

    void test_classify_exception()
    {
        auto tf = make_base_tf();
        tf.mcause = static_cast<uintptr_t>(riscv::ExceptionCode::Breakpoint);

        auto summary = trap::classify_trap(tf);
        assert(summary.is_interrupt == false);
        assert(summary.code == static_cast<uintptr_t>(riscv::ExceptionCode::Breakpoint));
    }

    void test_classify_interrupt()
    {
        auto tf = make_base_tf();
        tf.mcause = riscv::kMcauseInterruptBit |
                    static_cast<uintptr_t>(riscv::InterruptCode::MachineTimer);

        auto summary = trap::classify_trap(tf);
        assert(summary.is_interrupt == true);
        assert(summary.code == static_cast<uintptr_t>(riscv::InterruptCode::MachineTimer));
    }

    void test_handle_breakpoint_advances_mepc_by_4()
    {
        auto tf = make_base_tf();
        tf.mepc = 0x2000;

        auto summary = trap::handle_breakpoint(tf);
        assert(tf.mepc == 0x2004);
        assert(summary.result == trap::HandleResult::Resume);
        assert(summary.code == static_cast<uintptr_t>(riscv::ExceptionCode::Breakpoint));
    }

    void test_handle_ecall_advances_mepc_and_sets_return_value()
    {
        auto tf = make_base_tf();
        tf.mepc = 0x3000;
        tf.a0 = 1;
        tf.a7 = 42;

        auto summary = trap::handle_ecall_mmode(tf, 1234);
        assert(tf.mepc == 0x3004);
        assert(tf.a0 == 1234);
        assert(summary.result == trap::HandleResult::Resume);
        assert(summary.code == static_cast<uintptr_t>(riscv::ExceptionCode::EcallFromMMode));
    }

    void test_syscall_register_accessors()
    {
        auto tf = make_base_tf();
        tf.a0 = 11;
        tf.a1 = 22;
        tf.a2 = 33;
        tf.a7 = 99;

        assert(trap::syscall_number(tf) == 99);
        assert(trap::syscall_arg0(tf) == 11);
        assert(trap::syscall_arg1(tf) == 22);
        assert(trap::syscall_arg2(tf) == 33);

        trap::set_syscall_return(tf, 77);
        assert(tf.a0 == 77);
    }
}

int main()
{
    test_classify_exception();
    test_classify_interrupt();
    test_handle_breakpoint_advances_mepc_by_4();
    test_handle_ecall_advances_mepc_and_sets_return_value();
    test_syscall_register_accessors();

    std::cout << "trap_unit_tests: all tests passed\n";
    return 0;
}
