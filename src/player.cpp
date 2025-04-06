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
    player->x = x;
    player->y = y;
    player->speed = speed;

    return player;
}

void player_set_pos( player_t* player, float x, float y ) {
    if ( !player ) return;

    player->x = x;
    player->y = y;
}
void player_get_pos( player_t* player, float* x, float* y ) {
    if ( !player ) return;

    if ( x ) *x = player->x;
    if ( y ) *y = player->y;
}

void player_update(player_t* player, const Uint8* keystate) {
    // static float movement_buffer = 0.0f;

    if ( keystate[SDL_SCANCODE_A] ) {
        player->x -= player->speed;
    }
    if ( keystate[SDL_SCANCODE_D] ) {
        player->x += player->speed;
    }
    if ( keystate[SDL_SCANCODE_W] ) {
        player->y += player->speed;
    }
    if ( keystate[SDL_SCANCODE_S] ) {
        player->y -= player->speed;
    }
}

void player_destroy(player_t* player) {
    if ( !player ) return;

    sprite_destroy( player->sprite );
    free( player );
}

void player_render( player_t* player, camera_t* camera, SDL_Renderer* renderer ) {
    if ( camera_is_inside( camera, player->x, player->y ) ) {
        int player_screen_x, player_screen_y;
        camera_worldtoscreen_pos( camera, player->x, player->y, &player_screen_x, &player_screen_y );
        sprite_set_pos( player->sprite, player_screen_x, player_screen_y );

        sprite_render( player->sprite, renderer );
    }
}
