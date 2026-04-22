#include <scheduler.h>
#include <syscall.h>
#include <kernel/core/task/task_manager.h>

namespace kernel::task {

static uint8_t s_idle_stack[256];
static void idle_task_entry() {
    for(;;) {
        asm volatile("wfi");
    }
}

void TaskManager::init() noexcept {
    // TODO: initialize task table, task id allocator, and idle task metadata
    s_next_id = 1;

    s_idle_task = create_kernel_task(idle_task_entry, s_idle_stack, sizeof(s_idle_stack));

    for(auto & s_task : s_tasks) {
        s_task.state = State::Empty;
        s_task.stack_base = nullptr;
        s_task.stack_size = 0;
        s_task.entry = nullptr;
    }
}

Task* TaskManager::create_kernel_task(Entry entry, void* stack_base, size_t stack_size) noexcept {
    //  allocate a task slot and build initial kernel task context
    //  prepare context so first switch lands in task_entry_trampoline
    if(stack_base == nullptr || stack_size == 0) {
        return nullptr;
    }

    // 遍历数组任务表，找出其中为空的表项，分配给待创建的当前任务
    Task* task = nullptr;
    for(auto & s_task : s_tasks) {
        if(s_task.state == State::Empty) {
            task = &s_task;
            break;
        }
    }

    if(task == nullptr) {
        return nullptr;
    }

    task->id = s_next_id++;
    task->state = State::Ready;
    task->entry = entry;
    task->stack_base = static_cast<uint8_t*>(stack_base);
    task->stack_size = stack_size;

    task->run_node.prev = nullptr;
    task->run_node.next = nullptr;

    // 初始化函数返回地址
    task->context.ra = reinterpret_cast<uint64_t>(task_entry_trampoline);
    // 初始化栈区
    uintptr_t sp = reinterpret_cast<uintptr_t>(task->stack_base) + task->stack_size;
    sp &= ~static_cast<uintptr_t>(0xFUL); //16字节对齐
    task->context.sp = sp;

    task->context.s0 = 0;
    task->context.s1 = 0;
    task->context.s2 = 0;
    task->context.s3 = 0;
    task->context.s4 = 0;
    task->context.s5 = 0;
    task->context.s6 = 0;
    task->context.s7 = 0;
    task->context.s8 = 0;
    task->context.s9 = 0;
    task->context.s10 = 0;
    task->context.s11 = 0;

    return task;
}


Task* TaskManager::idle_task() noexcept {
    if(s_idle_task != nullptr) return s_idle_task;
    else return nullptr;
}

} // namespace kernel::task
