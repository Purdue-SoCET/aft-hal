#include "hal/hal.h"

using HAL::hal_init;
using HAL::GPIO;
using HAL::IntMgr;

void __attribute__((constructor)) message() {
    print("It's a secret to everybody\n");
}

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
    print("CLINT block is ");
    putinthex((uint32_t)intmgr->clint_reg_blk);
    intmgr->trig_sw_int();

    return 0;
}
