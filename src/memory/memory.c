// memory.c
#include "memory.h"
#include <string.h>

static uint8_t mem[MEMORY_SIZE];

void memory_init()
{
    memset(mem, 0, MEMORY_SIZE);
}

uint32_t memory_read_word(uint32_t address)
{
    return (mem[address] << 24) | (mem[address + 1] << 16) |
           (mem[address + 2] << 8) | (mem[address + 3]);
}

void memory_write_word(uint32_t address, uint32_t data)
{
    mem[address] = (data >> 24) & 0xFF;
    mem[address + 1] = (data >> 16) & 0xFF;
    mem[address + 2] = (data >> 8) & 0xFF;
    mem[address + 3] = data & 0xFF;
}
