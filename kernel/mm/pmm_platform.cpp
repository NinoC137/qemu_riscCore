#include <kernel/mm/pmm.h>

#include <platform/linker_symbols.h>
#include <platform/qemu/memmap.h>

namespace kernel::mm
{
    void pmm_init() noexcept
    {
        const ReservedRange reserved[] = {
            {platform::linker::kernel_start_addr(),
             platform::linker::kernel_end_addr() - platform::linker::kernel_start_addr()},
        };

        pmm_init(
            PhysicalRange{
                platform::memmap::kDram.base,
                platform::memmap::kDram.size,
            },
            reserved,
            1);
    }
} // namespace kernel::mm
