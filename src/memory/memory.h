// memory.h
#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define MEMORY_SIZE 0x100000 // 1 MB

void memory_init();
uint32_t memory_read_word(uint32_t address);
void memory_write_word(uint32_t address, uint32_t data);

#endif
