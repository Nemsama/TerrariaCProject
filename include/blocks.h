#ifndef BLOCKS_H
#define BLOCKS_H

#include <common.h>
#include <camera.h>
#include <sprite.h>

#define BLOCK_TYPES_COUNT 5
typedef enum {
    AIR,
    DIRT,
    GRASS,
    INVALID,
    STONE
} block_type_t;

#define BLOCK_TEXTURES_PATH "assets/blocks/"
#define BLOCK_TEXTURES_NAMES { \
    "air", \
    "dirt", \
    "grass", \
    "invalid", \
    "stone" \
}
#define BLOCK_TEXTURES_EXT  ".png"

typedef struct block {
    block_type_t type;
    sprite_t* sprite;  // block sprite
    int x, y;          // world position
} block_t;

extern block_t* blocks_types[]; // block struct for each block type

typedef block_type_t world_t[WORLD_WIDTH][WORLD_HEIGHT];

bool init_blocks_types( SDL_Renderer* renderer );
void destroy_blocks_types( void );

world_t* world_init( /* world parameters */ );
void world_destroy( world_t* world );

void world_set_block( world_t* world, int x, int y, block_type_t type );
void world_set_blocks( world_t* world, int x, int width, int y, int height, block_type_t type );

void world_render( world_t* world, camera_t* camera, SDL_Renderer* renderer );
void block_render( block_t* block, camera_t* camera, SDL_Renderer* renderer );

#endif