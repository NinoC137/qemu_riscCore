#ifndef __KERNEL_LIB_DS_INTRUSIVE_H__
#define __KERNEL_LIB_DS_INTRUSIVE_H__

#include <cstddef>
#include <cstdint>

#define KERNEL_OFFSETOF(type, member) ((size_t) &(((type*)0)->member))
#define KERNEL_CONTAINER_OF(ptr, type, member) \
    ((type*)((uintptr_t)(ptr) - KERNEL_OFFSETOF(type, member)))

#endif
