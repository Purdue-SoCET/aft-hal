#include "hal.h"

using HAL::PWM;
//./obj_dir/VbASIC_wrapper meminit.bin
// Declarations of static pwm objects
static PWM pwm;


bool PWM::pwm_init = false;

PWM::PWM() {
    // initialize PWM register block pointer   
}
//test func with direct initializing PWM channels
PWM* PWM::setup(){   //static 
    if (pwm_init == true){
        print("hal: PWM channels have been constructed\n");
        return nullptr;
    }
    print("hal: setting up pwm\n");
    pwm_init = true;
    pwm.pwm_reg_blk = ((PWMRegBlk*) PWM_BASE) ;
    return &pwm;
}



void PWM::setPeriod(int channel_num,uint32_t value) {
    
    if (channel_num == 0){
        print("PWM: setting up pwm channel 0 period\n");
        pwm_reg_blk->per0 = value;
        }
    else if (channel_num == 1){
        print("PWM: setting up pwm channel 1 period\n");
        pwm_reg_blk->per1 = value;
    }
    else{
        print("Invalid Channel Number, (0 to 1 only)\n");
    }
}

uint32_t PWM::getPeriod(int channel_num){
    if (channel_num == 0){
        return pwm_reg_blk->per0;
    }
    else if (channel_num == 1){
        return pwm_reg_blk->per1;
    }
    else{
        print("Invalid Channel Number, (0 to 1 only)\n");
    }
}

void PWM::setDuty(int channel_num,uint32_t value) {
    if (channel_num == 0){
        print("PWM: setting up pwm channel 0 Duty\n");
        pwm_reg_blk->duty0 = value;
    }
    else if (channel_num == 1){
        print("PWM: setting up pwm channel 1 Duty\n");
        pwm_reg_blk->duty0 = value;
    }
    else{
        print("Invalid Channel Number, (0 to 1 only)\n");
    }
}
uint32_t PWM::getDuty(int channel_num){
    if (channel_num == 0){
        return pwm_reg_blk->duty0;
    }
    else if (channel_num == 1){
        return pwm_reg_blk->duty1;
    }
    else{
        print("Invalid Channel Number, (0 to 1 only)\n");
    }
}
void PWM::setControlRegister(int channel_num,uint8_t enable, uint8_t polarity, uint8_t alignment) {
    print("Setting Control reg \n");
    if (channel_num == 0){
        pwm_reg_blk->ctrl0 = (enable << 0) | (polarity << 1) | (alignment << 2);
    }
    else if (channel_num == 1){
        pwm_reg_blk->ctrl1 = (enable << 0) | (polarity << 1) | (alignment << 2);
    }
    else{
        print("Invalid Channel Number, (0 to 1 only)\n");
    }
}
