#include "hal.h"

using HAL::get_err;
using HAL::hal_init;
using HAL::IntMgr;

uint32_t HAL::get_err() {
    return err_flg;
}

void HAL::hal_init() {
    // Starts interrupt manager
    IntMgr::get();
}