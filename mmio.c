#include "mmio.h"

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