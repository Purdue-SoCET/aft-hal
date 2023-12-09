#include "pal.h"
#include "hal.h"
#include "format.h"
int main() {
    //GPIO Test

    HAL::GPIO* gpioa = nullptr;
    gpioa = gpioa->open(A);
    gpioa->enable_output(Pin0);
    gpioa->enable_output(Pin6);
    gpioa->set_output(Pin6, High);
    return 0;
}
