#include <stdint.h>
#include "uart.h"
#include "mmio.h"

#define MTIMECMP  0x02004000
#define MTIME  0x0200bff8
#define MAX_TASKS 5
#define NUM_REGISTERS 32
struct Task{
    uint64_t registers[NUM_REGISTERS];
    uint64_t program_counter;
    uint64_t stack_pointer;
};
int curr_task = -1;
struct Task tasks[MAX_TASKS];
int num_tasks = 1;


void set_timer_interrupt(unsigned long interval) {
    uint64_t time = mmio_read64(MTIME, 0);
    mmio_write64(MTIMECMP, 0, time+interval);
}

void thread1() {
    while(1) {
        uart_putc(UART_BASE, '1');
        uart_putc(UART_BASE, '\n');
    }
}

void thread2() {
    while(1) {
        uart_putc(UART_BASE, '2');
        uart_putc(UART_BASE, '\n');
    }
}

void thread3() {
    while(1) {
        uart_putc(UART_BASE, '3');
        uart_putc(UART_BASE, '\n');
    }
}
void park() {
    while(1) {}
}
extern void _bss_end;
extern void _global_pointer;
int create_thread(void (*fun_ptr)(void), int stack_size) {
    //REMEMBER TO LOCK AND DISABLE INTERRUPTS IN FUTURE
    static uint64_t allocated_stack = &_bss_end+10000; // Kernel stack size - reserve 10KB for kernel
    if(curr_task  < 0) {
        curr_task = 0;
    }
    if(num_tasks >= MAX_TASKS) return -1;
    tasks[num_tasks].registers[0] = fun_ptr; //TODO create a termination callback
    tasks[num_tasks].registers[1] = allocated_stack;
    tasks[num_tasks].registers[2] = &_global_pointer;
    tasks[num_tasks].registers[31] = fun_ptr;
    tasks[num_tasks].program_counter = fun_ptr;
    tasks[num_tasks].stack_pointer = allocated_stack;
    num_tasks++;
    allocated_stack += stack_size;
    return num_tasks;
}

void init_scheduler() {
    set_timer_interrupt(10000000);
}
void kmain() {
    uart_init(UART_BASE);
    //uart_putc(UART_BASE, 'h');
    //uart_putc(UART_BASE, 'i');
    //uart_putc(UART_BASE, '\n');

    //create_thread(thread1, 1000);
    create_thread(thread2, 1000);
    create_thread(thread3, 1000);
    set_timer_interrupt(10000000);
    thread1(); 
}
uint64_t timer_interrupt(uint64_t stack_pointer, uint64_t mepc) {
    // Timer interrupt is cleared only when MTIME_CMP IS WRITTEN TO
    
    if(curr_task  < 0) {
        num_tasks++;
        curr_task = 0;
    }
    for(int i = 0; i < NUM_REGISTERS; i++) {
        tasks[curr_task%num_tasks].registers[i] = mmio_read64(stack_pointer+i*8, 0);
    }
    tasks[curr_task%num_tasks].program_counter = mepc;
    tasks[curr_task%num_tasks].stack_pointer = stack_pointer;
    curr_task++;
    stack_pointer = tasks[curr_task%num_tasks].stack_pointer;
    for(int i = 0; i < NUM_REGISTERS; i++) {
        mmio_write64(stack_pointer+i*8, 0, tasks[curr_task%num_tasks].registers[i]);
    }

    set_timer_interrupt(10000000);
    return stack_pointer;
}


#define MTIMER_INTERRUPT 7
uint64_t generic_interrupt(uint64_t mcause, uint64_t stack_pointer, uint64_t mepc) {
    uint64_t type = mcause & 0x7FFFFFFFFFFFFFFF;
    uint64_t interrupt = mcause & 0x8000000000000000;
    if(interrupt) {
        if(type == MTIMER_INTERRUPT) {
            return timer_interrupt(stack_pointer, mepc);
        }
    }
    return stack_pointer;
}