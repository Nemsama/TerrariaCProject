#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>

#include <sprite.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Nem's Terraria"
#define FPS 60
#define FRAME_TIME (1000 / FPS)

typedef struct game {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;

    sprite_t* background;

    bool running;
} game_t;

game_t* game_init( void );

void game_run( game_t* game );

void game_quit( game_t* game );

#endif