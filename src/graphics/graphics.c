#include "graphics/graphics.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Variabili statiche per tenere traccia di:
 * - finestra
 * - renderer
 */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

/**
 * Inizializza SDL, crea finestra e renderer.
 */
void graphics_init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("Emulatore CPU",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              640,
                              480,
                              SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    // Impostiamo il colore di "background" a nero
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Puliamo tutto lo schermo con il nero
    SDL_RenderClear(renderer);

    // Mostriamo sullo schermo il risultato
    SDL_RenderPresent(renderer);
}

/**
 * Disegna un singolo pixel (x,y) con colore (0xRRGGBB).
 */
void graphics_draw(uint32_t x, uint32_t y, uint32_t color)
{
    // Se le coordinate superano i limiti, potremmo ignorare oppure correggere.
    // Qui ignoriamo.
    if (x >= 640 || y >= 480)
    {
        return;
    }

    // Estraiamo i componenti R, G, B dal colore (formato 0xRRGGBB).
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    // Impostiamo il colore di disegno
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    // Disegniamo un punto
    // SDL_RenderDrawPoint() disegna 1 pixel
    SDL_RenderDrawPoint(renderer, (int)x, (int)y);

    // Non facciamo ancora il "present"; lo facciamo in graphics_update().
    // Se preferisci vedere il pixel immediatamente,
    // potresti chiamare SDL_RenderPresent(renderer) qui,
    // ma di solito si fa 1 present al frame.
}

/**
 * Aggiorna lo schermo (present).
 */
void graphics_update()
{
    SDL_RenderPresent(renderer);
}

/**
 * Rilascia le risorse e chiude SDL.
 */
void graphics_cleanup()
{
    // Distruggiamo il renderer
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    // Distruggiamo la finestra
    if (window)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    // Chiudiamo SDL
    SDL_Quit();
}
