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

void create_world_base( world_t* world, world_flags_t* world_flags ) {
    if ( world == NULL || world_flags == NULL ) return;

    // generate world height
    int    left_side[WORLD_WIDTH/2];
    int world_height[WORLD_WIDTH];

    generate_world_height( world_height + WORLD_WIDTH/2, WORLD_WIDTH/2, 0, SURFACE_UP_LIMIT, SURFACE_DOWN_LIMIT );
    generate_world_height( left_side                   , WORLD_WIDTH/2, 0, SURFACE_UP_LIMIT, SURFACE_DOWN_LIMIT );
    flip_terrain( world_height, left_side, WORLD_WIDTH/2 );
    world_height[WORLD_WIDTH/2] = 0;

    convert_terrain_to_world_height( world_height, WORLD_WIDTH, SURFACE_HEIGHT );

    // generate blocks
    for ( int x = 0; x < WORLD_WIDTH; x++ ) {
        world_set_blocks( world, x, x,                            0,              world_height[x]-1, AIR   );
        world_set_block ( world,    x,                                            world_height[x]  , GRASS );
        world_set_blocks( world, x, x,            world_height[x]+1, UNDERGROUND_TO_CAVERN_HEIGHT-1, DIRT  );
        world_set_blocks( world, x, x, UNDERGROUND_TO_CAVERN_HEIGHT,  CAVERN_TO_UNDERWORLD_HEIGHT-1, STONE );
        world_set_blocks( world, x, x,  CAVERN_TO_UNDERWORLD_HEIGHT,                 WORLD_HEIGHT-1, AIR   );
    }
}

void paint_surface_perlin( Uint8 ** world_mask, world_flags_t* world_flags ) {
    if ( world_flags == NULL ) return;

    float noise;

    for ( int x = 0; x < WORLD_WIDTH; x++ ) {
        for ( int y = 0; y < SURFACE_TO_UNDERGROUND_HEIGHT; y++ ) {

            noise = perlin2d( x, y, NOISE_FREQUENCY, NOISE_DEPTH, world_flags->seed );
            if ( noise > SURFACE_THRESHOLD ) {
                world_mask[x][y] = 1;
            }
            else {
                world_mask[x][y] = 0;
            }
        }
    }
}
void paint_underground_perlin( Uint8 ** world_mask, world_flags_t* world_flags ) {
    if ( world_flags == NULL ) return;

    float noise;

    for ( int x = 0; x < WORLD_WIDTH; x++ ) {
        for ( int y = SURFACE_TO_UNDERGROUND_HEIGHT; y < UNDERGROUND_TO_CAVERN_HEIGHT; y++ ) {

            noise = perlin2d( x, y, NOISE_FREQUENCY, NOISE_DEPTH, world_flags->seed );
            if ( noise > UNDERGROUND_THRESHOLD ) {
                world_mask[x][y] = 1;
            }
            else {
                world_mask[x][y] = 0;
            }
        }
    }
}
void paint_cavern_perlin( Uint8 ** world_mask, world_flags_t* world_flags ) {
    if ( world_flags == NULL ) return;

    float noise;

    for ( int x = 0; x < WORLD_WIDTH; x++ ) {
        for ( int y = UNDERGROUND_TO_CAVERN_HEIGHT; y < CAVERN_TO_UNDERWORLD_HEIGHT; y++ ) {

            noise = perlin2d( x, y, NOISE_FREQUENCY, NOISE_DEPTH, world_flags->seed );
            if ( noise > CAVERN_THRESHOLD ) {
                world_mask[x][y] = 1;
            }
            else {
                world_mask[x][y] = 0;
            }
        }
    }
}

void smooth_transition( Uint8 ** world_mask, world_flags_t* world_flags, int transition_height, int offset, float start_threshold, float end_threshold ) {
    if ( world_flags == NULL ) return;

    float threshold = start_threshold;
    float threshold_step = (end_threshold - start_threshold) / ( 2.0f * offset );

    float noise;

    for ( int y = transition_height - offset; y < transition_height + offset; y++ ) {
        for ( int x = 0; x < WORLD_WIDTH; x++) {
            noise = perlin2d( x, y, NOISE_FREQUENCY, NOISE_DEPTH, world_flags->seed );

            if ( noise > threshold ) {
                world_mask[x][y] = 1;
            }
            else {
                world_mask[x][y] = 0;
            }
        }
    
        threshold += threshold_step;
    }
}

