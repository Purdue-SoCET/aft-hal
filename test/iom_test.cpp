#include "pal.h"
#include "hal.h"
#include "format.h"
int main() {

    HAL::IOMux* iom = nullptr;
    iom = iom->IOMux_init();
    iom->set_config(8, IOM_F0_PWM0_0 | IOM_F0_PWM0_1);
    
    
    PWMRegBlk *pwm = (PWMRegBlk *) PWM_BASE;
    pwm->per0 = 0x10;
    pwm->duty0 = 0x9; // 50% duty cycle (PWM duty off by 1)

    pwm->per1 = 0x20;
    pwm->duty1 = 0x2; // 1/32 duty cycle

    
    pwm->ctrl0 = 0x5; // center-aligned, active high, enabled
    pwm->ctrl1 = 0x3; // left-aligned, active low, enabled
    return 0;
}
