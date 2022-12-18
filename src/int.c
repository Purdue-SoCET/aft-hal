#include "int.h"

#ifdef __cplusplus
extern "C" {
#endif

// Default handler (default implementation)
void __attribute__((weak)) __attribute__((interrupt)) default_handler(void) {
    print("hal: this is the default implementation of the default handler\n");
}

// Aliases default handler
void exception_handler( ) __attribute__((interrupt)) __attribute__((weak, alias("default_handler")));
void m_sw_handler( ) __attribute__((interrupt)) __attribute__((weak, alias("default_handler")));
void m_timer_handler( ) __attribute__((interrupt)) __attribute__((weak, alias("default_handler")));
void m_ext_handler( ) __attribute__((interrupt)) __attribute__((weak, alias("default_handler")));

// Sets up interrupt vector table
void __attribute((naked)) __attribute((aligned(4))) vtable( ) {
    asm volatile(".option push");
    asm volatile(".option norvc");
    asm volatile("j exception_handler");
    asm volatile("j default_handler");
    asm volatile("j default_handler");
    asm volatile("j m_sw_handler");
    asm volatile("j default_handler");
    asm volatile("j default_handler");
    asm volatile("j default_handler");
    asm volatile("j m_timer_handler");
    asm volatile("j default_handler");
    asm volatile("j default_handler");
    asm volatile("j default_handler");
    asm volatile("j m_ext_handler");
    asm volatile(".option pop");
}

#ifdef __cplusplus
}
#endif
