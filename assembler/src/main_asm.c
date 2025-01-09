#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "scanner.h"

// Definizione della struttura ParsedLine
typedef struct
{
    AsmLine line;
    int address; // indirizzo dell'istruzione
} ParsedLine;

// Dichiarazione delle funzioni getter
const ParsedLine *get_parsed_lines();
int get_line_used();

// Funzione per stampare un'istruzione in formato leggibile
void print_instruction(AsmLine *line, int address)
{
    printf("0x%04X: %s", address, line->mnemonic);

    for (int i = 0; i < line->ntok; i++)
    {
        printf(" %s", line->tokens[i]);
    }

    if (line->label[0] != '\0')
    {
        printf(" ; Label: %s", line->label);
    }
    printf("\n");
}

// int main(int argc, char *argv[])
// {
//     if (argc < 3)
//     {
//         printf("Uso: %s <input.asm> <output.bin>\n", argv[0]);
//         return 1;
//     }

//     int err = assemble_file(argv[1], argv[2]);
//     if (err == 0)
//     {
//         printf("Assemblato con successo: %s -> %s\n", argv[1], argv[2]);
//     }
//     else
//     {
//         printf("Errore nell'assemblare '%s'\n", argv[1]);
//     }
//     return err;
// }

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Uso: %s <input.asm> <output.bin>\n", argv[0]);
        return 1;
    }

    int err = assemble_file(argv[1], argv[2]);
    if (err == 0)
    {
        printf("Assemblato con successo: %s -> %s\n", argv[1], argv[2]);

        // Usa i getter per accedere ai dati delle istruzioni
        const ParsedLine *parsedLines = get_parsed_lines();
        int lineUsed = get_line_used();

        // Stampa le istruzioni
        printf("\n--- Istruzioni Assemblate ---\n");
        for (int i = 0; i < lineUsed; i++)
        {
            print_instruction(&parsedLines[i].line, parsedLines[i].address);
        }
        printf("--- Fine Istruzioni ---\n");
    }
    else
    {
        printf("Errore nell'assemblare '%s'\n", argv[1]);
    }
    return err;
}