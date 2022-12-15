#include "../hal/hal.h"

using HAL::IntMgr;

// Declaration of static interrupt manager object
static IntMgr INTMGR;

// Initializes interrupt manager initialization field
bool IntMgr::intmgr_init = false;

// Default constructor
IntMgr::IntMgr() : mstatus_last(0), clint_reg_blk((CLINTRegBlk *) CLINT_BASE), 
    plic_reg_blk((PLICRegBlk *) PLIC_BASE) {}

// Constructs/gets interrupt manager
IntMgr* IntMgr::get() {
    if (!intmgr_init) {
        print("hal: starting interrupt manager\n");
        INTMGR.setup_vtable();
    }

    intmgr_init = true;
    return &INTMGR;
}

void IntMgr::setup_vtable() {
    // Sets base addres to vtable and mode to vectored
    uint32_t mtvec_val = ((uint32_t) vtable) | 0x1;

    // Sets MIE, MPIE, and MPP
    uint32_t mstatus_val = 0x888;

    // Sets interrupt-enable bit for software-level interrupts
    uint32_t mie_val = 0x8;

    // Write values to respective registers
    asm volatile("csrw mtvec, %0" : : "r" (mtvec_val));
    asm volatile("csrw mie, %0" : : "r" (mie_val));

    // Activate interrupts
    asm volatile("csrw mstatus, %0" : : "r" (mstatus_val));
    print("intmgr: vectored interrupts active\n");
}

void IntMgr::trig_sw_int() {
    clint_reg_blk->msip = 0x1;
}

void IntMgr::clr_sw_int() {
    clint_reg_blk->msip = 0x0;
}

void IntMgr::entr_critsec() {
    print("intmgr: entering critical section\n");
    asm volatile("csrr %0, mstatus" : "=r" (mstatus_last));
    uint32_t mstatus_next = mstatus_last & ~(0x8);
    asm volatile("csrw mstatus, %0" : : "r" (mstatus_next));
}

void IntMgr::exit_critsec() {
    asm volatile("csrw mstatus, %0" : : "r" (mstatus_last));
    print("intmgr: critical section exited\n");
}

void IntMgr::reg_int_cb(void (*cb)(), IRQMap irq) {

}
