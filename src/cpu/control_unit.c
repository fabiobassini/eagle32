// src/cpu/control_unit.c

#include "cpu/control_unit.h"
#include "cpu/opcode.h"

Opcode decode_instruction(uint32_t instruction)
{
    // L'opcode è nei bit 31..24
    uint8_t opcode_byte = (instruction >> 24) & 0xFF;

    switch (opcode_byte)
    {
    case OP_MOV:
        return OP_MOV;
    case OP_MOVI:
        return OP_MOVI;
    case OP_ADD:
        return OP_ADD;
    case OP_SUB:
        return OP_SUB;
    case OP_AND:
        return OP_AND;
    case OP_OR:
        return OP_OR;
    case OP_XOR:
        return OP_XOR;
    case OP_NOT:
        return OP_NOT;
    case OP_SHL:
        return OP_SHL;
    case OP_SHR:
        return OP_SHR;
    case OP_LOAD:
        return OP_LOAD;
    case OP_STORE:
        return OP_STORE;
    case OP_JMP:
        return OP_JMP;
    case OP_JEQ:
        return OP_JEQ;
    case OP_JNE:
        return OP_JNE;
    case OP_PUSH:
        return OP_PUSH;
    case OP_POP:
        return OP_POP;
    case OP_CALL:
        return OP_CALL;
    case OP_RET:
        return OP_RET;
    case OP_DRAW:
        return OP_DRAW;
    default:
        return OP_UNKNOWN;
    }
}
