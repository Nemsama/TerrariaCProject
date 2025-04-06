#ifndef GAME_H
#define GAME_H

#include <common.h>

#include <camera.h>
#include <sprite.h>
#include <player.h>

typedef struct game {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;

    sprite_t* background;

    bool running;
    int frame_count;

    // game objects
    camera_t* camera;
    player_t* player;
} game_t;

game_t* game_init( void );

void game_run( game_t* game );

void game_quit( game_t* game );

#endif