// io.c
#include "io.h"
#include <stdio.h>

void io_init()
{
    // Inizializzazione delle risorse I/O se necessaria
}

uint32_t io_read(uint32_t port)
{
    // Simulazione di una lettura da porta I/O
    printf("Lettura da porta I/O %u\n", port);
    return 0;
}

void io_write(uint32_t port, uint32_t data)
{
    // Simulazione di una scrittura su porta I/O
    printf("Scrittura su porta I/O %u: dato %u\n", port, data);
}
