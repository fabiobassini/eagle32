#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>
#include "cpu/opcode.h" // Per NUM_REGISTERS se vuoi

/**
 * Struttura che contiene i registri generali del CPU.
 */
typedef struct
{
    uint32_t regs[NUM_REGISTERS];
} Registers;

/**
 * Inizializza i registri a 0.
 */
void registers_init(Registers *r);

/**
 * Legge dal registro index (0..15).
 */
uint32_t registers_read(const Registers *r, uint8_t index);

/**
 * Scrive nel registro index (0..15).
 */
void registers_write(Registers *r, uint8_t index, uint32_t value);

#endif // REGISTERS_H
