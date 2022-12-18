#include <cstddef>
#include "stdlib.h"

void print(const char* x) {
    for(auto i = 0; x[i]; i++) {
        *(volatile char *)0x20000 = x[i];
    }
}

void putinthex(int x) {
    char buf[11];

    buf[0] = '0';
    buf[1] = 'x';
    for(auto i = 2; i < 10; i++) {
        buf[i] = '0';
    }

    for(auto i = 9; i >= 2; i--) {
        auto temp = x & 0xF;
        if(temp < 10) {
            buf[i] = temp + '0';
        } else {
            buf[i] = temp - 10 + 'A';
        }

        x >>= 4;
    }
    
    buf[10] = '\0';

    print(buf);
}
