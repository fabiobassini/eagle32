#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "registers.h"
#include "opcode.h"

/**
 * Struttura che descrive lo stato globale della CPU.
 */
typedef struct
{
    Registers regs;  // Registri generali
    uint32_t pc;     // Program Counter
    uint32_t sp;     // Stack Pointer
    uint32_t flags;  // Contenitore di bit (carry, zero, sign, overflow, ecc.)
    uint8_t running; // Flag che indica se la CPU è in esecuzione
} CPU;

/**
 * Inizializza la CPU (azzerando registri, pc, ecc.).
 */
void cpu_init(CPU *cpu);

/**
 * Reset della CPU (stato iniziale).
 */
void cpu_reset(CPU *cpu);

/**
 * Esegue il ciclo fetch-decode-execute in loop
 * finché la CPU non viene fermata.
 */
void cpu_run(CPU *cpu);

/**
 * Esegui una singola istruzione (utile per debugging step-by-step).
 * Restituisce 0 se l'istruzione era OP_UNKNOWN (o se la CPU si spegne).
 */
int cpu_step(CPU *cpu);

#endif // CPU_H
