// include/cpu/opcode.h

#ifndef OPCODE_H
#define OPCODE_H

#include <stdint.h>

/**
 * Elenco degli opcode principali.
 * Ogni istruzione a 32 bit: [ 8 bit opcode | ... bit vari ... ]
 *
 * NB: puoi adattare i valori per evitare sovrapposizioni,
 *     qui mostriamo una possibile mappatura.
 */
typedef enum
{
    OP_MOV = 0x01,   // MOV reg, reg
    OP_MOVI = 0x02,  // MOV reg, immediato
    OP_ADD = 0x03,   // ADD reg, reg, reg
    OP_SUB = 0x04,   // SUB reg, reg, reg
    OP_AND = 0x05,   // AND reg, reg, reg
    OP_OR = 0x06,    // OR  reg, reg, reg
    OP_XOR = 0x07,   // XOR reg, reg, reg
    OP_NOT = 0x08,   // NOT reg, reg
    OP_SHL = 0x09,   // SHL reg, reg, (shift imm8) -- come esempio
    OP_SHR = 0x0A,   // SHR reg, reg, (shift imm8)
    OP_LOAD = 0x10,  // LOAD reg, [addr imm]
    OP_STORE = 0x11, // STORE reg, [addr imm]
    OP_JMP = 0x20,   // JMP imm24
    OP_JEQ = 0x21,   // JEQ imm24 (salta se Z=1)
    OP_JNE = 0x22,   // JNE imm24 (Z=0)
    OP_PUSH = 0x30,
    OP_POP = 0x31,
    OP_CALL = 0x32,
    OP_RET = 0x33,
    OP_DRAW = 0x40, // DRAW reg, reg, reg
    OP_UNKNOWN = 0xFF
} Opcode;

/**
 * Bitmask per i flag del CPU.
 * Questi verranno memorizzati in cpu->flags.
 */
#define FLAG_CARRY (1 << 0)
#define FLAG_ZERO (1 << 1)
#define FLAG_SIGN (1 << 2)
#define FLAG_OVERFLOW (1 << 3)

/**
 * Se desideri definire costanti di dimensione registri,
 * dimensione max Mem, ecc.
 */
#define NUM_REGISTERS 16
#define MEMORY_SIZE 0x100000 // 1 MB

#endif // OPCODE_H
