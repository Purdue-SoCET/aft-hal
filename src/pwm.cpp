#include "hal.h"

using HAL::PWM;
// Declarations of static pwm objects
static PWM pwm;

// Initializes GPIO initialization field
bool PWM::pwm_init = false;

PWM::PWM() {
    // initialize PWM register block pointer   
}
//test func with direct initializing PWM channels
PWM* PWM::setup(){   //static 
    if (pwm_init = false){
        print("hal: PWM channels have been constructed, ");
        return nullptr;
    }
    print("hal: setting up pwm\n");
    pwm_init = true;
    pwm.pwm_reg_blk = ((PWMRegBlk*) PWM_BASE) ;
    return &pwm;
}
//test func with direct access to reg.
void PWM::setP(uint32_t value) {

    print("Set period reg\n");
    pwm_reg_blk->per0 =  0x1;
}
//test func with direct access to reg.
void PWM::enableChanl() {

    print("Enable ctrl reg\n");
    pwm_reg_blk->ctrl0 |= (uint32_t) 0xffff;
}

//************************************************************
PWM* PWM::setupChannels(){
    if (pwm_init){
        print("hal: PWM channels have been constructed, ");
        print("returning nullptr\n");
        return nullptr;
    }
    print("hal: setting up pwm channels\n");
    // initialize PWM channels
    for (auto i = 0; i < pwm.num_of_channle; i++) {  //  2 -> number of the channel
        switch (i) {
            case 0:
                pwm.channels[i].per = &(pwm.pwm_reg_blk->per0);
                pwm.channels[i].duty = &pwm.pwm_reg_blk->duty0;
                pwm.channels[i].ctrl = &pwm.pwm_reg_blk->ctrl0;
                break;
            case 1:
                pwm.channels[i].per = &pwm.pwm_reg_blk->per1;
                pwm.channels[i].duty = &pwm.pwm_reg_blk->duty1;
                pwm.channels[i].ctrl = &pwm.pwm_reg_blk->ctrl1;
                break;
            case 2:
                pwm.channels[i].per = &pwm.pwm_reg_blk->per2;
                pwm.channels[i].duty = &pwm.pwm_reg_blk->duty2;
                pwm.channels[i].ctrl = &pwm.pwm_reg_blk->ctrl2;
                break;
            case 3:
                pwm.channels[i].per = &pwm.pwm_reg_blk->per3;
                pwm.channels[i].duty = &pwm.pwm_reg_blk->duty3;
                pwm.channels[i].ctrl = &pwm.pwm_reg_blk->ctrl3;
                break;
            case 4:
                pwm.channels[i].per = &pwm.pwm_reg_blk->per4;
                pwm.channels[i].duty = &pwm.pwm_reg_blk->duty4;
                pwm.channels[i].ctrl = &pwm.pwm_reg_blk->ctrl4;
                break;
            case 5:
                pwm.channels[i].per = &pwm.pwm_reg_blk->per5;
                pwm.channels[i].duty = &pwm.pwm_reg_blk->duty5;
                pwm.channels[i].ctrl = &pwm.pwm_reg_blk->ctrl5;
                break;
            case 6:
                pwm.channels[i].per = &pwm.pwm_reg_blk->per6;
                pwm.channels[i].duty = &pwm.pwm_reg_blk->duty6;
                pwm.channels[i].ctrl = &pwm.pwm_reg_blk->ctrl6;
                break;
            case 7:
                pwm.channels[i].per = &pwm.pwm_reg_blk->per7;
                pwm.channels[i].duty = &pwm.pwm_reg_blk->duty7;
                pwm.channels[i].ctrl = &pwm.pwm_reg_blk->ctrl7;
                break;
            default:
                pwm.channels[i].per = nullptr;
                pwm.channels[i].duty = nullptr;
                pwm.channels[i].ctrl = nullptr;
                break;
        }
    }
    pwm_init = true;
    return &pwm;
}

PWM::Channel::Channel() : per(nullptr), duty(nullptr), ctrl(nullptr) {}

void PWM::Channel::setPeriod(uint32_t value) {
    if (per != nullptr) {
        print("Setting period reg\n");
        *per = value;
        curr_per = value;
        print("\n");
        putinthex(*per);
    }
    else{
        print("Error NULL period reg\n");
    }
}

uint32_t PWM::Channel:: getPeriod(){
    return *per;
}

void PWM::Channel::setDuty(float percentage) { //should add limit for maximum period value
    if (duty != nullptr && per != nullptr) {
        print("Setting duty reg \n");
        // calculate duty value based on percentage of period
        uint32_t duty_val = (uint32_t)((percentage / 100.0) * (*per));
        
        // set duty register
        *duty = duty_val;
        
        // update current duty value
        curr_duty = duty_val;
    }
    else{
        print("Error NULL Duty reg \n");
    }
}
uint32_t PWM::Channel:: getDuty(){
    return *duty;
}
void PWM::Channel::setControlRegister(uint8_t enable, uint8_t polarity, uint8_t alignment) {
    if (ctrl != nullptr) {
         print("Setting Control reg \n");
        *ctrl = (enable << 0) | (polarity << 1) | (alignment << 2);
    }
    else{
        print("Error NULL Control reg \n");
    }
}



