#include <trap.h>

#include <drivers/uart/uart.h>
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

    g_uart0.puts("\n=== trap ===\n");
    g_uart0.puts("mcause = "); g_uart0.put_hex(tf->mcause); g_uart0.putc('\n');
    g_uart0.puts("mepc = "); g_uart0.put_hex(tf->mepc); g_uart0.putc('\n');
    g_uart0.puts("mtval = "); g_uart0.put_hex(tf->mtval); g_uart0.putc('\n');
    g_uart0.puts("mstatus = "); g_uart0.put_hex(tf->mstatus); g_uart0.putc('\n');
    g_uart0.puts("hart = "); g_uart0.put_dec(tf->mhartid); g_uart0.putc('\n');

    if (is_interrupt) {
        g_uart0.puts("type = interrupt: ");
        g_uart0.puts(trap::interrupt_name(code));
        g_uart0.putc('\n');

        // timer 以后在这里接
        for (;;) {
            asm volatile("wfi");
        }
    }

    g_uart0.puts("type = exception: ");
    g_uart0.puts(trap::exception_name(code));
    g_uart0.putc('\n');

    if(code == static_cast<uintptr_t>(riscv::ExceptionCode::Breakpoint)){
        //先只测试固定32位ebreak
        tf->mepc += 4;
        g_uart0.puts("resume after breakpoint\n");
        return;
    }

    for(;;){
        asm volatile("wfi");
    }
}

