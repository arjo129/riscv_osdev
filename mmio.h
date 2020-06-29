#ifndef __MMIO_H__
#define __MMIO_H__
#include <stdint.h>
void mmio_write(unsigned long address, int offset, uint8_t value);

uint8_t mmio_read(unsigned long address, int offset);


void mmio_write64(unsigned long address, int offset, uint64_t value);

uint64_t mmio_read64(unsigned long address, int offset);

#endif