#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define PIXELS_PER_BLOCK 32
#define BLOCK_SIZE PIXELS_PER_BLOCK
#define WINDOW_TITLE "Nem's Terraria"
#define FPS 60
#define FRAME_TIME (1000 / FPS)

#define WORLD_WIDTH 2048
#define WORLD_HEIGHT 1024
#define WORLD_SPAWN_X 0.0f
#define WORLD_SPAWN_Y 0.0f
#define PLAYER_BASE_SPEED 0.3f

#endif