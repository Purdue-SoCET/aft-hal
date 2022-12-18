#include "hal.h"

using HAL::GPIO;

// Declarations of static GPIO objects
static GPIO GPIOA;

// Initializes GPIO initialization field
bool GPIO::gpioa_init = false;

// Default constructor
GPIO::GPIO() {}

// Opens GPIO on defined ports
GPIO* GPIO::open(GPIOPort port) {
    switch (port) {
        case GPIOPort::A: {
            if (gpioa_init == true) {
                print("hal: GPIO already constructed using provided port, "); 
                print("returning nullptr\n");
                return nullptr;
            }

            print("hal: constructing GPIO on provided port\n");
            gpioa_init = true;
            GPIOA.reg_blk = ((GPIORegBlk*) port);
            return &GPIOA;
        }
        default: {
            print("hal: invalid GPIO port provided, returning nullptr\n");
            return nullptr;
        }
    }
}

void GPIO::enable_input(GPIOPin pin) {
    reg_blk->ddr &= ~((uint32_t) pin);
}

uint32_t GPIO::read_input(GPIOPin pin) {
    return reg_blk->data & ((uint32_t) pin);
}

void GPIO::enable_output(GPIOPin pin) {
    reg_blk->ddr |= ((uint32_t) pin);
}

void GPIO::set_output(GPIOPin pin, LogicLevel lvl) {
    reg_blk->data &= ~((uint32_t) pin);

    if (lvl == LogicLevel::High) {
        reg_blk->data |= ((uint32_t) pin);
    }
}
