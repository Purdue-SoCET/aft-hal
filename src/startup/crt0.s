.global _start

_start:

_clear_bss:
    la x1, _sbss
    la x2, _ebss
    li x3, 0

_clear_bss_loop:
    sb x3, 0(x1)
    addi x1, x1, 1
    blt x1, x2, _clear_bss_loop

_clear_bss_done:
    la sp, __stack_top
    la gp, __global_pointer$

_goto_main:
    jal main

_infinite_loop:
    j _infinite_loop
