#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>
//Memory map
#define UART_BASE 0x10000000
#define UART_THR 0x0
#define UART_RBR 0x0
#define UART_IER 0x1
#define UART_FCR 0x2
#define UART_LCR 0x3

void uart_init(unsigned long address);
void uart_putc(unsigned long address, uint8_t character);
uint8_t uart_getc(unsigned long address);
#endif