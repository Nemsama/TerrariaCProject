#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>

#include <camera.h>
#include <sprite.h>
#include <player.h>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE "Nem's Terraria"
#define FPS 60
#define FRAME_TIME (1000 / FPS)

#define PIXELS_PER_BLOCK 32.0f
#define BLOCKS_PER_PIXEL 1/PIXELS_PER_BLOCK

#define WORLD_SPAWN_X 0.0f
#define WORLD_SPAWN_Y 0.0f
#define PLAYER_BASE_SPEED 0.3f

typedef struct game {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;

    sprite_t* background;

    bool running;

    // game objects
    camera_t* camera;
    player_t* player;
} game_t;

game_t* game_init( void );

void game_run( game_t* game );

void game_quit( game_t* game );

#endif