#include "hal.h"
using HAL::TMR;

static TMR tmr;
bool TMR::tmr_init = false;

TMR::TMR(){}

TMR* TMR::get_tmr(){
    if (tmr_init == true){
        print("hal: TIMER already constructed, "); 
        print("returning nullptr\n");
        return nullptr;
    }
    print("hal: constructing timer\n");
    tmr_init = true;
    tmr.tmr_reg_blk = ((TimerRegBlk*) TIMER_BASE);
    return &tmr;
}

void TMR::tmr_enable(){
    tmr_reg_blk->tcr |= TIM_TCR_EN;
}
void TMR::tmr_disable(){
    tmr_reg_blk->tcr &= ~TIM_TCR_EN;
}

uint32_t TMR::get_tmr_count(){
    return (uint32_t)tmr_reg_blk->tcnt;
}
void TMR::tmr_config(uint32_t prescaler, uint32_t reloadval){
    if (prescaler > TIM_TPSC_MAX) {
        print("HAL:TIMER: Error, Prescaler Value larger than Maximum!");
        return;
    }
    if (reloadval > TIM_TARR_MAX) {
        print("HAL:TIMER: Error, Reload Value larger than Maximum!");
        return;
    }
    tmr_reg_blk->tpsc = (uint32_t)prescaler;
    tmr_reg_blk->tarr = (uint32_t)reloadval;
}
void TMR::channel_config(uint32_t channel, TimerChannelControlReg controlReg){
    tmr_reg_blk->tccmr[channel] =   (controlReg.enable << 0) |
                                    (controlReg.edge << 1) |
                                    (controlReg.outputMode << 3) |
                                    (controlReg.inputSource << 5) |
                                    (controlReg.inputMode << 7) |
                                    (controlReg.interruptEnable << 8);
}
void TMR::set_tccr(uint32_t channel, uint32_t tccr_val){
    tmr_reg_blk->tccr[channel] = (uint32_t)tccr_val;
}
