// io.h
#ifndef IO_H
#define IO_H

#include <stdint.h>

void io_init();
uint32_t io_read(uint32_t port);
void io_write(uint32_t port, uint32_t data);

#endif
