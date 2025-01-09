#include "cpu/alu.h"
#include "cpu/opcode.h" // Per FLAG_CARRY, FLAG_ZERO, ecc.

uint32_t alu_execute(ALU_Operation op, uint32_t operand1, uint32_t operand2, uint32_t *flags)
{
    // Reset flags prima di ogni operazione
    *flags = 0;

    // Per gestione carry/overflow usiamo wide_result (64 bit).
    uint64_t wide_result = 0;
    uint32_t result = 0;

    switch (op)
    {
    case ALU_ADD:
        wide_result = (uint64_t)operand1 + (uint64_t)operand2;
        result = (uint32_t)wide_result;
        if (wide_result & 0xFFFFFFFF00000000ULL)
        {
            *flags |= FLAG_CARRY;
        }
        // Overflow in add con segno
        if (((operand1 ^ result) & (operand2 ^ result) & 0x80000000) != 0)
        {
            *flags |= FLAG_OVERFLOW;
        }
        break;

    case ALU_SUB:
        // wide_result in 2's complement
        wide_result = (uint64_t)operand1 - (uint64_t)operand2;
        result = (uint32_t)wide_result;
        // Carry interpretato come borrow
        if ((wide_result & 0x100000000ULL) != 0)
        {
            *flags |= FLAG_CARRY;
        }
        // Overflow in sub con segno
        if (((operand1 ^ operand2) & (operand1 ^ result) & 0x80000000) != 0)
        {
            *flags |= FLAG_OVERFLOW;
        }
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

    case ALU_NOT:
        // unario: useremo operand1 come input, operand2 ignorato
        result = ~operand1;
        break;

    case ALU_SHL:
        // useremo operand2 come shift amount (0..31)
        result = operand1 << (operand2 & 0x1F);
        break;

    case ALU_SHR:
        // shift logico a destra
        result = operand1 >> (operand2 & 0x1F);
        break;
    }

    // Impostiamo ZERO e SIGN
    if (result == 0)
    {
        *flags |= FLAG_ZERO;
    }
    if (result & 0x80000000)
    {
        *flags |= FLAG_SIGN;
    }

    return result;
}
