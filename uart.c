#include "uart.h"
#include "mmio.h"

void uart_init(unsigned long address) {
    //Reset UART line
    mmio_write(address, UART_LCR, 0b11);
    //Enable FIFO
    mmio_write(address, UART_FCR, 1);
    //Enable interrupts
    mmio_write(address, UART_IER, 1);
}

void uart_putc(unsigned long address, uint8_t character) {
    mmio_write(address, UART_THR, character);
}

uint8_t uart_getc(unsigned long address) {
    return mmio_read(address, UART_RBR);
} 