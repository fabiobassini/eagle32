#include "io/io.h"
#include <stdio.h>

void io_init()
{
    // Se servono inizializzazioni particolari, metterle qui
    // Esempio: seriale, tastiera, ecc.
}

uint32_t io_read(uint32_t port)
{
    // Simulazione di un I/O read
    printf("[IO] Lettura da porta %u\n", port);
    // Potresti gestire switch su port per restituire vari device
    return 0;
}

void io_write(uint32_t port, uint32_t data)
{
    // Simulazione di un I/O write
    printf("[IO] Scrittura su porta %u: 0x%X\n", port, data);
    // Potresti gestire switch su port per controllare LED, suoni, ecc.
}
