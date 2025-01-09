#include "bus/bus.h"
#include "memory/memory.h"
#include "io/io.h"
#include "cpu/opcode.h" // per MEMORY_SIZE

uint32_t bus_read_word(uint32_t address)
{
    if (address < MEMORY_SIZE)
    {
        return memory_read_word(address);
    }
    else
    {
        // Se l'indirizzo è >= MEMORY_SIZE, lo consideriamo "porte I/O"
        // Ad esempio port = address - MEMORY_SIZE
        uint32_t port = address - MEMORY_SIZE;
        return io_read(port);
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
        uint32_t port = address - MEMORY_SIZE;
        io_write(port, data);
    }
}
