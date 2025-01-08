// register.h
#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

#define NUM_REGISTERS 16

typedef struct
{
    uint32_t regs[NUM_REGISTERS];
} Registers;

/**
 * Inizializza il set di registri azzerandoli.
 */
void registers_init(Registers *r);

/**
 * Legge il valore dal registro specificato.
 */
uint32_t registers_read(Registers *r, uint8_t index);

/**
 * Scrive un valore nel registro specificato.
 */
void registers_write(Registers *r, uint8_t index, uint32_t value);

#endif
