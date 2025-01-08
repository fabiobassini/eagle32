// control_unit.c
#include "control_unit.h"

ControlSignal decode_instruction(uint32_t instruction, CPU *cpu)
{
    uint8_t opcode = (instruction >> 24) & 0xFF;

    switch (opcode)
    {
    case 0x01:
        return CTRL_MOV;
    case 0x02:
        return CTRL_ADD;
    case 0x03:
        return CTRL_SUB;
    case 0x10:
        return CTRL_LOAD;
    case 0x11:
        return CTRL_STORE;
    case 0x20:
        return CTRL_JMP;
    case 0x30:
        return CTRL_DRAW;
    default:
        return CTRL_UNKNOWN;
    }
}
