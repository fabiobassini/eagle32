// include/bus/bus.h

#ifndef BUS_H
#define BUS_H

#include <stdint.h>

/**
 * Legge una word (32 bit) da un 'address'.
 * Se address < MEMORY_SIZE, si usa la memoria;
 * altrimenti, si considera I/O o periferiche.
 */
uint32_t bus_read_word(uint32_t address);

/**
 * Scrive una word (32 bit) a 'address'.
 */
void bus_write_word(uint32_t address, uint32_t data);

#endif // BUS_H
