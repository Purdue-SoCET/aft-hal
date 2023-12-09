#include "pal.h"

#include "hal.h"

HAL::TMR* timer = nullptr;

IOMuxRegBlk *iomux = (IOMuxRegBlk *) IO_MUX_BASE;
CLINTRegBlk *clint = (CLINTRegBlk *) CLINT_BASE;
PLICRegBlk *plic = (PLICRegBlk *) PLIC_BASE;

uint32_t interrupted = 0;
void __attribute__((interrupt)) mext_handler() {
    uint32_t cause = plic->ccr;
    // timer->tcr &= ~TIM_TCR_EN;
    timer->tmr_disable();
    print("Interrupted!\n");
    // print("Cause: %d\n", cause);
    interrupted = 1;
    plic->ccr = cause;  // claim interrupt
}
void interrupt_enable() {
    uint32_t mie_bit = 0x8;
    asm volatile(
        "csrs mstatus, %0"
        : : "r"(mie_bit));
}

int main() {
    // enable interrupt for CC channel 5
    plic->ier1 = 0xFFFFFFFF;
    plic->iprior[44] = 0x7;

    clint->mtimecmp = 0xFFFFFFFF;
    interrupt_enable();

    // Configure IO mux for timer CC output
    iomux->fsel1 = IOM_F1_TIM0_CC0 | IOM_F1_TIM0_CC1 | IOM_F1_TIM0_CC2 |
                   IOM_F1_TIM0_CC3 | IOM_F1_TIM0_CC4 | IOM_F1_TIM0_CC5;

    // Configure timer
    // TimerRegBlk *timer = (TimerRegBlk *) TIMER_BASE;
    // timer->tpsc = 0x100;
    // timer->tarr = 0xffff;
    timer = timer->get_tmr();
    timer->tmr_config(0x100, 0xffff);
    // Configure CC channels 0-5

    //timer 0
    // timer->tccr[0] = 0x1000;
    timer->set_tccr(0,0x1000);
    HAL::TMR::TimerChannelControlReg channelControl;
    channelControl.enable = 1;
    channelControl.outputMode = 0;
    channelControl.inputSource = 2;
    channelControl.edge = 0;
    channelControl.inputMode = 0;
    channelControl.interruptEnable = 0;
    // timer->tccmr[0] = TIM_TCCMR_EN | TIM_TCCMR_OUTPUT | TIM_TCCMR_MATCH_HI;
    timer->channel_config(0, channelControl);
    
    //timer 1
    //timer->tccr[1] = 0x2000;
    timer->set_tccr(1,0x2000);
    channelControl.enable = 1;
    channelControl.outputMode = 0;
    channelControl.inputSource = 3;
    channelControl.edge = 0;
    channelControl.inputMode = 0;
    channelControl.interruptEnable = 0;
    // timer->tccmr[1] = TIM_TCCMR_EN | TIM_TCCMR_OUTPUT | TIM_TCCMR_TOGGLE;
    timer->channel_config(1, channelControl);

    //timer 2
    //timer->tccr[2] = 0x4000;
    timer->set_tccr(2,0x4000);
    channelControl.enable = 1;
    channelControl.outputMode = 0;
    channelControl.inputSource = 1;
    channelControl.edge = 0;
    channelControl.inputMode = 2;
    channelControl.interruptEnable = 0;
    // timer->tccmr[2] = TIM_TCCMR_EN | TIM_TCCMR_OUTPUT | TIM_TCCMR_FORCE_HI;
    timer->channel_config(2, channelControl);
    
    

    //timer 3
    //timer->tccr[3] = 0x8000;
    timer->set_tccr(3,0x4000);
    channelControl.enable = 1;
    channelControl.outputMode = 0;
    channelControl.inputSource = 2;
    channelControl.edge = 0;
    channelControl.inputMode = 1;
    channelControl.interruptEnable = 0;
    // timer->tccmr[3] = TIM_TCCMR_EN | TIM_TCCMR_OUTPUT | TIM_TCCMR_PWM1;
    timer->channel_config(3, channelControl);
    
    //timer 4
    //timer->tccr[4] = 0x4000;
    timer->set_tccr(4,0x4000);
    channelControl.enable = 1;
    channelControl.outputMode = 0;
    channelControl.inputSource = 3;
    channelControl.edge = 0;
    channelControl.inputMode = 1;
    channelControl.interruptEnable = 0;
    // timer->tccmr[4] = TIM_TCCMR_EN | TIM_TCCMR_OUTPUT | TIM_TCCMR_PWM1;
    timer->channel_config(4, channelControl);

    //timer 5
    //timer->tccr[5] = 0x2000;
    timer->set_tccr(5,0x2000);
    channelControl.enable = 1;
    channelControl.outputMode = 1;
    channelControl.inputSource = 0;
    channelControl.edge = 0;
    channelControl.inputMode = 0;
    channelControl.interruptEnable = 1;
    // timer->tccmr[5] = TIM_TCCMR_EN | TIM_TCCMR_RISING | TIM_TCCMR_TI1_IN | TIM_TCCMR_IRQ_EN;
    timer->channel_config(5, channelControl);

    // timer->tcr = TIM_TCR_EN;
    timer->tmr_enable();
    while (!interrupted);

    return 0;
}