/* 
//direct access APIs
// Declaration of static PWM object
static PWM pwm;

// Initializes PWM initialization field
bool PWM::pwm_init[] = {false,false,false,false,false,false,false,false};

// Default constructor
PWM::PWM(): pwm_reg_blk((PWMRegBlk *) PWM_BASE){}

PWM* PWM::pwm_channel(int channel_num){
    if (!pwm_init[channel_num]) {
        print("hal: opening pwm channel\n");
        pwm_init[channel_num] = true;
        switch (channel_num) {
                    case 0:
                        per = &pwm_reg_blk->per0;
                        duty = &pwm_reg_blk->duty0;
                        ctrl = &pwm_reg_blk->ctrl0;
                        break;
                    case 1:
                        per = &pwm_reg_blk->per1;
                        duty = &pwm_reg_blk->duty1;
                        ctrl = &pwm_reg_blk->ctrl1;
                        break;
                    case 2:
                        per = &pwm_reg_blk->per2;
                        duty = &pwm_reg_blk->duty2;
                        ctrl = &pwm_reg_blk->ctrl2;
                        break;
                    case 3:
                        per = &pwm_reg_blk->per3;
                        duty = &pwm_reg_blk->duty3;
                        ctrl = &pwm_reg_blk->ctrl3;
                        break;
                    case 4:
                        per = &pwm_reg_blk->per4;
                        duty = &pwm_reg_blk->duty4;
                        ctrl = &pwm_reg_blk->ctrl4;
                        break;
                    case 5:
                        per = &pwm_reg_blk->per5;
                        duty = &pwm_reg_blk->duty5;
                        ctrl = &pwm_reg_blk->ctrl5;
                        break;
                    case 6:
                        per = &pwm_reg_blk->per6;
                        duty = &pwm_reg_blk->duty6;
                        ctrl = &pwm_reg_blk->ctrl6;
                        break;
                    case 7:
                        per = &pwm_reg_blk->per7;
                        duty = &pwm_reg_blk->duty7;
                        ctrl = &pwm_reg_blk->ctrl7;
                        break;
                    default:
                        
                        per = nullptr;
                        duty = nullptr;
                        ctrl = nullptr;
                        break;
                }

        return &pwm;
    }
    else{
        print("hal: this pwm channel already constructed\n");
        return nullptr;
    }
}

void PWM::set_enable(int channel_num) {
    switch(channel_num){
        case 0:{
            pwm_reg_blk->ctrl0 |= 0x1;
        }
        case 1:{
            pwm_reg_blk->ctrl1 |= 0x1;
        }
        case 2:{
            pwm_reg_blk->ctrl2 |= 0x1;
        }
        case 3:{
            pwm_reg_blk->ctrl3 |= 0x1;
        }
        case 4:{
            pwm_reg_blk->ctrl4 |= 0x1;
        }
        case 5:{
            pwm_reg_blk->ctrl5 |= 0x1;
        }
        case 6:{
            pwm_reg_blk->ctrl6 |= 0x1;
        }
        case 7:{
            pwm_reg_blk->ctrl7 |= 0x1;
        }
    }
}

void PWM::set_polarity(int channel_num , LogicLevel lvl) {
    uint32_t polrt = 0;
    if (lvl == LogicLevel::High){
        polrt = 0x2;
    }
    switch(channel_num){
        case 0:{
            pwm_reg_blk->ctrl0 &= ~0x2;
            pwm_reg_blk->ctrl0 |= polrt;
        }
        case 1:{
            pwm_reg_blk->ctrl1 &= ~0x2;
            pwm_reg_blk->ctrl1 |= polrt;
        }
        case 2:{
            pwm_reg_blk->ctrl2 &= ~0x2;
            pwm_reg_blk->ctrl2 |= polrt;
        }
        case 3:{
            pwm_reg_blk->ctrl3 &= ~0x2;
            pwm_reg_blk->ctrl3 |= polrt;
        }
        case 4:{
            pwm_reg_blk->ctrl4 &= ~0x2;
            pwm_reg_blk->ctrl4 |= polrt;
        }
        case 5:{
            pwm_reg_blk->ctrl5 &= ~0x2;
            pwm_reg_blk->ctrl5 |= polrt;
        }
        case 6:{
            pwm_reg_blk->ctrl6 &= ~0x2;
            pwm_reg_blk->ctrl6 |= polrt;
        }
        case 7:{
            pwm_reg_blk->ctrl7 &= ~0x2;
            pwm_reg_blk->ctrl7 |= polrt;
        }
    }
}

void PWM::set_alignment(int channel_num , LogicLevel lvl) {
    uint32_t algnmt = 0;
    if (lvl == LogicLevel::High){
        algnmt = 0x4;
    }
    switch(channel_num){
        case 0:{
            pwm_reg_blk->ctrl0 &= ~0x4;
            pwm_reg_blk->ctrl0 |= algnmt;
        }
        case 1:{
            pwm_reg_blk->ctrl1 &= ~0x4;
            pwm_reg_blk->ctrl1 |= algnmt;
        }
        case 2:{
            pwm_reg_blk->ctrl2 &= ~0x4;
            pwm_reg_blk->ctrl2 |= algnmt;
        }
        case 3:{
            pwm_reg_blk->ctrl3 &= ~0x4;
            pwm_reg_blk->ctrl3 |= algnmt;
        }
        case 4:{
            pwm_reg_blk->ctrl4 &= ~0x4;
            pwm_reg_blk->ctrl4 |= algnmt;
        }
        case 5:{
            pwm_reg_blk->ctrl5 &= ~0x4;
            pwm_reg_blk->ctrl5 |= algnmt;
        }
        case 6:{
            pwm_reg_blk->ctrl6 &= ~0x4;
            pwm_reg_blk->ctrl6 |= algnmt;
        }
        case 7:{
            pwm_reg_blk->ctrl7 &= ~0x4;
            pwm_reg_blk->ctrl7 |= algnmt;
        }
    }
}
*/
