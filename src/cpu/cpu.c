// cpu.c
#include "cpu.h"
#include "control_unit.h"
#include "alu.h"
#include "registers.h" // Inclusione del modulo dei registri
#include "../bus/bus.h"
#include "../graphics/graphics.h"

void cpu_init(CPU *cpu)
{
    cpu_reset(cpu);
}

void cpu_reset(CPU *cpu)
{
    registers_init(&cpu->regs); // Inizializza i registri tramite il modulo dedicato
    cpu->pc = 0;
    cpu->sp = 0xFFFF;
    cpu->flags = 0;
}

void cpu_execute(CPU *cpu)
{
    int running = 1;
    while (running)
    {
        // Fetch: leggi l'istruzione dalla memoria tramite il Bus
        uint32_t instruction = bus_read_word(cpu->pc);
        cpu->pc += 4;

        // Decode ed Execute in base al segnale di controllo
        switch (decode_instruction(instruction, cpu))
        {
        case CTRL_MOV:
        {
            uint8_t rd = (instruction >> 20) & 0x0F;
            uint8_t rs = (instruction >> 16) & 0x0F;
            uint32_t value = registers_read(&cpu->regs, rs);
            registers_write(&cpu->regs, rd, value);
            break;
        }
        case CTRL_ADD:
        {
            uint8_t rd = (instruction >> 20) & 0x0F;
            uint8_t rs = (instruction >> 16) & 0x0F;
            uint8_t rt = (instruction >> 12) & 0x0F;
            uint32_t result = alu_execute(ALU_ADD,
                                          registers_read(&cpu->regs, rs),
                                          registers_read(&cpu->regs, rt),
                                          &cpu->flags);
            registers_write(&cpu->regs, rd, result);
            break;
        }
        case CTRL_SUB:
        {
            uint8_t rd = (instruction >> 20) & 0x0F;
            uint8_t rs = (instruction >> 16) & 0x0F;
            uint8_t rt = (instruction >> 12) & 0x0F;
            uint32_t result = alu_execute(ALU_SUB,
                                          registers_read(&cpu->regs, rs),
                                          registers_read(&cpu->regs, rt),
                                          &cpu->flags);
            registers_write(&cpu->regs, rd, result);
            break;
        }
        case CTRL_LOAD:
        {
            uint8_t rd = (instruction >> 20) & 0x0F;
            uint32_t addr = instruction & 0xFFFFF;
            uint32_t data = bus_read_word(addr);
            registers_write(&cpu->regs, rd, data);
            break;
        }
        case CTRL_STORE:
        {
            uint8_t rs = (instruction >> 20) & 0x0F;
            uint32_t addr = instruction & 0xFFFFF;
            uint32_t data = registers_read(&cpu->regs, rs);
            bus_write_word(addr, data);
            break;
        }
        case CTRL_JMP:
        {
            uint32_t addr = instruction & 0xFFFFFF;
            cpu->pc = addr;
            break;
        }
        case CTRL_DRAW:
        {
            uint8_t rx = (instruction >> 20) & 0x0F;
            uint8_t ry = (instruction >> 16) & 0x0F;
            uint8_t rc = (instruction >> 12) & 0x0F;
            uint32_t x = registers_read(&cpu->regs, rx);
            uint32_t y = registers_read(&cpu->regs, ry);
            uint32_t color = registers_read(&cpu->regs, rc);
            graphics_draw(x, y, color);
            break;
        }
        default:
            running = 0;
            break;
        }
    }
}
