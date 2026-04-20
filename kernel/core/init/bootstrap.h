#ifndef __KERNEL_CORE_INIT_BOOTSTRAP_H__
#define __KERNEL_CORE_INIT_BOOTSTRAP_H__

#include <kernel/core/task/task.h>
#include <kernel/core/task/task_manager.h>
#include <kernel/core/sched/run_queue.h>

namespace kernel::init {

extern "C" void task_bootstrap_entry(void);
void bootstrap_scheduler() noexcept;

} // namespace kernel::init

#endif
