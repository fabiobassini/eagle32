// alu.c
#include "alu.h"

uint32_t alu_execute(ALU_Operation op, uint32_t operand1, uint32_t operand2, uint32_t *flags)
{
    uint32_t result = 0;
    switch (op)
    {
    case ALU_ADD:
        result = operand1 + operand2;
        break;
    case ALU_SUB:
        result = operand1 - operand2;
        break;
    case ALU_AND:
        result = operand1 & operand2;
        break;
    case ALU_OR:
        result = operand1 | operand2;
        break;
    case ALU_XOR:
        result = operand1 ^ operand2;
        break;
    default:
        break;
    }
    return result;
}
