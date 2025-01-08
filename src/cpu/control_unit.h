// control_unit.h
#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include <stdint.h>
#include "cpu.h"

typedef enum
{
    CTRL_MOV,
    CTRL_ADD,
    CTRL_SUB,
    CTRL_LOAD,
    CTRL_STORE,
    CTRL_JMP,
    CTRL_DRAW,
    CTRL_UNKNOWN
} ControlSignal;

ControlSignal decode_instruction(uint32_t instruction, CPU *cpu);

#endif
