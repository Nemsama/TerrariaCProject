#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define PIXELS_PER_BLOCK 32
#define BLOCK_SIZE PIXELS_PER_BLOCK
#define WINDOW_TITLE "Nem's Terraria"
#define FPS 60
#define FRAME_TIME (1000.0f / FPS)
#define CAMERA_SCALE 1.0f
#define CAMERA_SPEED 1.0f

#define PLAYER_ACCELERATION 0.05f // blocks per frame^2
#define PLAYER_MAX_SPEED 20.0f // blocks per second
#define PLAYER_MASS 10.0f
#define PLAYER_JUMP_SPEED 30.0f // blocks per second

#define GRAVITY 0.02f 

#endif