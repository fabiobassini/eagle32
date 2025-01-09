#include "cpu/registers.h"
#include <string.h>

void registers_init(Registers *r)
{
    memset(r->regs, 0, sizeof(r->regs));
}

uint32_t registers_read(const Registers *r, uint8_t index)
{
    if (index < NUM_REGISTERS)
    {
        return r->regs[index];
    }
    return 0; // Oppure potresti generare un errore
}

void registers_write(Registers *r, uint8_t index, uint32_t value)
{
    if (index < NUM_REGISTERS)
    {
        r->regs[index] = value;
    }
}
