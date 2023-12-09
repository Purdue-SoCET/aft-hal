#include "hal.h"

using HAL::IOMux;
//./obj_dir/VbASIC_wrapper meminit.bin
// Declarations of static pwm objects


IOMux::IOMux(){

}
static IOMux iomux;
bool IOMux::iomux_init = false;
IOMux* IOMux::IOMux_init(){
    if (iomux_init == true){
        print("hal: IOMux have been constructed, ");
        return nullptr;
    }
    print("hal: setting up IOMux\n");
    iomux_init = true;
    
    iomux.iomux_reg_blk = ((IOMuxRegBlk*) IO_MUX_BASE) ;
    return &iomux;
}

void IOMux::set_config(int pin,uint32_t func_sel){
    if (pin < 16){
        iomux_reg_blk->fsel0 = func_sel;
    }
    else if (pin <32){
        iomux_reg_blk->fsel0 = func_sel;
    }
    else {
        print("hal: IOMUX: invalid pin number, 0 - 31 pins only.");
    }
}


void IOMux::clear_config(int pin,uint32_t func){
    if (pin < 16){
        iomux_reg_blk->fsel0 &= ~func;
    }
    else if (pin <32){
        iomux_reg_blk->fsel1 &= ~ func;
    }
    else {
        print("hal: IOMUX: invalid pin number, 0 - 31 pins only.");
    }
}
