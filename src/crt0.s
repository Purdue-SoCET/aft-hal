.global _start
_start:

_clear_bss:
    la x1, __bss_start
    la x2, __bss_end
    li x3, 0

_clear_bss_loop:
    sb x3, 0(x1)
    addi x1, x1, 1
    blt x1, x2, _clear_bss_loop

_clear_bss_done:
    la sp, __stack_top
    la gp, __global_pointer$

_handle_init:
    la x3, __init_array_start
    la x4, __init_array_end
    beq x3, x4, _goto_main
    addi sp, sp, -8

_init_loop:
    sw x3, 0(sp)
    sw x4, 4(sp)
    jal x3
    lw x3, 0(sp)
    lw x4, 4(sp)
    addi x3, x3, 4
    bne x3, x4, _init_loop

_init_loop_done:
    addi sp, sp, 8
#_call_init_array:
#    la x18, __init_array_start
#    la x19, __init_array_end
#    beq x18, x19, _goto_main

#_call_init_array_loop:
#    jal x18 # __init_array[i]()
#    addi x18, x18, 4
#    bne x18, x19, _call_init_array_loop

_goto_main:
    jal main

_infinite_loop:
    j _infinite_loop
