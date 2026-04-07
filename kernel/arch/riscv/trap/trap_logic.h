#ifndef KERNEL_ARCH_RISCV_TRAP_TRAP_LOGIC_H
#define KERNEL_ARCH_RISCV_TRAP_TRAP_LOGIC_H

#include <stdint.h>
#include <kernel/arch/riscv/trap/trap.h>
#include <platform/riscv.h>

namespace trap
{

    enum class HandleResult
    {
        Resume,
        Fatal,
    };

    struct HandleSummary
    {
        bool is_interrupt;
        uintptr_t code;
        HandleResult result;
    };

    constexpr uintptr_t kInsnBytes32 = 4;

    enum class SyscallNumber : uintptr_t {
        Write = 1,
        Exit = 2,
        Yield = 3,
    };

    static uintptr_t syscall_dispatch(trap::TrapFrame* tf){
        g_uart0.puts("start syscall dispatch\n");
        g_uart0.puts("get tf a7: ");
        g_uart0.put_dec(tf->a7);    g_uart0.putc('\n');
        
        switch (static_cast<SyscallNumber>(tf->a7))
        {
        case SyscallNumber::Write:
            g_uart0.puts("call system write\n");
            return static_cast<uintptr_t>(-1);
        case SyscallNumber::Exit:
            g_uart0.puts("call system exit\n");
            return static_cast<uintptr_t>(-1);
        case SyscallNumber::Yield:
            g_uart0.puts("call system Yield\n");
            return static_cast<uintptr_t>(-1);

        default:
            return static_cast<uintptr_t>(-1);
        }
    }

    inline uintptr_t syscall_number(const TrapFrame &tf) noexcept
    {
        return tf.a7;
    }

    inline uintptr_t syscall_arg0(const TrapFrame &tf) noexcept
    {
        return tf.a0;
    }

    inline uintptr_t syscall_arg1(const TrapFrame &tf) noexcept
    {
        return tf.a1;
    }

    inline uintptr_t syscall_arg2(const TrapFrame &tf) noexcept
    {
        return tf.a2;
    }

    inline void set_syscall_return(TrapFrame &tf, uintptr_t value) noexcept
    {
        tf.a0 = value;
    }

    inline HandleSummary handle_breakpoint(TrapFrame &tf) noexcept
    {
        tf.mepc += kInsnBytes32;
        return {false, static_cast<uintptr_t>(riscv::ExceptionCode::Breakpoint), HandleResult::Resume};
    }

    inline HandleSummary handle_ecall_mmode(TrapFrame &tf, uintptr_t syscall_ret) noexcept
    {
        tf.mepc += kInsnBytes32;
        tf.a0 = syscall_ret;
        syscall_dispatch(&tf);
        // g_uart0.puts("\nin ecall handle.\n");
        return {false, static_cast<uintptr_t>(riscv::ExceptionCode::EcallFromMMode), HandleResult::Resume};
    }

    inline HandleSummary classify_trap(const TrapFrame &tf) noexcept
    {
        return {
            riscv::mcause_is_interrupt(tf.mcause),
            riscv::mcause_code(tf.mcause),
            HandleResult::Resume,
        };
    }

} // namespace trap

#endif
