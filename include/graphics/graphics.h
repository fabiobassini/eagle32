// include/graphics/graphics.h

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

/**
 * Inizializza la finestra e l'ambiente grafico.
 */
void graphics_init();

/**
 * Disegna un "pixel" o un punto nelle coordinate (x, y) con color 0xRRGGBB.
 */
void graphics_draw(uint32_t x, uint32_t y, uint32_t color);

/**
 * Aggiorna lo schermo (swap del buffer).
 */
void graphics_update();

/**
 * Rilascia le risorse grafiche (chiude finestra ecc.).
 */
void graphics_cleanup();

#endif // GRAPHICS_H
