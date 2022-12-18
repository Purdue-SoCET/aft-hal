#include "hal.h"
#include "stdlib.h"

void __attribute__((constructor)) message() {
    print("hal: constructing peripherals\n");
}

void m_sw_handler() {
    print("this is the user implementation of the software handler\n");
    HAL::IntMgr::get()->clr_sw_int();
}

void do_assert(bool condition, const char *msg) {
    if(!condition) {
        print("Assertion failed: ");
        print(msg);
        print("\n");
        for(;;);

        __builtin_unreachable();
    }
}

int main() {
    HAL::GPIO* gpioa = nullptr;
    HAL::IntMgr* intmgr = nullptr;

    HAL::hal_init( );

    void *ptr_arr[1024 / 64]; // Hold 1K / 64 pointers
    for(auto i = 0; i < (1024 / 64); i++) {
        ptr_arr[i] = malloc(48);
        do_assert((ptr_arr[i] != nullptr), "malloc returned NULL!"); 
        print("Iteration ");
        putinthex(i);
        print(" successful (");
        putinthex((uint32_t)ptr_arr[i]);
        print(")\n");
    }

    void *p = malloc(64);
    do_assert((p == nullptr), "malloc returned garbage instead of NULL!");

    free(ptr_arr[0]);
    p = malloc(48);
    do_assert((p != nullptr), "free was apparently unsuccessful!");

    print("Test passed!\n");

    return 0;
}
