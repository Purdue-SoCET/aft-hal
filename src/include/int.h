#ifndef INT_H_
#define INT_H_

#include "stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif
    extern void __attribute__((weak)) __attribute__((interrupt)) exception_handler(void);
    extern void __attribute__((weak)) __attribute__((interrupt)) default_handler(void);
    extern void __attribute__((weak)) __attribute__((interrupt)) m_sw_handler(void);
    extern void __attribute__((weak)) __attribute__((interrupt)) m_timer_handler(void);
    extern void __attribute__((weak)) __attribute__((interrupt)) m_ext_handler(void);
    void __attribute((naked)) __attribute((aligned(4))) vtable( );
#ifdef __cplusplus
}
#endif

#endif /* INT_H_ */
