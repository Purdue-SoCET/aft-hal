#include "hal.h"

using HAL::hal_init;

void HAL::hal_init() {
    // Setup vectored interrupt mode
    uint32_t mtvec_val = ((uint32_t) vtable) | 0x1;
    uint32_t mstatus_val = 0x888;
    uint32_t mie_val = 0x8;

    asm volatile("csrw mtvec, %0" : : "r" (mtvec_val));
    asm volatile("csrw mstatus, %0" : : "r" (mstatus_val));
    asm volatile("csrw mie, %0" : : "r" (mie_val));
}