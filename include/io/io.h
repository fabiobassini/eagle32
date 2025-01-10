// include/io/io.h

#ifndef IO_H
#define IO_H

#include <stdint.h>

/**
 * Inizializza il sistema di I/O.
 */
void io_init();

/**
 * Legge da una "porta" di I/O, simulata.
 * @param port: numero di porta (decidi tu la mappa)
 * @return valore a 32 bit
 */
uint32_t io_read(uint32_t port);

/**
 * Scrive un valore su una porta di I/O.
 * @param port: numero di porta
 * @param data: valore a 32 bit
 */
void io_write(uint32_t port, uint32_t data);

#endif // IO_H
