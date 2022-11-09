#include "hal/hal.h"

using HAL::hal_init;
using HAL::GPIO;
using HAL::IntMgr;

void m_sw_handler() {
    print("this is the user implementation of the software handler\n");
    IntMgr::get()->clr_sw_int();
}

int main() {
    GPIO* gpioa = nullptr;
    IntMgr* intmgr = nullptr;

    hal_init( );
    
    if (!(gpioa = GPIO::open(A))) {
        print("error: issue opening GPIO module for selected port\n");
    }

    intmgr = IntMgr::get();

    gpioa->dbg_blink();
    intmgr->trig_sw_int();

    return 0;
}