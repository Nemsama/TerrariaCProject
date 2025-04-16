#include <player.h>


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
    player->max_speed = max_speed;
    player->jump_speed = jump_speed;

    return player;
}

void player_destroy( player_t* player ) {
    entity_destroy( player->entity );
    free( player );
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
        player_movement_x += running_force( vector2_get_x( player->entity->velocity ), player->mov_acceleration, player->max_speed );;
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
    
void player_handle_controls( player_t* player, const Uint8* keystate ) {
    if ( !player ) return;

    gravity_scum( player, keystate );

    // god_flight( player, keystate );
}

void player_update( player_t* player, const Uint8* keystate, world_t* world ) {
    if ( !player ) return;

    player_handle_controls( player, keystate );

    entity_apply_force( player->entity );

    // printf("player speed: "); vector2_print( player->entity->velocity ); puts("");
    // printf("player pos  : "); vector2_print( player->entity->world_rect.position ); puts("");

    // prevent_world_exit( &player->entity->world_rect );
    entity_apply_collisions( player->entity, world );
}

void player_render( player_t* player, camera_t* camera, SDL_Renderer* renderer ) {
    entity_render( player->entity, camera, renderer );
}
