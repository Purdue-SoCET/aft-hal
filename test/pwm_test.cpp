#include "pal.h"
#include "hal.h"
#include "format.h"
int main() {
    

    HAL::PWM* pwm = nullptr;
    
    IOMuxRegBlk *iomux = (IOMuxRegBlk *) IO_MUX_BASE;
    iomux->fsel0 = IOM_F0_PWM0_0 | IOM_F0_PWM0_1;
    //PWM Test
    pwm = pwm->setup();
    pwm->setPeriod(0, 0x10);
    pwm->setDuty(0, 0x9);

    pwm->setPeriod(1, 0x20);
    pwm->setDuty(1, 0x2);

    pwm->setControlRegister(0, 1, 0, 1);// center-aligned, active high, enabled
    pwm->setControlRegister(1, 1, 1, 0);// left-aligned, active low, enabled


    // PWMRegBlk *pwm = (PWMRegBlk *) PWM_BASE;
    // pwm->per0 = 0x10;
    // pwm->duty0 = 0x9; // 50% duty cycle (PWM duty off by 1)

    // pwm->per1 = 0x20;
    // pwm->duty1 = 0x2; // 1/32 duty cycle

    
    // pwm->ctrl0 = 0x5; // center-aligned, active high, enabled
    // pwm->ctrl1 = 0x3; // left-aligned, active low, enabled

    return 0;
}
