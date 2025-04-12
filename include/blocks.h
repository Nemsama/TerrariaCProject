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

bool init_blocks_types( SDL_Renderer* renderer );
void destroy_blocks_types( void );

void block_render( block_t* block, camera_t* camera, SDL_Renderer* renderer );

#endif