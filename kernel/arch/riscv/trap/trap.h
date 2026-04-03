#ifndef KERNEL_ARCH_RISCV_TRAP_TRAP_H
#define KERNEL_ARCH_RISCV_TRAP_TRAP_H

#include <stdint.h>

namespace trap
{

    struct TrapFrame
    {
        uintptr_t ra;
        uintptr_t gp;
        uintptr_t tp;
        uintptr_t t0;
        uintptr_t t1;
        uintptr_t t2;
        uintptr_t s0;
        uintptr_t s1;
        uintptr_t a0;
        uintptr_t a1;
        uintptr_t a2;
        uintptr_t a3;
        uintptr_t a4;
        uintptr_t a5;
        uintptr_t a6;
        uintptr_t a7;
        uintptr_t s2;
        uintptr_t s3;
        uintptr_t s4;
        uintptr_t s5;
        uintptr_t s6;
        uintptr_t s7;
        uintptr_t s8;
        uintptr_t s9;
        uintptr_t s10;
        uintptr_t s11;
        uintptr_t t3;
        uintptr_t t4;
        uintptr_t t5;
        uintptr_t t6;
        uintptr_t sp;

        uintptr_t mepc;
        uintptr_t mcause;
        uintptr_t mtval;
        uintptr_t mstatus;
        uintptr_t mhartid;
    };

    void init() noexcept;

} // namespace trap

extern "C" void trap_entry();
extern "C" void trap_handle(trap::TrapFrame *tf);

#endif