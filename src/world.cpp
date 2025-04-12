#include <world.h>


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
bool world_set_blocks( world_t* world, int start_x, int end_x, int start_y, int end_y, block_type_t type ) {
    if ( !world ) return false;
    if ( start_x < 0 || end_x >= WORLD_WIDTH || start_y < 0 || end_y >= WORLD_HEIGHT ) return false;

    for ( int x = start_x; x <= end_x; x++ ) {
        for ( int y = start_y; y <= end_y; y++ ) {
            (*world)[x][y] = type;
        }
    }

    return true;
}

void world_destroy( world_t* world ) {
    if ( world == NULL ) return;

    free( world );
}

void create_flat_base( world_t* world ) {
    if ( world == NULL ) return;

    world_set_blocks( world, 
                      0, WORLD_WIDTH-1, 
                      0, SURFACE_HEIGHT-1, 
                      AIR );

    world_set_blocks( world, 
                      0, WORLD_WIDTH-1, 
                      SURFACE_HEIGHT, SURFACE_HEIGHT, 
                      GRASS );
    
    world_set_blocks( world, 
                      0, WORLD_WIDTH-1, 
                      SURFACE_HEIGHT+1, SURFACE_TO_UNDERGROUND_HEIGHT-1,
                      DIRT  );
    
    world_set_blocks( world, 
                      0, WORLD_WIDTH-1, 
                      SURFACE_TO_UNDERGROUND_HEIGHT, CAVERN_TO_UNDERWORLD_HEIGHT-1, 
                      STONE );
    
    world_set_blocks( world,
                      0, WORLD_WIDTH-1, 
                      CAVERN_TO_UNDERWORLD_HEIGHT, WORLD_HEIGHT-1, 
                      AIR );
    
}
void generate_flat_structures( world_t* world ) {
    if ( world == NULL ) return;

    // lil stone
    world_set_blocks( world, WORLD_SPAWN_X+4, WORLD_SPAWN_X+6, SURFACE_HEIGHT  , SURFACE_HEIGHT  , DIRT  );
    world_set_blocks( world, WORLD_SPAWN_X+4, WORLD_SPAWN_X+6, SURFACE_HEIGHT-1, SURFACE_HEIGHT-1, STONE );
    world_set_blocks( world, WORLD_SPAWN_X+4, WORLD_SPAWN_X+5, SURFACE_HEIGHT-2, SURFACE_HEIGHT-2, STONE );

    world_set_blocks( world, WORLD_SPAWN_X-6, WORLD_SPAWN_X-5, SURFACE_HEIGHT-8, SURFACE_HEIGHT-8, STONE );
    world_set_blocks( world, WORLD_SPAWN_X-6, WORLD_SPAWN_X-4, SURFACE_HEIGHT-7, SURFACE_HEIGHT-7, STONE );
}

// space        height = 60
// surface      height = 380
// underground  height = 200
// cavern       height = 950
// underworld   height = 210
// world(total) height = 1800

void set_default_world_flags( world_flags_t* world_flags ) {
    if ( world_flags == NULL ) return;

    snprintf( world_flags->name, MAX_WORLD_NAME_LENGTH, "New World" );
    world_flags->seed         = (unsigned int)time(NULL);
    world_flags->special_seed = NONE;
    world_flags->size         = 1; // medium
    world_flags->difficulty   = 1; // classic
    world_flags->world_type   = 0; // random
    world_flags->dongeon_side = 0; // random
}

void world_generate_base( world_t* world, world_flags_t* world_flags ) {
    if ( world == NULL || world_flags == NULL ) return;

    if ( world_flags->special_seed == FLAT ) {
        return create_flat_base( world );
    }
}
void paint_world( world_t* world, world_flags_t* world_flags ) {
    if ( world == NULL || world_flags == NULL ) return;

    if ( world_flags->special_seed == FLAT ) {
        return;
    }
}
void world_generate_caverns( world_t* world, world_flags_t* world_flags ) {
    if ( world == NULL || world_flags == NULL ) return;

    if ( world_flags->special_seed == FLAT ) {
        return;
    }
}
void world_generate_biomes( world_t* world, world_flags_t* world_flags ) {
    if ( world == NULL || world_flags == NULL ) return;

    if ( world_flags->special_seed == FLAT ) {
        return;
    }
}
void world_generate_structures( world_t* world, world_flags_t* world_flags ) {
    if ( world == NULL || world_flags == NULL ) return;

    if ( world_flags->special_seed == FLAT ) {
        generate_flat_structures( world );
        return;
    }
}
world_t* create_world( world_flags_t* world_flags ) {
    world_t* world = (world_t*)calloc( 1, sizeof(*world) );
    if ( world == NULL ) {
        perror("Failed to allocate memory for world");
        return NULL;
    }

    world_generate_base( world, world_flags );

    paint_world( world, world_flags );

    world_generate_caverns( world, world_flags );

    world_generate_biomes( world, world_flags );

    world_generate_structures( world, world_flags );

    return world;
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
            if ( x < 0 || x >= WORLD_WIDTH || y < 0 || y >= WORLD_HEIGHT ) {
                block_type = INVALID;
            }
            else {
                block_type = (*world)[x][y];
            }

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
