.section .text

.globl context_switch
context_switch:
    # TODO: save current task callee-saved context into *a0
    # TODO: restore next task callee-saved context from *a1
    # TODO: return into next task execution flow
    ret

.globl task_entry_trampoline
task_entry_trampoline:
    # TODO: establish a clean first-entry path for newly created kernel tasks
    # TODO: jump into task entry function and handle unexpected return
    ret
