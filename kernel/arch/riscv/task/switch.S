.section .text

# Context layout
# 0   ra
# 8   sp
# 16  s0
# 24  s1
# 32  s2
# 40  s3
# 48  s4
# 56  s5
# 64  s6
# 72  s7
# 80  s8
# 88  s9
# 96  s10
# 104 s11

.globl context_switch
context_switch:
    #save current task callee-saved context into *a0
    #restore next task callee-saved context from *a1
    #return into next task execution flow

    # 把 CPU 当前寄存器写入 current 指向的内存
    sd ra, 0(a0) #返回地址寄存器return address
    sd sp, 8(a0) #栈指针Stack pointer
    sd s0, 16(a0)
    sd s1, 24(a0)
    sd s2, 32(a0)
    sd s3, 40(a0)
    sd s4, 48(a0)
    sd s5, 56(a0)
    sd s6, 64(a0)
    sd s7, 72(a0)
    sd s8, 80(a0)
    sd s9, 88(a0)
    sd s10, 96(a0)
    sd s11, 104(a0)

    ld ra, 0(a1) #开始装载next的上下文 
    ld sp, 8(a1) 
    ld s0, 16(a1)
    ld s1, 24(a1)
    ld s2, 32(a1)
    ld s3, 40(a1)
    ld s4, 48(a1)
    ld s5, 56(a1)
    ld s6, 64(a1)
    ld s7, 72(a1)
    ld s8, 80(a1)
    ld s9, 88(a1)
    ld s10, 96(a1)
    ld s11, 104(a1)

    ret

.globl task_entry_trampoline
task_entry_trampoline:
    # TODO: establish a clean first-entry path for newly created kernel tasks
    # TODO: jump into task entry function and handle unexpected return
    call task_bootstrap_entry
1:
    j 1b
