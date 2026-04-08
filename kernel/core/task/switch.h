#ifndef __KERNEL_CORE_TASK_SWITCH_H__
#define __KERNEL_CORE_TASK_SWITCH_H__

#include <kernel/core/task/context.h>

namespace kernel::task {

extern "C" void context_switch(Context* current, Context* next);
extern "C" void task_entry_trampoline();

} // namespace kernel::task

#endif
