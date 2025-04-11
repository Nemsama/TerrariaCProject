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

block_type_t world_output_block( world_t* world, int x, int y ) {
    if ( !world ) return INVALID;
    if ( x < 0 || x >= WORLD_WIDTH || y < 0 || y >= WORLD_HEIGHT ) return INVALID;

    return (*world)[x][y];
}

void world_set_block( world_t* world, int x, int y, block_type_t type ) {
    if ( !world ) return;
    if ( x < 0 || x >= WORLD_WIDTH || y < 0 || y >= WORLD_HEIGHT ) return;

    (*world)[x][y] = type;
}
void world_set_blocks( world_t* world, int x, int width, int y, int height, block_type_t type ) {
    if ( !world ) return;
    if ( x < 0 || x + width > WORLD_WIDTH || y < 0 || y + height > WORLD_HEIGHT ) return;

    for ( int i = x; i < x+width; i++ ) {
        for ( int j = y; j < y+height; j++ ) {
            (*world)[i][j] = type;
        }
    }
}

world_t* world_init( /* world parameters */ ) {
    world_t* world = (world_t*)calloc( 1, sizeof(*world) );
    if ( world == NULL ) {
        perror("Failed to allocate memory for world");
        return NULL;
    }

    world_set_blocks( world, 0, WORLD_WIDTH ,                 0,     WORLD_HEIGHT/2, AIR   );

    world_set_blocks( world, 0, WORLD_WIDTH,  WORLD_HEIGHT/2   ,                  1, GRASS );
    world_set_blocks( world, 0, WORLD_WIDTH, (WORLD_HEIGHT/2)+1,                  7, DIRT  );
    world_set_blocks( world, 0, WORLD_WIDTH, (WORLD_HEIGHT/2)+8, (WORLD_HEIGHT/2)-8, STONE );

    // lil stone
    world_set_blocks( world, WORLD_WIDTH/2+4, 3, WORLD_HEIGHT/2  , 1, DIRT  );
    world_set_blocks( world, WORLD_WIDTH/2+4, 3, WORLD_HEIGHT/2-1, 1, STONE );
    world_set_blocks( world, WORLD_WIDTH/2+4, 2, WORLD_HEIGHT/2-2, 1, STONE );

    world_set_blocks( world, WORLD_WIDTH/2-6, 2, WORLD_HEIGHT/2-8, 1, STONE );
    world_set_blocks( world, WORLD_WIDTH/2-6, 3, WORLD_HEIGHT/2-7, 1, STONE );

    return world;
}
void world_destroy( world_t* world ) {
    if ( world == NULL ) return;

    free( world );
}

void block_render( block_t* block, camera_t* camera, SDL_Renderer* renderer ) {
    if ( block == NULL || camera == NULL || renderer == NULL ) return;

    int block_screen_x, block_screen_y;
    camera_worldtoscreen_pos( camera, vector2_new( (float)block->x, (float)block->y ), &block_screen_x, &block_screen_y );
    sprite_set_pos( block->sprite, block_screen_x, block_screen_y );

    // printf("block %d %d %d %d\n", block->x, block->y, block->sprite->dest_rect.x, block->sprite->dest_rect.y);

    sprite_render( block->sprite, renderer );
}

void world_render( world_t* world, camera_t* camera, SDL_Renderer* renderer ) {
    if ( world == NULL || camera == NULL || renderer == NULL ) return;
    block_type_t block_type;
    block_t* block;
    vector2_t camera_pos;
    vector2_t camera_shape;
    camera_get_pos( camera, &camera_pos );
    camera_get_shape( camera, &camera_shape );

    for ( int x = (int)camera_pos.x; x < camera_pos.x + camera_shape.x; x++ ) {
        for ( int y = (int)camera_pos.y; y < camera_pos.y + camera_shape.y; y++ ) {
            if ( x < 0 || x >= WORLD_WIDTH || y < 0 || y >= WORLD_HEIGHT ) continue;

            block_type = (*world)[x][y];
            
            if ( block_type == AIR ) continue;
            
            block = blocks_types[block_type];
            if ( block == NULL ) {
                fprintf(stderr, "Block type %d not found\n", block_type);
                continue;
            }

            block->x = x;
            block->y = y;
            block_render( block, camera, renderer );
        }
    }
}

void world_get_collisions( world_t* world, int left, int top, int right, int bottom, vector2_t* displacement ) {
    if ( !world || !displacement ) return;

    for ( int i=left+1; i<=right-1; i++ ) {
        if ( AIR == world_output_block( world, i, top ) ) continue;
        
        // printf("collision against %d in pos (%d, %d)\n", world_output_block( world, i, colliding_top ), i, colliding_top);
        vector2_add_to( displacement, vector2_new( 0.0f, 1.0f ) );
    }
    for ( int i=left+1; i<=right-1; i++ ) {
        if ( AIR == world_output_block( world, i, bottom ) ) continue;
        
        // printf("collision against %d in pos (%d, %d)\n", world_output_block( world, i, colliding_bottom ), i, colliding_bottom);
        vector2_add_to( displacement, vector2_new( 0.0f, -1.0f ) );
    }
    for ( int j=top+1; j<=bottom-1; j++ ) {
        if ( AIR == world_output_block( world, left, j ) ) continue;
        
        // printf("collision against %d in pos (%d, %d)\n", world_output_block( world, colliding_left, j ), colliding_left, j);
        vector2_add_to( displacement, vector2_new( 1.0f, 0.0f ) );
    }
    for ( int j=top+1; j<=bottom-1; j++ ) {
        if ( AIR == world_output_block( world, right, j ) ) continue;
        
        // printf("collision against %d in pos (%d, %d)\n", world_output_block( world, colliding_right, j ), colliding_right, j);
        vector2_add_to( displacement, vector2_new( -1.0f, 0.0f ) );
    }
}
