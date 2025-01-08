// alu.h
#ifndef ALU_H
#define ALU_H

#include <stdint.h>

typedef enum
{
    ALU_ADD,
    ALU_SUB,
    ALU_AND,
    ALU_OR,
    ALU_XOR
} ALU_Operation;

uint32_t alu_execute(ALU_Operation op, uint32_t operand1, uint32_t operand2, uint32_t *flags);

#endif
