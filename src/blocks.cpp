#include <blocks.h>

block_t* blocks_types[BLOCK_TYPES_COUNT] = { NULL, NULL, NULL, NULL, NULL };

bool init_blocks_types( SDL_Renderer* renderer ) {
    char texture_path[256];
    const char* block_texture_names[BLOCK_TYPES_COUNT] = BLOCK_TEXTURES_NAMES;

    for ( int i = 0; i < BLOCK_TYPES_COUNT; i++ ) {
        blocks_types[i] = (block_t*)calloc( 1, sizeof(*blocks_types[i]) );
        if ( blocks_types[i] == NULL ) {
            perror("Failed to allocate memory for block from blocks_types");
            return false;
        }

        blocks_types[i]->type = (block_type_t)i;

        snprintf( texture_path, sizeof(texture_path), "%s%s%s", BLOCK_TEXTURES_PATH, block_texture_names[i], BLOCK_TEXTURES_EXT );
        blocks_types[i]->sprite = sprite_init( texture_path, renderer, 0, 0, BLOCK_SIZE, BLOCK_SIZE );
        if ( blocks_types[i]->sprite == NULL ) {
            perror("Failed to create blocks_types sprite");
            destroy_blocks_types();
            return false;
        }
    }

    return true;
}
void destroy_blocks_types( void ) {
    for ( int i = 0; i < BLOCK_TYPES_COUNT; i++ ) {
        if ( blocks_types[i] == NULL ) continue;

        sprite_destroy( blocks_types[i]->sprite );
        free( blocks_types[i] );
    }
}

void block_render( block_t* block, camera_t* camera, SDL_Renderer* renderer ) {
    if ( block == NULL || camera == NULL || renderer == NULL ) return;

    int block_screen_x, block_screen_y;
    camera_worldtoscreen_pos( camera, vector2_new( (float)block->x, (float)block->y ), &block_screen_x, &block_screen_y );
    sprite_set_pos( block->sprite, block_screen_x, block_screen_y );

    // printf("block %d %d %d %d\n", block->x, block->y, block->sprite->dest_rect.x, block->sprite->dest_rect.y);

    sprite_render( block->sprite, camera, renderer );
}
