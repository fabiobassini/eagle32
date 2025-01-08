// bus.h
#ifndef BUS_H
#define BUS_H

#include <stdint.h>

uint32_t bus_read_word(uint32_t address);
void bus_write_word(uint32_t address, uint32_t data);

#endif
