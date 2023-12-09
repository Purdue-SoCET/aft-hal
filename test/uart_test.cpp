#include "pal.h"
#include "hal.h"

#define BAUD_CYCLES 2604

UARTRegBlk *Uart = (UARTRegBlk *) UART_BASE;

int main() {
    HAL::UART* uart = nullptr;
    uart = uart->UART_init();
    uint32_t data = 0;
    
    // Uart->rxstate = (BAUD_CYCLES / 16) << 16;
    // uart->txstate = BAUD_CYCLES << 16;
    uart->set_Rx_BR(BAUD_CYCLES);
    uart->set_Tx_BR(BAUD_CYCLES);
    for (int i = 0; i < 40000 * 4; i++);  // wait for bytes to be sent to test RX buffering
    while (uart->checkRx_state());
    data = uart->get_RX_data();

    if ((uart->get_RX_NumByteRead()) != 3) {
        print("Incorrect number of bytes read\n");
        return 1;
    }
    if ((data & 0xffffff) != 0x6045f7) {
        print("Incorrect bytes received\n");
        return 1;
    }

    // uart->txdata = 0xaabb11 | 3 << 24;
    uart->send_byte(0xaabb11);
    while (uart->checkTx_state());

    if ((uart->get_TX_BR()) != BAUD_CYCLES) {
        print("Incorrect baud rate read\n");
        return 1;
    }

    return 0;
}
