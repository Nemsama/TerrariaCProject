#ifndef BLOCKS_H
#define BLOCKS_H

#include <common.h>
#include <camera.h>
#include <sprite.h>

#define BLOCK_TYPES_COUNT 6
typedef enum {
    AIR,
    DIRT,
    GRASS,
    INVALID,
    SAND,
    STONE
} block_type_t;
SDL_Texture* blocks_textures[BLOCK_TYPES_COUNT]; // textures for each block type
#define BLOCK_TEXTURES_PATH "assets/blocks/"
#define BLOCK_TEXTURES_EXT  ".png"
#define BLOCK_TEXTURES_NAMES { \
    "air", \
    "dirt", \
    "grass", \
    "invalid", \
    "sand", \
    "stone" \
}

typedef struct block {
    block_type_t type;
    sprite_t* sprite;  // block sprite
    int x, y;          // world position
} block_t;

typedef block_type_t world_t[WORLD_WIDTH][WORLD_HEIGHT];

bool init_blocks_textures( SDL_Renderer* renderer );
void destroy_blocks_textures( void );

world_t* world_init( /* world parameters */ );
void world_destroy( world_t* world );

void world_set_block( world_t* world, int x, int y, block_type_t type );
void world_set_blocks( world_t* world, int x, int width, int y, int height, block_type_t type );

void world_render( world_t* world, camera_t* camera, SDL_Renderer* renderer );
void block_render( block_t* block, camera_t* camera, SDL_Renderer* renderer );

#endif