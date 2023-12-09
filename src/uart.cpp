#include "hal.h"

using HAL::UART;

static UART uart;

bool UART::uart_init = false;
UART::UART(){

}
UART* UART::UART_init(){
    if (uart_init == true){
        print("hal: UART have been constructed, ");
        return nullptr;
    }
    print("hal: setting up UART\n");
    uart_init = true;
    
    uart.uart_reg_blk = ((UARTRegBlk*) UART_BASE) ;
    return &uart;
}
void UART::send_byte(uint8_t byte){
    //TX error check
    if (checkTx_state()) {
        print("UART Send (TX)\n");
        uart_reg_blk->txdata = byte | (3 << 24);
        return;
    }
    else {
        return;
    }
}
void UART::set_Rx_BR(uint32_t BAUD_CYCLES){
    uart_reg_blk->rxstate = (BAUD_CYCLES / 16) << 16;
}
void UART::set_Tx_BR(uint32_t BAUD_CYCLES){
    uart_reg_blk->txstate = BAUD_CYCLES << 16;
}
uint32_t UART::get_RX_data(){

    uint32_t data = 0;
    if (checkRx_state()){
        print("UART Data Recieved (RX)\n");
        data = uart_reg_blk->rxdata;
        return data;
    }
    else {
        print("UART Return Null (RX)\n");
        return data;
    }
}
bool UART::checkRx_state(){
    if(uart_reg_blk->rxstate & 0x2) {
        print("UART Error (TX)\n");
        return 0;
    }
    else if(!uart_reg_blk->rxstate & 1) {
        print("UART Data not Ready (RX)\n");
        return 0;
    }
    else {
        return (uart_reg_blk->rxstate & 1);
    }
}
uint8_t UART::get_RX_NumByteRead(){
    return (uart_reg_blk->rxdata >> 24);
}
bool UART::checkTx_state(){
    if(uart_reg_blk->txstate & 0x2) {
        print("UART Error (TX)\n");
        return 0;
    }
    else if(!uart_reg_blk->txstate & 1) {
        print("UART TX not ready to send (TX)\n");
        return 0;
    }
    else {
        return (uart_reg_blk->txstate & 1);
    }
}
uint8_t UART::get_TX_BR(){
    return (uart_reg_blk->txstate >> 16);
}
