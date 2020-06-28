#include <stdint.h>

#define UART_BASE 0x10000000
#define UART_THR 0x0
#define UART_RBR 0x0
#define UART_IER 0x1
#define UART_FCR 0x2
#define UART_LCR 0x3

#define MTIMECMP  0x02004000
#define MTIME  0x0200bff8

void mmio_write(unsigned long address, int offset, uint8_t value)
{
	volatile uint8_t *reg = (uint8_t *)address;
	*(reg + offset) = value;
}

uint8_t mmio_read(unsigned long address, int offset)
{
	volatile uint8_t *reg = (uint8_t *)address;
	return *(reg + offset);
}

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

void mmio_write64(unsigned long address, int offset, uint64_t value)
{
	volatile uint64_t *reg = (uint64_t *)address;
	*(reg + offset) = value;
}

uint64_t mmio_read64(unsigned long address, int offset)
{
	volatile uint64_t *reg = (uint64_t *)address;
	return *(reg + offset);
}

void set_timer_interrupt(unsigned long interval) {
    uint64_t time = mmio_read64(MTIME, 0);
    mmio_write64(MTIMECMP, 0, time+interval);
}
void kmain() {
    uart_init(UART_BASE);
    uart_putc(UART_BASE, 'h');
    uart_putc(UART_BASE, 'i');
    uart_putc(UART_BASE, '\n');
    set_timer_interrupt(100000);
    while(1) {
        char  x = uart_getc(UART_BASE);
        uart_putc(UART_BASE, x);
    }
}

void timer_interrupt() {
    uart_putc(UART_BASE, 't');
    uart_putc(UART_BASE, 'i');
    uart_putc(UART_BASE, 'm');
    uart_putc(UART_BASE, 'e');
    uart_putc(UART_BASE, 'r');
    uart_putc(UART_BASE, '\n');
    set_timer_interrupt(100000);
}