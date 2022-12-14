#include <stdint.h>
#include "stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t __bss_start[];
extern uint8_t __bss_end[];
extern uint8_t __heap_start[];
extern uint32_t __stack_top;
extern uint32_t __global_pointer$;
extern void (*__preinit_array_start[])();
extern void (*__preinit_array_end[])();
extern void (*__init_array_start[])();
extern void (*__init_array_end[])();
extern int main();

void __attribute__((section(".startup"))) _start() {
    uint32_t count = 0;
    
    // Setup stack/global pointer
    asm volatile(".option push;"
                 ".option norelax;"
                 "la gp, __global_pointer$;"
                 ".option pop;"
                 "la sp, __stack_top;");

    // Zero out BSS
    for(uint8_t *p = __bss_start; p < __bss_end; p++) {
        *p = 0;
    }
    
    /*putinthex((uint32_t)__bss_start);
    putinthex((uint32_t)__bss_end);*/
    print("BSS: "); putinthex((uint32_t)__bss_start); print(" - "); putinthex((uint32_t)__bss_end); print("\n");
    print("Heap: "); putinthex((uint32_t)__heap_start); print("\n");
    print("Stack: "); putinthex((uint32_t)&__stack_top); print("\n");

    // Call init array
    count = __preinit_array_end - __preinit_array_start;
    print("preinit_array: "); putinthex((uint32_t)count); print(" items.\n");    
    for(int i = 0; i < count; i++) {
        __preinit_array_start[i]();
    }
   
    count = __init_array_end - __init_array_start;
    print("init_array: "); putinthex((uint32_t)count); print(" items.\n");    
    for(int i = 0; i < count; i++) {
        __init_array_start[i]();
    }
    
    // Call main
    main();

    // Loop forever
    for(;;);
}

#ifdef __cplusplus
}
#endif
