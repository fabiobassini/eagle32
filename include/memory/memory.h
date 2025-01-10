// include/memory/memory.h

#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include "cpu/opcode.h" // per MEMORY_SIZE

/**
 * Inizializza la memoria (azzera tutto).
 */
void memory_init();

/**
 * Legge una word (32 bit) da 'address' (allineato).
 * Se l'architettura è big-endian o little-endian, la funzione
 * deve essere coerente con la convenzione scelta.
 */
uint32_t memory_read_word(uint32_t address);

/**
 * Scrive una word (32 bit) all'indirizzo 'address'.
 */
void memory_write_word(uint32_t address, uint32_t data);

#endif // MEMORY_H
