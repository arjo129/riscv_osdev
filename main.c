#include <stdint.h>
#include "uart.h"
#include "mmio.h"

#define MTIMECMP  0x02004000
#define MTIME  0x0200bff8



void set_timer_interrupt(unsigned long interval) {
    uint64_t time = mmio_read64(MTIME, 0);
    mmio_write64(MTIMECMP, 0, time+interval);
}
void kmain() {
    uart_init(UART_BASE);
    uart_putc(UART_BASE, 'h');
    uart_putc(UART_BASE, 'i');
    uart_putc(UART_BASE, '\n');
    set_timer_interrupt(10000000);
    while(1) {
        char  x = uart_getc(UART_BASE);
        uart_putc(UART_BASE, x);
    }
}
void timer_interrupt() {
    // Timer interrupt is cleared only when MTIME_CMP IS WRITTEN TO
    set_timer_interrupt(10000000); 
    uart_putc(UART_BASE, 't');
    uart_putc(UART_BASE, 'i');
    uart_putc(UART_BASE, 'm');
    uart_putc(UART_BASE, 'e');
    uart_putc(UART_BASE, 'r');
    uart_putc(UART_BASE, '\n');
}

#define MTIMER_INTERRUPT 7
void generic_interrupt(uint64_t mcause) {
    uint64_t type = mcause & 0x7FFFFFFFFFFFFFFF;
    uint64_t interrupt = mcause & 0x8000000000000000;
    if(interrupt) {
        if(type == MTIMER_INTERRUPT) {
            timer_interrupt();
        }
    } 
}