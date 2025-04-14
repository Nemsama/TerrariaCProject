#ifndef WORLD_H
#define WORLD_H

#include <time.h>

#include <common.h>
#include <blocks.h>
#include <generation.h>
#include <perlin.h>

#define WORLD_WIDTH 6400
#define WORLD_HEIGHT 1800
#define SURFACE_HEIGHT 390
#define SURFACE_UP_LIMIT 50    // how many blocks above(relative) SURFACE_HEIGHT the actual surface can be
#define SURFACE_DOWN_LIMIT -10 // how many blocks below(relative) SURFACE_HEIGHT the actual surface can be
#define SURFACE_TO_UNDERGROUND_HEIGHT 440
#define UNDERGROUND_TO_CAVERN_HEIGHT 640
#define CAVERN_TO_UNDERWORLD_HEIGHT 1590

#define WORLD_SPAWN_X WORLD_WIDTH / 2.0f
#define WORLD_SPAWN_Y SURFACE_HEIGHT - 3.0f


#define       NOISE_DEPTH     5
#define       NOISE_FREQUENCY 0.16f

#define     SURFACE_THRESHOLD 0.71f
#define UNDERGROUND_THRESHOLD 0.62f
#define      CAVERN_THRESHOLD 0.38f

typedef enum {
    BOX_BLUR,
    GAUSSIAN_BLUR
} filter_type_t;

#define MAX_WORLD_NAME_LENGTH 256

typedef block_type_t world_t[WORLD_WIDTH][WORLD_HEIGHT];

typedef enum {
    NONE,
    FLAT
} special_seed_t;

typedef struct world_flags {
    char name[MAX_WORLD_NAME_LENGTH];
    unsigned int seed;
    special_seed_t special_seed;
    Sint8 size; // 0 = small, 1 = medium, 2 = large
    Sint8 difficulty; // 0 = journey, 1 = classic, 2 = expert, 3 = master
    Sint8 world_type; // 0 = random, 1 = corruption, 2 = crimson
    Sint8 dongeon_side; // 0 = random, 1 = left, 2 = right
    // dongeon and ice biome are on one side and jungle and desert on the other
} world_flags_t;

void set_default_world_flags( world_flags_t* world_flags );
world_t* create_world( world_flags_t* world_flags );
void world_destroy( world_t* world );

block_type_t world_output_block( world_t* world, int x, int y );
void world_set_block( world_t* world, int x, int y, block_type_t type );
bool world_set_blocks( world_t* world, int start_x, int end_x, int start_y, int end_y, block_type_t type );

void world_render( world_t* world, camera_t* camera, SDL_Renderer* renderer );

void world_get_collisions( world_t* world, int left, int top, int right, int bottom, vector2_t* displacement );

#endif