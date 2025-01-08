// cpu.h
#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "registers.h" // Includiamo il modulo dei registri

typedef struct
{
    Registers regs; // Set di registri dedicato
    uint32_t pc;
    uint32_t sp;
    uint32_t flags;
} CPU;

void cpu_init(CPU *cpu);
void cpu_reset(CPU *cpu);
void cpu_execute(CPU *cpu);

#endif
