// main.c
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "graphics/graphics.h"
#include "io/io.h"
#include "bus/bus.h"
#include <SDL2/SDL.h>
#include "cpu/registers.h" // Includi il modulo registri se necessario

int main()
{
    memory_init();
    io_init();
    graphics_init();

    CPU cpu;
    cpu_init(&cpu);

    // Carica il programma binario in memoria all'indirizzo 0
    FILE *f = fopen("circle.bin", "rb");
    if (f)
    {
        uint32_t instruction;
        uint32_t addr = 0;
        while (fread(&instruction, sizeof(uint32_t), 1, f) == 1)
        {
            bus_write_word(addr, instruction);
            addr += 4;
        }
        fclose(f);
    }
    else
    {
        fprintf(stderr, "Errore nell'apertura di circle.bin\n");
        return 1;
    }

    // Esegue il programma caricando ed eseguendo fino all'istruzione UNKNOWN
    cpu_execute(&cpu);

    graphics_update();

    // Ciclo di eventi per mantenere la finestra aperta e reattiva
    SDL_Event event;
    int running = 1;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }
        SDL_Delay(10);
    }

    graphics_cleanup();
    return 0;
}
