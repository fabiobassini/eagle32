// bus.c
#include "bus.h"
#include "../memory/memory.h"
#include "../io/io.h"

uint32_t bus_read_word(uint32_t address)
{
    if (address < MEMORY_SIZE)
    {
        return memory_read_word(address);
    }
    else
    {
        return io_read(address - MEMORY_SIZE);
    }
}

void bus_write_word(uint32_t address, uint32_t data)
{
    if (address < MEMORY_SIZE)
    {
        memory_write_word(address, data);
    }
    else
    {
        io_write(address - MEMORY_SIZE, data);
    }
}
