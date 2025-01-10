// include/cpu/alu.h

#ifndef ALU_H
#define ALU_H

#include <stdint.h>

/**
 * Elenco delle operazioni ALU di base.
 */
typedef enum
{
    ALU_ADD,
    ALU_SUB,
    ALU_AND,
    ALU_OR,
    ALU_XOR,
    ALU_NOT, // unario
    ALU_SHL, // shift left
    ALU_SHR, // shift right
} ALU_Operation;

/**
 * Esegue un'operazione ALU su operand1 e operand2 (o operand secondario).
 * @param op:   operazione ALU (ADD, SUB, ecc.)
 * @param op1:  primo operando
 * @param op2:  secondo operando (se l'operazione è unaria, ignorato o 0)
 * @param flags: puntatore a un uint32_t che contiene/aggiorna i flag
 * @return il risultato a 32 bit dell'operazione
 */
uint32_t alu_execute(ALU_Operation op, uint32_t op1, uint32_t op2, uint32_t *flags);

#endif // ALU_H
