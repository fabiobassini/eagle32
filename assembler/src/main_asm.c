#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

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
    }
    else
    {
        printf("Errore nell'assemblare '%s'\n", argv[1]);
    }
    return err;
}
