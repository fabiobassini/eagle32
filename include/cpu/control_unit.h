// include/cpu/control_unit.h

#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include <stdint.h>
#include "cpu.h" // Forward reference a CPU
#include "opcode.h"

/**
 * Decodifica l'istruzione e restituisce l'opcode interpretato.
 * NB: Invece di restituire un ControlSignal custom,
 *     possiamo semplicemente restituire un 'Opcode'.
 */
Opcode decode_instruction(uint32_t instruction);

#endif // CONTROL_UNIT_H
