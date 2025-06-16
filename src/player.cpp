#include <player.h>

SDL_Texture* inventory_slot_texture;
SDL_Rect  slot_src_rect = { 0, 0, 100, 100 };
SDL_Rect slot_dest_rect = { 0, 0, 60, 60 };

// entity->jump_speed = jump_speed / FPS; // convert to blocks per frame
player_t* player_init( sprite_t* sprite, vector2_t position, float acceleration, float max_speed /* in blocks per seconds */, float jump_speed ) {
    player_t* player = (player_t*)calloc( 1, sizeof(*player) );
    if ( player == NULL ) {
        perror("Failed to allocate memory for player structure");
        return NULL;
    }

    player->entity = entity_init( sprite, position );
    if ( player->entity == NULL ) {
        perror("Failed to create entity for player");
        return NULL;
    }

    player->mov_acceleration = acceleration;
    player->max_speed = max_speed / FPS;
    player->jump_speed = jump_speed / FPS;

    // init empty inventory
    for ( int i = 0; i < PLAYER_INVENTORY_HEIGHT * PLAYER_INVENTORY_WIDTH; i++ ) {
        player->inventory[i] = ITEM_EMPTY_SLOT;
    }
    player->hand_index = 0;
    player->hand_item = player->inventory[player->hand_index];

    return player;
}
void init_inventory_slot_texture( SDL_Renderer* renderer ) {
    SDL_Surface* surface = IMG_Load( INVENTORY_SLOT_PATH );
    if ( surface == NULL ) {
        perror("Failed to load texture for inventory slot");
        inventory_slot_texture = NULL;
    }
    else {
        inventory_slot_texture = SDL_CreateTextureFromSurface( renderer, surface );
        SDL_FreeSurface( surface );
        if ( inventory_slot_texture == NULL ) {
            perror("Failed to create texture from surface for inventory slot");
        }
    }
}

void player_destroy( player_t* player ) {
    if ( player == NULL ) return;
    entity_destroy( player->entity, DESTROY_TEXTURE );
    free( player );
}
void player_destroy_inventory( player_t* player ) {
    if ( player == NULL ) return;
    for ( int i = 0; i < PLAYER_INVENTORY_WIDTH; i++ ) {
        item_destroy( player->inventory[i], DESTROY_TEXTURE );
    }
}
void destroy_inventory_slot_texture( void ) {
    if ( inventory_slot_texture ) SDL_DestroyTexture( inventory_slot_texture );
}

void god_flight( player_t* player, const Uint8* keystate ) {
    float player_movement_x = 0.0f;
    float player_movement_y = 0.0f;

    if ( keystate[SDL_SCANCODE_A] ) {
        player_movement_x -= player->mov_acceleration;
    }
    if ( keystate[SDL_SCANCODE_D] ) {
        player_movement_x += player->mov_acceleration;
    }
    if ( keystate[SDL_SCANCODE_W] ) {
        player_movement_y -= player->mov_acceleration;
    }
    if ( keystate[SDL_SCANCODE_S] ) {
        player_movement_y += player->mov_acceleration;
    }
    vector2_add_to( &player->entity->world_rect.position, vector2_new( player_movement_x, player_movement_y ) );
}
void gravity_scum( player_t* player, const Uint8* keystate ) {
    float player_movement_x = 0.0f;
    float player_movement_y = 0.0f;

    if ( keystate[SDL_SCANCODE_A] ) {
        player_movement_x -= running_force( vector2_get_x( player->entity->velocity ), -player->mov_acceleration, -player->max_speed );
    }
    if ( keystate[SDL_SCANCODE_D] ) {
        player_movement_x += running_force( vector2_get_x( player->entity->velocity ), player->mov_acceleration, player->max_speed );
    }
    if ( 0 == player_movement_x ) {
        if ( fabs( vector2_get_x( player->entity->velocity ) ) < 0.04f ) {
            player->entity->velocity.x = 0.0f;
        }
        player_movement_x = -sign( vector2_get_x( player->entity->velocity ) ) * player->mov_acceleration/2.0f;
    }

    if ( keystate[SDL_SCANCODE_W] && player->entity->is_grounded ) {
        entity_impulse( player->entity, vector2_new( 0.0f, -player->jump_speed ) );
    }

    
    entity_add_force( player->entity, vector2_new( player_movement_x, player_movement_y + GRAVITY ) );
}

void item_selection( player_t* player, const Uint8* keystate ) {
    // SDL scancodes for 1 to 0 (1,2,...9,0) are 30 to 39
    for ( SDL_Scancode scancode = SDL_SCANCODE_0; scancode >= SDL_SCANCODE_1; scancode = (SDL_Scancode)(scancode-1) ) {
        if ( keystate[scancode] ) {
            player->hand_index = scancode - SDL_SCANCODE_1;
            player->hand_item = player->inventory[player->hand_index];
        }
    }
}
    
void player_handle_controls( player_t* player, const Uint8* keystate ) {
    if ( !player ) return;

    gravity_scum( player, keystate );
    // god_flight( player, keystate );

    item_selection( player, keystate );
}

