#ifndef PALH_H_
#define PALH_H_

#include <stdint.h>

// IO definitions
#define __I  volatile const // Defines 'read-only' permissions
#define __O  volatile       // Defines 'write-only' permissions
#define __IO volatile       // Defines 'read/write' permissions

// Possible logic levels settings
enum LogicLevel {
    Low,
    High
};

// Peripheral memory map
#define BOTTOMRAM_BASE      ((uint32_t)0x00000000)
#define SELFTEST_BASE       ((uint32_t)0x00000200)
#define TOPRAM_BASE         ((uint32_t)0x00008000)
#define SRAM_BASE           ((uint32_t)0x00008400)
#define GPIO_BASE           ((uint32_t)0x80000000)
#define CLINT_BASE          ((uint32_t)0xE0000000)
#define PLIC_BASE           ((uint32_t)0xE0010000)

// GPIO register block
typedef struct {
    uint32_t reserved0;
    __IO uint32_t data;
    __IO uint32_t ddr;
    __IO uint32_t ier;
    __IO uint32_t per;
    __IO uint32_t ner;
    __IO uint32_t icr;
    __IO uint32_t isr;
} GPIORegBlk;

// Available GPIOs
enum GPIOPort {
    A = GPIO_BASE
};

// Availale GPIO pins
enum GPIOPin {
    Pin0 = 1 << 0,
    Pin1 = 1 << 1,
    Pin2 = 1 << 2,
    Pin3 = 1 << 3,
    Pin4 = 1 << 4,
    Pin5 = 1 << 5,
    Pin6 = 1 << 6,
    Pin7 = 1 << 7
};

// IRQ mappings
enum IRQMap {
    Dflt  = 0,
    Exc   = 1,
    MSoft = 2,
    MTim  = 3,
    MExt  = 4
};

// CLINT register block
typedef struct {
    __IO uint32_t msip;
    __IO uint64_t mtime;
    __IO uint64_t mtimecmp;
} CLINTRegBlk;

#define N_INTS 32

// PLIC register block
typedef struct {
    uint32_t reserved0;
    __IO uint32_t iprior[N_INTS];
    __IO uint32_t ipndgr;
    __IO uint32_t ier;
    uint32_t reserved1;
    __IO uint32_t ptr;
    __IO uint32_t ccr;
} PLICRegBlk;
#endif /* PAL_H_ */