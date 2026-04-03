//
// Created by Nino Zhu on 26-4-3.
//

#ifndef LINKER_SYMBOLS_H
#define LINKER_SYMBOLS_H

#include <stdint.h>

extern "C"
{
    extern char bss_start[];
    extern char bss_end[];
    extern char stack_top[];
    extern char global_pointer[];
}

#endif // LINKER_SYMBOLS_H
