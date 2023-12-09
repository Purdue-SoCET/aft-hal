#include "hal.h"

using HAL::DMA;

static DMA dma;

bool DMA::dma_init = false;

DMA::DMA(){

}

DMA* DMA::dma_setup(){
    if (dma_init == true){
        print("hal: DMA have been constructed\n");
        return nullptr;
    }
    print("hal: setting up dma\n");
    dma_init = true;
    dma.dma_reg_blk = ((DMARegBlk*) DMA_BASE);
    return &dma;
}


void DMA::dma_config(dmaControlConfig config){
    print("Configuring CRTL Reg\n");
    dma_reg_blk->cr &= ~0x0000FFFF;
    dma_reg_blk->cr = (config.TCIE <<1) | (config.HTIE <<2) | (config.SRC <<4) | (config.DST <<5) 
                    | (config.PSIZE <<6) | (config.trig_en <<8) | (config.circular <<10) 
                    | (config.IDST <<11) | (config.ISRC <<12);
}
void DMA::set_source(uint32_t src_addr){
    dma_reg_blk->sar = (uint32_t) src_addr;
}
void DMA::set_dest(uint32_t dest_addr){
    dma_reg_blk->dar = (uint32_t) dest_addr;
}
void DMA::set_size(uint32_t bytes){
    dma_reg_blk->tsr &= ~ 0x0000FFFF;
    dma_reg_blk->tsr |= (uint32_t) bytes;
}
void DMA::startTxf(){
    dma_reg_blk->cr |= 0x1;
}
void DMA::stopTxf(){
    dma_reg_blk->cr &= ~0x1;
}
bool DMA::checkeErr(){
    if (dma_reg_blk->sr == 0){
        return false;
    }
    else{
        return true;
    }
}
