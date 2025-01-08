// graphics.c
#include "graphics.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <stdlib.h>

static SDL_Window *window = NULL;
static SDL_GLContext gl_context = NULL;

void graphics_init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Errore SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }

    // Crea una finestra con supporto OpenGL
    window = SDL_CreateWindow("Emulatore CPU",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              640, 480,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "Errore SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    // Crea un contesto OpenGL per la finestra
    gl_context = SDL_GL_CreateContext(window);
    if (!gl_context)
    {
        fprintf(stderr, "Errore SDL_GL_CreateContext: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    // Imposta la proiezione ortografica per il disegno 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Sostituisce gluOrtho2D: imposta una proiezione ortografica con origine in alto a sinistra
    glOrtho(0.0, 640.0, 480.0, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Imposta il colore di sfondo a nero e cancella il buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window);
}

void graphics_draw(uint32_t x, uint32_t y, uint32_t color)
{
    printf("Disegno pixel alle coordinate (%u, %u) con colore 0x%06X\n", x, y, color);

    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    // Disegna un punto in modalità immediate con OpenGL
    glColor3ub(r, g, b);
    glBegin(GL_POINTS);
    glVertex2i((GLint)x, (GLint)y);
    glEnd();

    // Aggiorna la finestra per mostrare il punto disegnato
    SDL_GL_SwapWindow(window);
}

void graphics_update()
{
    SDL_GL_SwapWindow(window);
}

void graphics_cleanup()
{
    if (gl_context)
    {
        SDL_GL_DeleteContext(gl_context);
    }
    if (window)
    {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}