bool player_loot_item( player_t* player, item_t* item ) {
    if ( player == NULL || item == NULL ) return false;
    printf("looted item "); item_print( item ); puts("");

    int remaining = -1;
    int i = 0;
    while ( remaining != 0 ) {
        if ( i == PLAYER_INVENTORY_SIZE ) return false;
        if ( item_is_empty( player->inventory[i] ) ) {
            player->inventory[i] = item_copy( item );
            item->count = 0;
            remaining = 0;
        }
        else {
            remaining = item_group( player->inventory[i], item );
        }

        i++;
    }
    
    return true;
}
void player_check_for_loot( player_t* player ) {
    if ( player == NULL ) return;

    item_t* looted_item;
    // item_list_print( loaded_items ); puts(" (before checking collisions)");
    item_list_grab_first_verifying( loaded_items, item_is_in_pickup_range, player->entity, &looted_item, NO_EXCEPTION );
    // item_list_print( loaded_items ); puts(" (after checking collision)");
    if ( looted_item != ITEM_EMPTY_SLOT ) {
        // item_list_print( loaded_items ); puts(" (before looting)");
        if ( player_loot_item( player, looted_item ) ) { // item completely looted
            // printf("item "); item_print( looted_item ); printf(" fully looted, destroying it"); puts("");
            // item_list_print( loaded_items ); puts(" (before removing item)");
            loaded_items = item_list_remove( loaded_items, looted_item );
            // item_list_print( loaded_items ); puts(" (before destroying item)");
            item_destroy( looted_item, KEEP_TEXTURE ); // TODO : modify the code to allow texture destruction for non-block items
        }
        // item_list_print( loaded_items ); puts(" (after looting)");
    }
}

void player_update( player_t* player, const Uint8* keystate, world_t* world ) {
    if ( !player ) return;

    player_handle_controls( player, keystate );

    entity_animate_movement( player->entity );

    entity_apply_force( player->entity );

    // printf("player mov_force: %f\n", running_force( vector2_get_x( player->entity->velocity ), player->mov_acceleration, player->max_speed ) );
    // printf("player     speed: " ); vector2_print( player->entity->velocity ); puts("");
    // printf("player       pos: "); vector2_print( player->entity->world_rect.position ); puts("");

    // prevent_world_exit( &player->entity->world_rect );
    entity_apply_collisions( player->entity, world );

    player_check_for_loot( player );
}

void dig_block( world_t* world, int x, int y ) {
    block_type_t block_type = world_output_block( world, x, y );
    if ( block_type == AIR ) return;
    world_set_block( world, x, y, AIR );
    printf("digging block %d at %d %d", (int)block_type, x, y ); puts("");

    block_t* block = blocks_types[block_type];
    sprite_t* sprite = sprite_copy( block->sprite );
    sprite_set_scale( sprite, BLOCK_ITEM_SCALE );
    char name[128];
    block_get_name( block_type, name );

    loaded_items = item_list_add_new( loaded_items, sprite, vector2_new( (float)x + BLOCK_ITEM_SCALE/2.0f, (float)y + BLOCK_ITEM_SCALE/2.0f ), name, true, 1, false );
}
void place_block( player_t* player, world_t* world, int block_x, int block_y ) {
    if ( world_output_block( world, block_x, block_y ) == AIR ) {
        // TODO : condition on item type (if not a block can't place it, maybe put this condition inside player_left_click)
        world_set_block( world, block_x, block_y, block_get_type( player->hand_item->name ) );
        item_remove_one( player->hand_item );
        if ( item_get_count( player->hand_item ) <= 0 ) {
            item_destroy( player->hand_item, KEEP_TEXTURE );
            player->inventory[player->hand_index] = ITEM_EMPTY_SLOT;
            player->hand_item = player->inventory[player->hand_index];
        }
    }
}
void player_left_click( player_t* player, camera_t* camera, vector2_t world_pos, world_t* world ) {
    if ( !player || !camera || !world ) return;
    
    int block_x = (int)vector2_get_x( world_pos );
    int block_y = (int)vector2_get_y( world_pos );

    if ( player->hand_item == ITEM_EMPTY_SLOT ) dig_block( world, block_x, block_y );
    else                                      place_block( player, world, block_x, block_y );
}

void player_render( player_t* player, camera_t* camera, SDL_Renderer* renderer ) {
    entity_render( player->entity, camera, renderer );
}

void render_inventory_slot( int x, int y, SDL_Renderer* renderer ) {
    if ( renderer == NULL || inventory_slot_texture == NULL ) return;

    slot_dest_rect.x = x - slot_dest_rect.w/2;
    slot_dest_rect.y = y - slot_dest_rect.h/2;

    SDL_RenderCopy( renderer, inventory_slot_texture, &slot_src_rect, &slot_dest_rect );
}
void render_main_hand_slot( int x, int y, SDL_Renderer* renderer ) {
    if ( renderer == NULL || inventory_slot_texture == NULL ) return;

    slot_dest_rect.x = x - slot_dest_rect.w/2;
    slot_dest_rect.y = y - slot_dest_rect.h/2;

    SDL_SetTextureColorMod( inventory_slot_texture, 100, 100, 255 );
    SDL_RenderCopy( renderer, inventory_slot_texture, &slot_src_rect, &slot_dest_rect );
    SDL_SetTextureColorMod( inventory_slot_texture, 255, 255, 255 );
}
void render_hotbar( player_t* player, SDL_Renderer* renderer ) {
    int y = slot_dest_rect.h/2;
    for ( int i = 0; i < PLAYER_INVENTORY_WIDTH; i++ ) {
        int x = slot_dest_rect.w/2 + i*slot_dest_rect.w;
        if ( player->hand_index == i ) render_main_hand_slot( x, y, renderer );
        else                           render_inventory_slot( x, y, renderer );

        if ( item_is_same( ITEM_EMPTY_SLOT, player->inventory[i] ) ) continue;

        item_render_center( player->inventory[i], x, y, renderer );
    }
}
void player_render_inventory( player_t* player, SDL_Renderer* renderer ) {
    if ( player == NULL || renderer == NULL ) return;

    render_hotbar( player, renderer );
}

void player_print_inventory( player_t* player ) {
    printf("player's inventory :\n");
    for ( int i = 0; i < PLAYER_INVENTORY_WIDTH; i++ ) {
        item_print( player->inventory[i] ); printf(" ");
    }
    puts("");
}
