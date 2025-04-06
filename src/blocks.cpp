#include <blocks.h>

bool init_blocks_textures( SDL_Renderer* renderer ) {
    char texture_path[256];
    const char* block_texture_names[BLOCK_TYPES_COUNT] = BLOCK_TEXTURES_NAMES;

    for ( int i = 0; i < BLOCK_TYPES_COUNT; i++ ) {
        snprintf( texture_path, sizeof(texture_path), "%s%s%s", BLOCK_TEXTURES_PATH, block_texture_names[i], BLOCK_TEXTURES_EXT );
        blocks_textures[i] = IMG_LoadTexture( renderer, texture_path );
        if ( blocks_textures[i] == NULL ) {
            perror("Failed to load block texture");
            return false;
        }
    }

    return true;
}
void destroy_blocks_textures( void ) {
    for ( int i = 0; i < BLOCK_TYPES_COUNT; i++ ) {
        if ( blocks_textures[i] != NULL ) {
            SDL_DestroyTexture( blocks_textures[i] );
            blocks_textures[i] = NULL;
        }
    }
}

void world_set_block( world_t world, int x, int y, block_type_t type ) {
    if ( !world ) return;
    if ( x < 0 || x >= WORLD_WIDTH || y < 0 || y >= WORLD_HEIGHT ) return;

    world[x][y] = type;
}
void world_set_blocks( world_t world, int x, int width, int y, int height, block_type_t type ) {
    if ( !world ) return;
    if ( x < 0 || x + width > WORLD_WIDTH || y < 0 || y + height > WORLD_HEIGHT ) return;

    for ( int i = x; i < x+width; i++ ) {
        for ( int j = y; j < y+height; j++ ) {
            world[i][j] = type;
        }
    }
}

world_t* world_init( /* world parameters */ ) {
    world_t* world = (world_t*)calloc( 1, sizeof(*world) );
    if ( world == NULL ) {
        perror("Failed to allocate memory for world");
        return NULL;
    }

    world_set_blocks( world, 0, WORLD_WIDTH , WORLD_HEIGHT/2, WORLD_HEIGHT/2, AIR );

    world_set_blocks( world, 0, WORLD_WIDTH, (WORLD_HEIGHT/2)- 1,                   1, GRASS );
    world_set_blocks( world, 0, WORLD_WIDTH, (WORLD_HEIGHT/2)-10,                   9, DIRT  );
    world_set_blocks( world, 0, WORLD_WIDTH,                   0, (WORLD_HEIGHT/2)-10, STONE );

    return world;
}
void world_destroy( world_t* world ) {
    if ( world == NULL ) return;

    free( world );
}

void block_render( block_t* block, camera_t* camera, SDL_Renderer* renderer ) {
    if ( block == NULL || camera == NULL || renderer == NULL ) return;

    camera_worldtoscreen_pos( camera, block->x, block->y, &block->sprite->dest_rect.x, &block->sprite->dest_rect.y );

    sprite_render( block->sprite, renderer );
}

void world_render( world_t* world, camera_t* camera, SDL_Renderer* renderer ) {
    
}
