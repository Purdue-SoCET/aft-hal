#include "pal.h"
#include "hal.h"
int main() {

    // DMARegBlk *dma0 = (DMARegBlk *) DMA_BASE;
    // dma0->cr |= 0x2;
    volatile int* src_address = (volatile int*) 0x8100;
    volatile int* second_byte = (volatile int*) 0x8104;
    volatile int* dest_address = (volatile int*) 0x8200;
    
    volatile int src_arr[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    volatile int dest_arr[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    
    src_address = src_arr; // 0x12345678;  
    dest_address = dest_arr;
    *second_byte = 0xDEADBEEF; //compare dest and src arr to check if send

    HAL::DMA* dma = nullptr;
    dma = dma->dma_setup();
    HAL::DMA::dmaControlConfig congfig;
    congfig.TCIE = 0x0;
    congfig.HTIE = 0x0;
    congfig.SRC = 0x0;
    congfig.DST = 0x0;
    congfig.PSIZE = 0x1;
    congfig.trig_en = 0x0;
    congfig.circular = 0x0;
    congfig.IDST = 0x1;
    congfig.ISRC = 0x1;

    dma->dma_config(congfig);
    dma->set_source(0x8100);
    dma->set_dest(0x8200);
    dma->set_size(0x3);
    dma->startTxf();

    volatile int i;
    volatile int j;
    for(i = 0; i < 100; i++);
    {
        j = i;
    }
    print("Hello, World!\n");
    print("Testing this.  \n");
    dma->checkeErr();
    volatile int k;

    for(k = 0; k < 8; k++){
        if (*dest_address+k == k) {
            print("Transfer Success.\n");
        }
        else {
            print("Transfer Failed.\n");
        }
    }

    return 0;
}
