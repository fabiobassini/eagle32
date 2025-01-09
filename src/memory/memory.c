#include "memory/memory.h"
#include <string.h>
#include <stdio.h>

// Allocazione reale della memoria
static uint8_t mem[MEMORY_SIZE];

void memory_init()
{
    memset(mem, 0, MEMORY_SIZE);
}

uint32_t memory_read_word(uint32_t address)
{
    // Controllo eventuale disallineamento, se vogliamo
    if (address + 3 >= MEMORY_SIZE)
    {
        // Errore: read fuori memoria
        // Invece di crashare, potremmo restituire 0 o generare un messaggio
        fprintf(stderr, "memory_read_word: indirizzo fuori limite (0x%X)\n", address);
        return 0;
    }

    // Se la CPU è big-endian, costruiamo la word così come stai facendo (Byte0=MSB).
    // Se la CPU fosse little-endian, invertire (Byte0=LSB).
    // return (mem[address] << 24) | (mem[address + 1] << 16) | (mem[address + 2] << 8) | (mem[address + 3]);

    // memory_read_word (little-endian)
    return (mem[address]) | (mem[address + 1] << 8) | (mem[address + 2] << 16) | (mem[address + 3] << 24);
}

void memory_write_word(uint32_t address, uint32_t data)
{
    if (address + 3 >= MEMORY_SIZE)
    {
        fprintf(stderr, "memory_write_word: indirizzo fuori limite (0x%X)\n", address);
        return;
    }

    // mem[address] = (data >> 24) & 0xFF;
    // mem[address + 1] = (data >> 16) & 0xFF;
    // mem[address + 2] = (data >> 8) & 0xFF;
    // mem[address + 3] = data & 0xFF;

    // memory_write_word (little-endian)
    mem[address] = data & 0xFF;
    mem[address + 1] = (data >> 8) & 0xFF;
    mem[address + 2] = (data >> 16) & 0xFF;
    mem[address + 3] = (data >> 24) & 0xFF;
}
