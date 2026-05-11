//
// Created by Nino Zhu on 26-4-3.
//

#ifndef LINKER_SYMBOLS_H
#define LINKER_SYMBOLS_H

#include <cstdint>

extern "C"
{
    extern char kernel_start[];
    extern char kernel_end[];

    extern char text_start[];
    extern char text_end[];
    extern char rodata_start[];
    extern char rodata_end[];
    extern char data_start[];
    extern char data_end[];
    extern char bss_start[];
    extern char bss_end[];

    extern char boot_stack_bottom[];
    extern char boot_stack_top[];
    extern char stack_bottom[];
    extern char stack_top[];

    extern char heap_start[];
    extern char global_pointer[];
}

namespace platform::linker
{
    inline uintptr_t addr(const char *symbol) noexcept
    {
        return reinterpret_cast<uintptr_t>(symbol);
    }

    inline uintptr_t kernel_start_addr() noexcept { return addr(kernel_start); }
    inline uintptr_t kernel_end_addr() noexcept { return addr(kernel_end); }

    inline uintptr_t text_start_addr() noexcept { return addr(text_start); }
    inline uintptr_t text_end_addr() noexcept { return addr(text_end); }
    inline uintptr_t rodata_start_addr() noexcept { return addr(rodata_start); }
    inline uintptr_t rodata_end_addr() noexcept { return addr(rodata_end); }
    inline uintptr_t data_start_addr() noexcept { return addr(data_start); }
    inline uintptr_t data_end_addr() noexcept { return addr(data_end); }
    inline uintptr_t bss_start_addr() noexcept { return addr(bss_start); }
    inline uintptr_t bss_end_addr() noexcept { return addr(bss_end); }

    inline uintptr_t boot_stack_bottom_addr() noexcept { return addr(boot_stack_bottom); }
    inline uintptr_t boot_stack_top_addr() noexcept { return addr(boot_stack_top); }
    inline uintptr_t stack_bottom_addr() noexcept { return addr(stack_bottom); }
    inline uintptr_t stack_top_addr() noexcept { return addr(stack_top); }

    inline uintptr_t heap_start_addr() noexcept { return addr(heap_start); }
    inline uintptr_t global_pointer_addr() noexcept { return addr(global_pointer); }
} // namespace platform::linker

#endif // LINKER_SYMBOLS_H
