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

void GPIO::set_direct(GPIOPin pin, LogicLevel lvl){
    reg_blk->ddr &= ~((uint32_t) pin);

    if (lvl == LogicLevel::High) {
        reg_blk->ddr |= ((uint32_t) pin);
    }
}
void GPIO::set_intrpt(GPIOPin pin, LogicLevel lvl){
    reg_blk->ier &= ~((uint32_t) pin);

    if (lvl == LogicLevel::High) {
        reg_blk->ier |= ((uint32_t) pin);
    }
}
void GPIO::set_pos_edge(GPIOPin pin, LogicLevel lvl){
    reg_blk->per &= ~((uint32_t) pin);

    if (lvl == LogicLevel::High) {
        reg_blk->per |= ((uint32_t) pin);
    }
}
void GPIO::set_neg_edge(GPIOPin pin, LogicLevel lvl){
    reg_blk->ner &= ~((uint32_t) pin);

    if (lvl == LogicLevel::High) {
        reg_blk->ner |= ((uint32_t) pin);
    }
}
void GPIO::clear_intrpt(GPIOPin pin){
        reg_blk->icr |= ((uint32_t) pin);
}
bool GPIO::read_intrpt_status(GPIOPin pin) {
    return reg_blk->isr & ((uint32_t) pin);
}
