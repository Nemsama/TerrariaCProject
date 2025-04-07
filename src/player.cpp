#include <player.h> 

player_t* player_init( sprite_t* sprite, float x, float y, float speed ) {
    if ( !sprite ) {
        perror("An initialized sprite is needed for a player");
        return NULL;
    }

    player_t* player = (player_t*)calloc( 1, sizeof(*player) );
    if ( player == NULL ) {
        perror("Failed to allocate memory for player structure");
        return NULL;
    }
    
    player->sprite = sprite;
    rect_set_pos( &player->world_rect, x, y );
    rect_set_size( &player->world_rect, 
                   (float)player->sprite->dest_rect.w / PIXELS_PER_BLOCK, 
                   (float)player->sprite->dest_rect.h / PIXELS_PER_BLOCK );
    player->speed = speed;

    return player;
}

void player_set_pos( player_t* player, float x, float y ) {
    if ( !player ) return;

    rect_set_pos( &player->world_rect, x, y );
}
void player_get_pos( player_t* player, float* x, float* y ) {
    if ( !player ) return;

    rect_get_pos( &player->world_rect, x, y );
}

void player_update(player_t* player, const Uint8* keystate) {
    // static float movement_buffer = 0.0f;

    if ( keystate[SDL_SCANCODE_A] ) {
        player->world_rect.x -= player->speed;
    }
    if ( keystate[SDL_SCANCODE_D] ) {
        player->world_rect.x += player->speed;
    }
    if ( keystate[SDL_SCANCODE_W] ) {
        player->world_rect.y -= player->speed;
    }
    if ( keystate[SDL_SCANCODE_S] ) {
        player->world_rect.y += player->speed;
    }
}

void player_destroy(player_t* player) {
    if ( !player ) return;

    sprite_destroy( player->sprite );
    free( player );
}

void player_render( player_t* player, camera_t* camera, SDL_Renderer* renderer ) {
    if ( camera_is_seeing( camera, &player->world_rect ) ) {
        int player_screen_x, player_screen_y;
        camera_worldtoscreen_pos( camera, player->world_rect.x, player->world_rect.y, &player_screen_x, &player_screen_y );
        sprite_set_pos( player->sprite, player_screen_x, player_screen_y );

        sprite_render( player->sprite, renderer );
    }
}
