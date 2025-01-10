// src/main.c

#include "cpu/cpu.h"
#include "memory/memory.h"
#include "graphics/graphics.h"
#include "io/io.h"
#include "bus/bus.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Funzione di caricamento del programma binario in memoria.
 * Ritorna la dimensione caricata in byte.
 */
static size_t load_program_into_memory(const char *filename)
{
    FILE *f = fopen(filename, "rb");
    if (!f)
    {
        fprintf(stderr, "Impossibile aprire il file '%s'\n", filename);
        return 0;
    }

    size_t bytes_read = 0;
    uint32_t instruction;
    uint32_t addr = 0;

    // while (fread(&instruction, sizeof(uint32_t), 1, f) == 1)
    // {
    //     bus_write_word(addr, instruction);
    //     addr += 4;
    //     printf("LETTI 4 BYTE: 0x%08X -> bus_write_word(0x%X)\n", instruction, addr - 4);

    //     bytes_read += sizeof(uint32_t);
    // }

    while (fread(&instruction, sizeof(uint32_t), 1, f) == 1)
    {
        uint32_t swapped = ((instruction & 0xFF) << 24) | ((instruction & 0xFF00) << 8) | ((instruction & 0xFF0000) >> 8) | ((instruction & 0xFF000000) >> 24);

        printf("SWAP 0x%08X => 0x%08X\n", instruction, swapped);
        bus_write_word(addr, swapped);
        addr += 4;

        bytes_read += sizeof(uint32_t);
    }

    fclose(f);
    return bytes_read;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Utilizzo: %s <programma.bin>\n", argv[0]);
        return 1;
    }

    // Inizializza memory, I/O, graphics
    memory_init();
    io_init();
    graphics_init();

    // Carica programma
    size_t prog_size = load_program_into_memory(argv[1]);
    if (prog_size == 0)
    {
        // Errore
        graphics_cleanup();
        return 1;
    }
    printf("Caricati %zu byte dal programma '%s'\n", prog_size, argv[1]);

    // Inizializza CPU
    CPU cpu;
    cpu_init(&cpu);

    // Esegui il programma
    cpu_run(&cpu);

    // Al termine, facciamo un ultimo update grafico
    graphics_update();

    // Teniamo aperta la finestra finché l'utente non chiude
    SDL_Event event;
    int done = 0;
    while (!done)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                done = 1;
            }
        }
        SDL_Delay(16);
    }

    graphics_cleanup();
    return 0;
}
