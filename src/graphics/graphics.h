// graphics.h
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

void graphics_init();
void graphics_draw(uint32_t x, uint32_t y, uint32_t color);
void graphics_update();
void graphics_cleanup();

#endif
