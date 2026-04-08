#ifndef __KERNEL_CORE_INIT_BOOTSTRAP_H__
#define __KERNEL_CORE_INIT_BOOTSTRAP_H__

namespace kernel::init {

void bootstrap_tasks() noexcept;
void bootstrap_scheduler() noexcept;

} // namespace kernel::init

#endif
