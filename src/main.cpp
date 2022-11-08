#include "hal/hal.h"

using HAL::hal_init;
using HAL::GPIO;

void m_sw_handler( ) {
    print("this is the user implementation of the software handler\n");
    uint32_t* addr = (uint32_t*) 0xE0000000;
    *addr = 0x0;
}

int main() {
    GPIO* gpioa = nullptr;
    GPIO* gpioa_cpy = nullptr;

    hal_init( );
    
    if (!(gpioa = GPIO::open(A))) {
        print("error: issue opening GPIO module for selected port\n");
    }

    if (!(gpioa_cpy = GPIO::open(A))) {
        print("error: issue opening GPIO module for selected port\n");
    }

    gpioa->dbg_blink( );

    uint32_t* addr = (uint32_t*) 0xE0000000;
    *addr = 0x1;

    return 0;
}