float get_mask_value( Uint8 ** world_mask, int x, int y ) {
    if ( x < 0 || x >= WORLD_WIDTH || y < 0 || y >= WORLD_HEIGHT ) return 0.5f;

    return (float)world_mask[x][y];
}
float get_box_filter_value( Uint8 ** world_mask, int x, int y ) {
    float sum = 0;

    for ( int i = x-1; i <= x+1; i++ ) {
        for ( int j = y-1; j <= y+1; j++ ) {
            sum += get_mask_value( world_mask, i, j );
        }
    }

    return sum/9;
}
float gaussian_weight( int x, int y, int i, int j ) {
    if ( i == x && j == y ) return 4.0f;
    else if ( fabs( i-x ) + fabs( j-y ) == 1 ) return 2.0f;
    else return 1.0f;
}
float get_gaussian_filter_value( Uint8 ** world_mask, int x, int y ) {
    float sum = 0;
    float weight_sum = 0;

    for ( int i = x-1; i <= x+1; i++ ) {
        for ( int j = y-1; j <= y+1; j++ ) {
            float weight = gaussian_weight( x, y, i, j );
            sum += get_mask_value( world_mask, i, j ) * weight;
            weight_sum += weight;
        }
    }

    return sum/weight_sum;
}
void filter_mask_and_paint_world( world_t* world, Uint8 ** world_mask, filter_type_t filter_type ) {
    if ( world == NULL ) return;
    float filter_value;

    for ( int i = 0; i < WORLD_WIDTH; i++ ) {
        for ( int j = 0; j < WORLD_HEIGHT; j++ ) {
            if ( AIR == world_output_block( world, i, j ) ) continue;

            switch ( filter_type ) {
                case BOX_BLUR:
                    filter_value = get_box_filter_value( world_mask, i, j );
                    break;
                case GAUSSIAN_BLUR:
                    filter_value = get_gaussian_filter_value( world_mask, i, j );
                    break;
                default:
                    perror("Unknown filter type");
                    return;
                    break;
            }

            if ( filter_value >= 0.5f ) {
                world_set_block( world, i, j, STONE );
            }
            else {
                if ( GRASS == world_output_block( world, i, j ) ) continue;
                else world_set_block( world, i, j, DIRT );
            }
        }
    }
}
void paint_world_perlin( world_t* world, world_flags_t* world_flags ) {
    if ( world == NULL || world_flags == NULL ) return;

    Uint8 ** world_mask = (Uint8**)calloc( WORLD_WIDTH, sizeof(Uint8*) );
    for ( int i = 0; i < WORLD_WIDTH; i++ ) {
        world_mask[i] = (Uint8*)calloc( WORLD_HEIGHT, sizeof(Uint8) );
    }

    printf("    Painting surface stone...\n");
    paint_surface_perlin( world_mask, world_flags );
    printf("    Painting underground stone...\n");
    paint_underground_perlin( world_mask, world_flags );
    printf("    Painting cavern dirt...\n");
    paint_cavern_perlin( world_mask, world_flags );

    printf("    Smoothing transitions...\n");
    smooth_transition( world_mask, world_flags, SURFACE_TO_UNDERGROUND_HEIGHT, 4,     SURFACE_THRESHOLD, UNDERGROUND_THRESHOLD );
    smooth_transition( world_mask, world_flags,  UNDERGROUND_TO_CAVERN_HEIGHT, 6, UNDERGROUND_THRESHOLD,      CAVERN_THRESHOLD );

    printf("    Blurring lonely rocks...\n");
    filter_mask_and_paint_world( world, world_mask, GAUSSIAN_BLUR );
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

    switch ( world_flags->special_seed ) {
        case NONE:
            create_world_base( world, world_flags );
            break;
        case FLAT:
            create_flat_base( world );
            break;
        default:
            perror("Unknown special seed");
            break;
    }
}
void paint_world( world_t* world, world_flags_t* world_flags ) {
    if ( world == NULL || world_flags == NULL ) return;

    switch ( world_flags->special_seed ) {
        case NONE:
            paint_world_perlin( world, world_flags );
            break;
        case FLAT:
            // nothing to do
            break;
        default:
            break;
    }
}
void world_generate_caves( world_t* world, world_flags_t* world_flags ) {
    if ( world == NULL || world_flags == NULL ) return;
    
}
void world_generate_biomes( world_t* world, world_flags_t* world_flags ) {
    if ( world == NULL || world_flags == NULL ) return;

    
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

    if ( world_flags->special_seed == NONE ) {
        srand( world_flags->seed );
    }

    printf("Drawing surface line...\n");
    world_generate_base( world, world_flags );

    printf("Painting world...\n");
    paint_world( world, world_flags );

    printf("Digging holes...\n");
    world_generate_caves( world, world_flags );

    printf("Adding variety...\n");
    world_generate_biomes( world, world_flags );

    printf("Giving you things to loot...\n");
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
