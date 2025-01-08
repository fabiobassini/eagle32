// assembler.c
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static uint8_t parse_register(const char *token)
{
    // Supponiamo che il token inizi con 'R' seguito da un numero
    return (uint8_t)atoi(token + 1);
}

void assemble(const char *input_file, const char *output_file)
{
    FILE *fin = fopen(input_file, "r");
    if (!fin)
    {
        perror("Errore apertura file di input");
        return;
    }

    FILE *fout = fopen(output_file, "wb");
    if (!fout)
    {
        perror("Errore apertura file di output");
        fclose(fin);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fin))
    {
        // Rimuove newline/carriage return
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0)
            continue; // salta righe vuote

        uint32_t instruction = 0;
        char *token = strtok(line, " ,");

        if (!token)
            continue;

        if (strcmp(token, "DRAW") == 0)
        {
            instruction |= (0x30 << 24); // Imposta l'opcode per DRAW
            // Aspettiamo tre registri
            char *reg1 = strtok(NULL, " ,");
            char *reg2 = strtok(NULL, " ,");
            char *reg3 = strtok(NULL, " ,");

            if (reg1 && reg2 && reg3)
            {
                uint8_t r1 = parse_register(reg1);
                uint8_t r2 = parse_register(reg2);
                uint8_t r3 = parse_register(reg3);
                instruction |= (r1 & 0x0F) << 20;
                instruction |= (r2 & 0x0F) << 16;
                instruction |= (r3 & 0x0F) << 12;
            }
            else
            {
                fprintf(stderr, "Errore: istruzione DRAW richiede 3 registri\n");
                continue;
            }
        }
        else if (strcmp(token, "UNKNOWN") == 0)
        {
            instruction = 0xFFFFFFFF;
        }
        // Si possono aggiungere altri mnemonici qui

        fwrite(&instruction, sizeof(uint32_t), 1, fout);
    }

    fclose(fin);
    fclose(fout);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s input.asm output.bin\n", argv[0]);
        return 1;
    }
    assemble(argv[1], argv[2]);
    return 0;
}