#include <player.h>

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
    vector2_add_to( &player->world_rect.position, vector2_new( player_movement_x, player_movement_y ) );
}
void gravity_scum( player_t* player, const Uint8* keystate ) {
    float player_movement_x = 0.0f;
    float player_movement_y = 0.0f;

    if ( keystate[SDL_SCANCODE_A] ) {
        player_movement_x -= running_force( vector2_get_x( player->velocity ), -player->mov_acceleration, -player->max_speed );
    }
    if ( keystate[SDL_SCANCODE_D] ) {
        player_movement_x += running_force( vector2_get_x( player->velocity ), player->mov_acceleration, player->max_speed );;
    }
    if ( 0 == player_movement_x ) {
        if ( fabs( vector2_get_x( player->velocity ) ) < 0.04f ) {
            player->velocity.x = 0.0f;
        }
        player_movement_x = -sign( vector2_get_x( player->velocity ) ) * player->mov_acceleration/2.0f;
    }

    if ( keystate[SDL_SCANCODE_W] && player->is_grounded ) {
        entity_impulse( player, vector2_new( 0.0f, -player->jump_speed ) );
    }

    
    entity_add_force( player, vector2_new( player_movement_x, player_movement_y + GRAVITY ) );
}
    
void player_handle_controls( player_t* player, const Uint8* keystate ) {
    if ( !player ) return;

    gravity_scum( player, keystate );

    // god_flight( player, keystate );
}

void player_update( player_t* player, const Uint8* keystate, world_t* world ) {
    if ( !player ) return;

    player_handle_controls( player, keystate );

    entity_apply_force( player );

    // printf("player speed: "); vector2_print( player->velocity ); puts("");
    // printf("player pos  : "); vector2_print( player->world_rect.position ); puts("");

    // prevent_world_exit( &player->world_rect );
    entity_apply_collisions( player, world );
}
