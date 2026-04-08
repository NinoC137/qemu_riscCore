#ifndef __KERNEL_CORE_TASK_TASK_TYPES_H__
#define __KERNEL_CORE_TASK_TASK_TYPES_H__

#include <cstdint>

namespace kernel::task {

enum class State : uint32_t {
    Empty = 0,
    Ready,
    Running,
    Blocked,
    Dead,
};

using TaskId = uint32_t;
using Entry = void (*)();

} // namespace kernel::task

#endif
