//
// Created by Nino Zhu on 26-4-3.
//

#ifndef PLATFORM_RISCV_H
#define PLATFORM_RISCV_H

#include <stdint.h>

namespace riscv
{

    using reg_t = uintptr_t;

    inline reg_t read_mcause() noexcept
    {
        reg_t value;
        asm volatile("csrr %0, mcause" : "=r"(value));
        return value;
    }

    inline reg_t read_mepc() noexcept
    {
        reg_t value;
        asm volatile("csrr %0, mepc" : "=r"(value));
        return value;
    }

    inline reg_t read_mtval() noexcept
    {
        reg_t value;
        asm volatile("csrr %0, mtval" : "=r"(value));
        return value;
    }

    inline reg_t read_mstatus() noexcept
    {
        reg_t value;
        asm volatile("csrr %0, mstatus" : "=r"(value));
        return value;
    }

    inline reg_t read_mhartid() noexcept
    {
        reg_t value;
        asm volatile("csrr %0, mhartid" : "=r"(value));
        return value;
    }

    inline void write_mtvec(reg_t value) noexcept
    {
        asm volatile("csrw mtvec, %0" ::"r"(value));
    }

    inline void write_mepc(reg_t value) noexcept
    {
        asm volatile("csrw mepc, %0" ::"r"(value));
    }

    inline void set_mstatus_bits(reg_t bits) noexcept
    {
        asm volatile("csrs mstatus, %0" ::"r"(bits) : "memory");
    }

    inline void clear_mstatus_bits(reg_t bits) noexcept
    {
        asm volatile("csrc mstatus, %0" ::"r"(bits) : "memory");
    }

    inline void set_mie_bits(reg_t bits) noexcept
    {
        asm volatile("csrs mie, %0" ::"r"(bits) : "memory");
    }

    inline void clear_mie_bits(reg_t bits) noexcept
    {
        asm volatile("csrc mie, %0" ::"r"(bits) : "memory");
    }

    constexpr reg_t kMstatusMie = reg_t{1} << 3;
    constexpr reg_t kMieMtie = reg_t{1} << 7;

    constexpr reg_t kMcauseInterruptBit = reg_t{1} << (sizeof(reg_t) * 8 - 1);

    inline bool mcause_is_interrupt(reg_t mcause) noexcept
    {
        return (mcause & kMcauseInterruptBit) != 0;
    }

    inline reg_t mcause_code(reg_t mcause) noexcept
    {
        return mcause & ~kMcauseInterruptBit;
    }

    enum class ExceptionCode : reg_t
    {
        InstructionAddressMisaligned = 0,
        InstructionAccessFault = 1,
        IllegalInstruction = 2,
        Breakpoint = 3,
        LoadAddressMisaligned = 4,
        LoadAccessFault = 5,
        StoreAmoAddressMisaligned = 6,
        StoreAmoAccessFault = 7,
        EcallFromUMode = 8,
        EcallFromSMode = 9,
        EcallFromMMode = 11,
        InstructionPageFault = 12,
        LoadPageFault = 13,
        StoreAmoPageFault = 15,
    };

    enum class InterruptCode : reg_t
    {
        MachineSoftware = 3,
        MachineTimer = 7,
        MachineExternal = 11,
    };

} // namespace riscv

#endif // PLATFORM_RISCV_H
