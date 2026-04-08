#include <trap.h>

#include <drivers/uart/uart.h>
#include <kernel/time/timer.h>
#include <platform/qemu/devices/timer/clint_timer.h>
#include <kernel/arch/riscv/trap/trap_logic.h>
#include <platform/riscv.h>

extern const UART g_uart0;

namespace trap
{
    void init() noexcept
    {
        // 声明好trap入口点
        riscv::write_mtvec(reinterpret_cast<uintptr_t>(&trap_entry));
    }

    static const char *exception_name(uintptr_t code) noexcept
    {
        using E = riscv::ExceptionCode;
        switch (static_cast<E>(code))
        {
        case E::IllegalInstruction:
            return "illegal instruction";
        case E::Breakpoint:
            return "breakpoint";
        case E::LoadAccessFault:
            return "load access fault";
        case E::StoreAmoAccessFault:
            return "store/amo access fault";
        case E::EcallFromMMode:
            return "ecall from M-mode";
        default:
            return "unknown exception";
        }
    }

    static const char* interrupt_name(uintptr_t code) noexcept {
        using I = riscv::InterruptCode;
        switch (static_cast<I>(code)) {
            case I::MachineTimer: return "machine timer interrupt";
            case I::MachineSoftware: return "machine software interrupt";
            case I::MachineExternal: return "machine external interrupt";
            default: return "unknown interrupt";
        }
    }
} // namespace trap

extern "C" void trap_handle(trap::TrapFrame* tf) {
    const bool is_interrupt = riscv::mcause_is_interrupt(tf->mcause);
    const uintptr_t code = riscv::mcause_code(tf->mcause);

    // g_uart0.puts("\n=== trap ===\n");
    // g_uart0.puts("mcause = "); g_uart0.put_hex(tf->mcause); g_uart0.putc('\n');
    // g_uart0.puts("mepc = "); g_uart0.put_hex(tf->mepc); g_uart0.putc('\n');
    // g_uart0.puts("mtval = "); g_uart0.put_hex(tf->mtval); g_uart0.putc('\n');
    // g_uart0.puts("mstatus = "); g_uart0.put_hex(tf->mstatus); g_uart0.putc('\n');
    // g_uart0.puts("hart = "); g_uart0.put_dec(tf->mhartid); g_uart0.putc('\n');

    if (is_interrupt) {
        g_uart0.puts("type = interrupt: ");
        g_uart0.puts(trap::interrupt_name(code));
        g_uart0.putc('\n');


        if(code == static_cast<uintptr_t>(riscv::InterruptCode::MachineTimer)){
            g_uart0.puts("catch a timer interrupt\n");
            
            KernelTimer::on_interrupt();

            return;
        }

        for (;;) {
            asm volatile("wfi");
        }
    }

    g_uart0.puts("type = exception: ");
    g_uart0.puts(trap::exception_name(code));
    g_uart0.putc('\n');

    if(code == static_cast<uintptr_t>(riscv::ExceptionCode::Breakpoint)){
        trap::handle_breakpoint(*tf);
        g_uart0.puts("resume after breakpoint\n");
        return;
    }

    if(code == static_cast<uintptr_t>(riscv::ExceptionCode::EcallFromMMode)){
        trap::handle_ecall_mmode(*tf);
        // g_uart0.puts("catch Ecall from M-mode\n");
        return;
    }

    if(code == static_cast<uintptr_t>(riscv::ExceptionCode::IllegalInstruction)){
        g_uart0.puts("Excpetion: Illegal instruction\n");
    }

    for(;;){
        asm volatile("wfi");
    }
}

