#include "graphics/graphics.h"
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
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow(
        "Emulatore CPU",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    gl_context = SDL_GL_CreateContext(window);
    if (!gl_context)
    {
        fprintf(stderr, "SDL_GL_CreateContext Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    // Set ortho 2D (0,0 in alto a sinistra)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 640.0, 480.0, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window);
}

void graphics_draw(uint32_t x, uint32_t y, uint32_t color)
{
    // Notiamo su console
    // printf("Draw pixel (%u, %u) color=0x%06X\n", x, y, color);

    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    glColor3ub(r, g, b);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();

    // Non necessariamente occorre swap immediato,
    // potresti farlo solo in graphics_update().
    // SDL_GL_SwapWindow(window);
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
