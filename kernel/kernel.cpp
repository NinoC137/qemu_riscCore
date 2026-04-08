#include <stdint.h>
#include <kernel/arch/riscv/trap/trap.h>

#include <platform/riscv.h>
#include <platform/qemu/devices/timer/clint_timer.h>
#include <drivers/uart/uart.h>

static inline long do_syscall_write(long fd, const char* buf, long len)
{
    register long a0 asm("a0") = fd;
    register long a1 asm("a1") = reinterpret_cast<long>(buf);
    register long a2 asm("a2") = len;
    register long a7 asm("a7") = 1;

    asm volatile(
        "ecall"
        : "+r"(a0)
        : "r"(a1), "r"(a2), "r"(a7)
        : "memory");

    return a0;
}

static inline long do_syscall_exit(){
    register long a0 asm("a0") = 137;
    register long a7 asm("a7") = 2;

    asm volatile(
        "ecall"
        : "+r"(a0)
        : "r"(a7)
        : "memory");
        
    g_uart0.puts("after ecall\n");
    return 0;
}

extern "C" void kmain(void) {
    g_uart0.init();

    //uart tests
    // g_uart0.puts("Hello C++ kernel!");
    // g_uart0.putc('\n');
    // g_uart0.puts("UART ready, val = ");
    // g_uart0.put_hex(0x1234abcdULL);

    //trap tests
    trap::init();

    //init global interrupt
    riscv::set_mie_bits(1ull << 7); // MTIE
    riscv::set_mstatus_bits(1ull << 3); // MIE

    g_uart0.puts("trap installed\n");

    ClintTimer::schedule_after(10000000ULL);
    

    // g_uart0.puts("before ebreak\n");
    // asm volatile(".4byte 0x00100073");
    // g_uart0.puts("after ebreak\n");

    // g_uart0.puts("before ecall\n");

    // const char msg[] = "System Write: hello from syscall\n";
    // long ret = do_syscall_write(1, msg, sizeof(msg) - 1);

    // g_uart0.puts("illegal instruction test\n");
    // asm volatile(".word 0xffffffff");


    for (;;) {
        asm volatile("wfi");
    }
}
