.section .init
.option norvc

.type start, @function
.global start
start:
	.option push
	.option norelax
	la gp, global_pointer
	.option pop

	csrw satp, zero
	la sp, stack_top

	la t5, bss_start
	la t6, bss_end
1:
	bgeu t5, t6, 2f
	sd zero, 0(t5)
	addi t5, t5, 8
	j 1b
2:
	tail kmain
