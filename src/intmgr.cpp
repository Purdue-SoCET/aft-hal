#include "hal.h"

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
		for (int i = 0; i < N_INTS; i++) {
			INTMGR.user_extISR[i] = NULL;
		}
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


// Enter and exit critical sections (interrupts turned OFF)
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


/**
 *
 *  SOFTWARE INTERRUPTS
 *
 */

void IntMgr::enable_sw_interrupts() {
    uint64_t mie_val;
    asm volatile("csrr %0, mie" : "=r" (mie_val));
    mie_val |= (1 << 3);
    asm volatile("csrw mie, %0" : : "r" (mie_val));
}


void IntMgr::disable_sw_interrupts() {
    uint64_t mie_val;
    asm volatile("csrr %0, mie" : "=r" (mie_val));
    mie_val &= ~(1 << 3);
    asm volatile("csrw mie, %0" : : "r" (mie_val));
}


void IntMgr::trig_sw_int() {
    clint_reg_blk->msip = 0x1;
}


void IntMgr::clr_sw_int() {
    clint_reg_blk->msip = 0x0;
}


void IntMgr::set_swISR(void (*handler)()) {
   user_swISR = handler;
}


void (*IntMgr::get_swISR())() {
   return user_swISR;
}


void __attribute((interrupt)) __attribute((weak)) m_sw_handler() {
   void (*user_swISR)() = INTMGR.get_swISR();
   if (user_swISR != NULL) {
      (user_swISR)();
   }

   INTMGR.clr_sw_int();    
}


/**
 *
 *  TIMER INTERRUPTS
 *
 */

void IntMgr::enable_tm_interrupts() {
    uint64_t mie_val;
    asm volatile("csrr %0, mie" : "=r" (mie_val));
    mie_val |= (1 << 7);
    asm volatile("csrw mie, %0" : : "r" (mie_val));
}


void IntMgr::disable_tm_interrupts() {
    uint64_t mie_val;
    asm volatile("csrr %0, mie" : "=r" (mie_val));
    mie_val &= ~(1 << 7); 
    asm volatile("csrw mie, %0" : : "r" (mie_val));
}


void IntMgr::set_timerISR(void (*handler)(uint32_t), uint32_t interval) {
   user_timerISR = handler;
   user_mtimecmp_interval = interval;
}


void (*IntMgr::get_timerISR(uint32_t *increment))(uint32_t) {
   *increment = user_mtimecmp_interval;
   return user_timerISR;
}


void __attribute((interrupt)) __attribute((weak)) m_timer_handler() {
   uint32_t user_mtimecmp_interval;
   void (*user_timerISR)(uint32_t) = INTMGR.get_timerISR(&user_mtimecmp_interval);

   // increment the mtimecmp register forward
   if (user_mtimecmp_interval > 0) {
      INTMGR.increment_mtimecmp(user_mtimecmp_interval);
   } else {
      INTMGR.increment_mtimecmp(0xFFFF);
      INTMGR.disable_tm_interrupts();
   }

   if (user_timerISR != NULL) {
      (*user_timerISR)(INTMGR.read_mtime());
   }
}


uint32_t IntMgr::read_mtime() {
    return clint_reg_blk->mtime;
}


uint32_t IntMgr::read_mtimecmp() {
    return clint_reg_blk->mtimecmp;
}


void IntMgr::write_mtimecmp(uint32_t value) {
    clint_reg_blk->mtimecmp = (uint32_t) value; 
}


void IntMgr::increment_mtimecmp(uint32_t increment) {
    clint_reg_blk->mtimecmp = increment + clint_reg_blk->mtimecmp;
}


void IntMgr::increment_mtimecmp_past_mtime(uint32_t increment) {
    clint_reg_blk->mtimecmp = increment + clint_reg_blk->mtime;
}


/**
 *
 *  EXTERNAL (PLIC) INTERRUPTS
 *
 */

void IntMgr::enable_ext_interrupts() {
	uint64_t mie_val;
	asm volatile("csrr %0, mie" : "=r" (mie_val));
	mie_val |= (1 << 11);
	asm volatile("csrw mie, %0" : : "r" (mie_val));
}


void IntMgr::disable_ext_interrupts() {
	uint64_t mie_val;
	asm volatile("csrr %0, mie" : "=r" (mie_val));
	mie_val &= ~(1 << 11);
	asm volatile("csrw mie, %0" : : "r" (mie_val));
}


void IntMgr::set_extISR(void (*handler)(uint32_t), uint32_t int_id) {
   user_extISR[int_id] = handler;
}


void (*IntMgr::get_extISR(uint32_t int_id))(uint32_t) {
   return user_extISR[int_id];
}


void __attribute((interrupt)) __attribute((weak)) m_ext_handler() {
   uint32_t int_id = INTMGR.claim_extISR();
   void (*user_extISR)(uint32_t) = INTMGR.get_extISR(int_id);
   if (user_extISR != NULL) {
      (*user_extISR)(int_id); 
   }

   INTMGR.complete_extISR(int_id);
}


uint32_t IntMgr::claim_extISR() {
   return (uint32_t) plic_reg_blk->ccr;
}


void IntMgr::complete_extISR(uint32_t id) {
  plic_reg_blk->ccr = id;
}


void IntMgr::enable_extISR(uint8_t select,uint32_t int_id) {
    if (select == 0){
        plic_reg_blk->ier0 |= 1 << int_id;
    }
    else {
        plic_reg_blk->ier1 |= 1 << int_id;
    }
}

void IntMgr::disable_extISR(uint8_t select,uint32_t int_id) {
    if (select == 0){
        plic_reg_blk->ier0 &= ~(1 << int_id);
    }
    else {
        plic_reg_blk->ier1 &= ~(1 << int_id);
    }
}


void IntMgr::set_priority_extISR(uint32_t priority, uint32_t int_id) {
   plic_reg_blk->iprior[int_id-1] = priority;
}


void IntMgr::set_threshold_extISR(uint32_t priority_threshold) {
	plic_reg_blk->ptr = priority_threshold;
}
