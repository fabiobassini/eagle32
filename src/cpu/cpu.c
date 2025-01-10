// src/cpu/cpu.c

#include "cpu/cpu.h"
#include "cpu/control_unit.h"
#include "cpu/alu.h"
#include "cpu/registers.h"
#include "bus/bus.h"
#include "graphics/graphics.h"
#include <stdio.h>

/**
 * Funzioni ausiliarie per pipeline
 */
static inline uint32_t fetch_instruction(CPU *cpu)
{
    uint32_t instr = bus_read_word(cpu->pc);
    printf("PC=0x%08X => instr=0x%08X\n", cpu->pc, instr);

    cpu->pc += 4;
    return instr;
}

/**
 * Esegue l'istruzione su CPU, restituendo 0 se interrompe l'esecuzione,
 * 1 se continua.
 */
static int execute_instruction(CPU *cpu, Opcode op, uint32_t instruction)
{
    switch (op)
    {
    case OP_MOV:
    {
        // form: [31..24: opcode] [23..20: rd] [19..16: rs] [15..0: unused]
        uint8_t rd = (instruction >> 20) & 0x0F;
        uint8_t rs = (instruction >> 16) & 0x0F;
        uint32_t val = registers_read(&cpu->regs, rs);
        registers_write(&cpu->regs, rd, val);
        break;
    }
    case OP_MOVI:
    {
        // form: [31..24: opcode] [23..20: rd] [19..0: immediate]
        uint8_t rd = (instruction >> 20) & 0x0F;
        uint32_t imm = instruction & 0xFFFFF; // 20 bit di immediato
        registers_write(&cpu->regs, rd, imm);
        break;
    }
    case OP_ADD:
    {
        // form: [31..24: opcode] [23..20: rd] [19..16: rs] [15..12: rt] ...
        uint8_t rd = (instruction >> 20) & 0x0F;
        uint8_t rs = (instruction >> 16) & 0x0F;
        uint8_t rt = (instruction >> 12) & 0x0F;
        uint32_t result = alu_execute(
            ALU_ADD,
            registers_read(&cpu->regs, rs),
            registers_read(&cpu->regs, rt),
            &cpu->flags);
        registers_write(&cpu->regs, rd, result);
        break;
    }
    case OP_SUB:
    {
        uint8_t rd = (instruction >> 20) & 0x0F;
        uint8_t rs = (instruction >> 16) & 0x0F;
        uint8_t rt = (instruction >> 12) & 0x0F;
        uint32_t result = alu_execute(
            ALU_SUB,
            registers_read(&cpu->regs, rs),
            registers_read(&cpu->regs, rt),
            &cpu->flags);
        registers_write(&cpu->regs, rd, result);
        break;
    }
    case OP_AND:
    {
        uint8_t rd = (instruction >> 20) & 0x0F;
        uint8_t rs = (instruction >> 16) & 0x0F;
        uint8_t rt = (instruction >> 12) & 0x0F;
        uint32_t result = alu_execute(
            ALU_AND,
            registers_read(&cpu->regs, rs),
            registers_read(&cpu->regs, rt),
            &cpu->flags);
        registers_write(&cpu->regs, rd, result);
        break;
    }
    case OP_OR:
    {
        uint8_t rd = (instruction >> 20) & 0x0F;
        uint8_t rs = (instruction >> 16) & 0x0F;
        uint8_t rt = (instruction >> 12) & 0x0F;
        uint32_t result = alu_execute(
            ALU_OR,
            registers_read(&cpu->regs, rs),
            registers_read(&cpu->regs, rt),
            &cpu->flags);
        registers_write(&cpu->regs, rd, result);
        break;
    }
    case OP_XOR:
    {
        uint8_t rd = (instruction >> 20) & 0x0F;
        uint8_t rs = (instruction >> 16) & 0x0F;
        uint8_t rt = (instruction >> 12) & 0x0F;
        uint32_t result = alu_execute(
            ALU_XOR,
            registers_read(&cpu->regs, rs),
            registers_read(&cpu->regs, rt),
            &cpu->flags);
        registers_write(&cpu->regs, rd, result);
        break;
    }
    case OP_NOT:
    {
        // form: [31..24: opcode] [23..20: rd] [19..16: rs]
        uint8_t rd = (instruction >> 20) & 0x0F;
        uint8_t rs = (instruction >> 16) & 0x0F;
        uint32_t result = alu_execute(
            ALU_NOT,
            registers_read(&cpu->regs, rs),
            0,
            &cpu->flags);
        registers_write(&cpu->regs, rd, result);
        break;
    }
    case OP_SHL:
    {
        // form: [31..24: opcode] [23..20: rd] [19..16: rs] [15..8: imm8?]
        uint8_t rd = (instruction >> 20) & 0x0F;
        uint8_t rs = (instruction >> 16) & 0x0F;
        // ipotizziamo che i successivi 8 bit siano shift amount
        uint8_t shift_amount = (instruction >> 8) & 0xFF;
        uint32_t result = alu_execute(
            ALU_SHL,
            registers_read(&cpu->regs, rs),
            shift_amount,
            &cpu->flags);
        registers_write(&cpu->regs, rd, result);
        break;
    }
    case OP_SHR:
    {
        uint8_t rd = (instruction >> 20) & 0x0F;
        uint8_t rs = (instruction >> 16) & 0x0F;
        uint8_t shift_amount = (instruction >> 8) & 0xFF;
        uint32_t result = alu_execute(
            ALU_SHR,
            registers_read(&cpu->regs, rs),
            shift_amount,
            &cpu->flags);
        registers_write(&cpu->regs, rd, result);
        break;
    }
    case OP_LOAD:
    {
        // form: [31..24: opcode] [23..20: rd] [19..0: address]
        uint8_t rd = (instruction >> 20) & 0x0F;
        uint32_t addr = (instruction & 0xFFFFF);
        uint32_t data = bus_read_word(addr);
        registers_write(&cpu->regs, rd, data);
        break;
    }
    case OP_STORE:
    {
        uint8_t rs = (instruction >> 20) & 0x0F;
        uint32_t addr = (instruction & 0xFFFFF);
        uint32_t data = registers_read(&cpu->regs, rs);
        bus_write_word(addr, data);
        break;
    }
    case OP_JMP:
    {
        // form: [31..24: opcode] [23..0: immediate address]
        uint32_t addr = instruction & 0xFFFFFF;
        cpu->pc = addr;
        break;
    }
    case OP_JEQ:
    {
        // salta se Z=1
        uint32_t addr = instruction & 0xFFFFFF;
        if (cpu->flags & FLAG_ZERO)
        {
            cpu->pc = addr;
        }
        break;
    }
    case OP_JNE:
    {
        uint32_t addr = instruction & 0xFFFFFF;
        if (!(cpu->flags & FLAG_ZERO))
        {
            cpu->pc = addr;
        }
        break;
    }
    case OP_PUSH:
    {
        // form: [31..24: opcode] [23..20: rs]
        uint8_t rs = (instruction >> 20) & 0x0F;
        uint32_t data = registers_read(&cpu->regs, rs);
        // scriviamo data nello stack
        cpu->sp -= 4; // decrementa stack pointer
        bus_write_word(cpu->sp, data);
        break;
    }
    case OP_POP:
    {
        uint8_t rd = (instruction >> 20) & 0x0F;
        uint32_t data = bus_read_word(cpu->sp);
        cpu->sp += 4;
        registers_write(&cpu->regs, rd, data);
        break;
    }
    case OP_CALL:
    {
        // form: [31..24: opcode] [23..0: address]
        uint32_t addr = instruction & 0xFFFFFF;
        // push PC su stack
        cpu->sp -= 4;
        bus_write_word(cpu->sp, cpu->pc);
        // salta
        cpu->pc = addr;
        break;
    }
    case OP_RET:
    {
        // pop PC
        uint32_t ret_addr = bus_read_word(cpu->sp);
        cpu->sp += 4;
        cpu->pc = ret_addr;
        break;
    }
    case OP_DRAW:
    {
        // form: [31..24: opcode] [23..20: rx] [19..16: ry] [15..12: rc]
        uint8_t rx = (instruction >> 20) & 0x0F;
        uint8_t ry = (instruction >> 16) & 0x0F;
        uint8_t rc = (instruction >> 12) & 0x0F;
        uint32_t x = registers_read(&cpu->regs, rx);
        uint32_t y = registers_read(&cpu->regs, ry);
        uint32_t color = registers_read(&cpu->regs, rc);

        printf("[DEBUG] DRAW -> (x=%u, y=%u, color=0x%06X)\n", x, y, color);
        graphics_draw(x, y, color);
        break;
    }
    case OP_UNKNOWN:
    default:
        // Fermiamo la CPU
        return 0;
    }

    return 1;
}

void cpu_init(CPU *cpu)
{
    cpu_reset(cpu);
}

void cpu_reset(CPU *cpu)
{
    registers_init(&cpu->regs);
    cpu->pc = 0;
    cpu->sp = 0x10000; // stack top? decidi tu (se hai 1MB di RAM, potresti metterlo altrove)
    cpu->flags = 0;
    cpu->running = 1;
}

int cpu_step(CPU *cpu)
{
    // Se la CPU è già ferma, non fare nulla
    if (!cpu->running)
        return 0;

    // Fetch
    uint32_t instruction = fetch_instruction(cpu);

    // Decode
    Opcode op = decode_instruction(instruction);

    // Execute
    int ok = execute_instruction(cpu, op, instruction);

    if (!ok)
    {
        cpu->running = 0;
    }

    return ok;
}

void cpu_run(CPU *cpu)
{
    while (cpu->running)
    {
        if (!cpu_step(cpu))
        {
            // se step restituisce 0, esci
            break;
        }
    }
}
