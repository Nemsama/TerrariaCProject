#include <player.h>

void player_handle_controls( player_t* player, const Uint8* keystate ) {
    float player_movement_x = 0.0f;
    float player_movement_y = 0.0f;

    if ( keystate[SDL_SCANCODE_A] ) {
        player_movement_x -= player->movement_speed;
    }
    if ( keystate[SDL_SCANCODE_D] ) {
        player_movement_x += player->movement_speed;
    }
    if ( keystate[SDL_SCANCODE_W] ) {
        player_movement_y -= player->movement_speed;
    }
    if ( keystate[SDL_SCANCODE_S] ) {
        player_movement_y += player->movement_speed;
    }

    entity_add_force( player, vector2_new( player_movement_x, player_movement_y ) );
}

void player_update( player_t* player, const Uint8* keystate, world_t* world ) {
    if ( !player ) return;

    player_handle_controls( player, keystate );

    entity_apply_force( player );
    // prevent_world_exit( &player->world_rect );
    entity_apply_collisions( player, world );
}
