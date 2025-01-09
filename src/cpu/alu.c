#include "cpu/alu.h"
#include "cpu/opcode.h" // Per FLAG_CARRY, FLAG_ZERO, ecc.

/**
 * Esegue un'operazione ALU su operand1 e operand2.
 * @param op       : tipo di operazione (ALU_ADD, ALU_SUB, ecc.)
 * @param operand1 : primo operando
 * @param operand2 : secondo operando (o shift amount, se l'operazione è SHL/SHR)
 * @param flags    : puntatore a un uint32_t che conterrà i flag (Carry, Zero, Sign, Overflow)
 * @return         : risultato a 32 bit dell'operazione
 */
uint32_t alu_execute(ALU_Operation op, uint32_t operand1, uint32_t operand2, uint32_t *flags)
{
    // Resetta i flag all'inizio di ogni operazione
    *flags = 0;

    // Per gestire carry/borrow e overflow, conviene usare un wide_result (64 bit).
    uint64_t wide_result = 0;
    uint32_t result = 0;

    switch (op)
    {
    case ALU_ADD:
        wide_result = (uint64_t)operand1 + (uint64_t)operand2;
        result = (uint32_t)wide_result;
        // Flag di Carry (unsigned) → se wide_result supera 0xFFFFFFFF
        if (wide_result & 0xFFFFFFFF00000000ULL)
        {
            *flags |= FLAG_CARRY;
        }
        // Overflow con segno (bit 31) → logica standard “two’s complement”
        // Se il segno del risultato differisce dal segno dei due operand
        // e i due operand hanno lo stesso segno, c'è un overflow.
        if (((operand1 ^ result) & (operand2 ^ result) & 0x80000000) != 0)
        {
            *flags |= FLAG_OVERFLOW;
        }
        break;

    case ALU_SUB:
        wide_result = (uint64_t)operand1 - (uint64_t)operand2;
        result = (uint32_t)wide_result;
        // Borrow: in un'architettura “semplice”, potremmo riusare FLAG_CARRY
        // per dire “c’è stato un borrow” se wide_result ha un bit in posizione 32
        if ((wide_result & 0x100000000ULL) != 0)
        {
            *flags |= FLAG_CARRY; // interpretato come borrow
        }
        // Overflow con segno:
        // Avviene quando operand1 e operand2 hanno segni diversi,
        // e il risultato ha un segno diverso da operand1.
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
        // Operazione unaria: usiamo operand1, ignoriamo operand2
        result = ~operand1;
        break;

    case ALU_SHL:
        // operand2 è lo shift amount
        result = operand1 << (operand2 & 0x1F);
        // Se i bit in uscita contenevano 1, potresti settare Carry
        // (dipende da come vuoi definire la semantica dello shift).
        break;

    case ALU_SHR:
        // shift logico a destra
        result = operand1 >> (operand2 & 0x1F);
        // Idem come sopra, potresti impostare carry se i bit in uscita sono 1
        break;

        // Se hai altre operazioni (MUL, DIV, ecc.) aggiungile qui
    }

    // Flag Zero
    if (result == 0)
    {
        *flags |= FLAG_ZERO;
    }

    // Flag Sign (negativo) → bit più alto
    if (result & 0x80000000)
    {
        *flags |= FLAG_SIGN;
    }

    return result;
}

// uint32_t alu_execute(ALU_Operation op, uint32_t operand1, uint32_t operand2, uint32_t *flags)
// {
//     // Reset flags prima di ogni operazione
//     *flags = 0;

//     // Per gestione carry/overflow usiamo wide_result (64 bit).
//     uint64_t wide_result = 0;
//     uint32_t result = 0;

//     switch (op)
//     {
//     case ALU_ADD:
//         wide_result = (uint64_t)operand1 + (uint64_t)operand2;
//         result = (uint32_t)wide_result;
//         if (wide_result & 0xFFFFFFFF00000000ULL)
//         {
//             *flags |= FLAG_CARRY;
//         }
//         // Overflow in add con segno
//         if (((operand1 ^ result) & (operand2 ^ result) & 0x80000000) != 0)
//         {
//             *flags |= FLAG_OVERFLOW;
//         }
//         break;

//     case ALU_SUB:
//         // wide_result in 2's complement
//         wide_result = (uint64_t)operand1 - (uint64_t)operand2;
//         result = (uint32_t)wide_result;
//         // Carry interpretato come borrow
//         if ((wide_result & 0x100000000ULL) != 0)
//         {
//             *flags |= FLAG_CARRY;
//         }
//         // Overflow in sub con segno
//         if (((operand1 ^ operand2) & (operand1 ^ result) & 0x80000000) != 0)
//         {
//             *flags |= FLAG_OVERFLOW;
//         }
//         break;

//     case ALU_AND:
//         result = operand1 & operand2;
//         break;

//     case ALU_OR:
//         result = operand1 | operand2;
//         break;

//     case ALU_XOR:
//         result = operand1 ^ operand2;
//         break;

//     case ALU_NOT:
//         // unario: useremo operand1 come input, operand2 ignorato
//         result = ~operand1;
//         break;

//     case ALU_SHL:
//         // useremo operand2 come shift amount (0..31)
//         result = operand1 << (operand2 & 0x1F);
//         break;

//     case ALU_SHR:
//         // shift logico a destra
//         result = operand1 >> (operand2 & 0x1F);
//         break;
//     }

//     // Impostiamo ZERO e SIGN
//     if (result == 0)
//     {
//         *flags |= FLAG_ZERO;
//     }
//     if (result & 0x80000000)
//     {
//         *flags |= FLAG_SIGN;
//     }

//     return result;
// }
