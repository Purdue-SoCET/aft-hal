#include "hal.h"

uint32_t HAL::get_err() {
    return err_flg;
}

void HAL::hal_init() {
    // Starts interrupt manager
    IntMgr::get();
}
