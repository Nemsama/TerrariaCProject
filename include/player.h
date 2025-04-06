#ifndef PLAYER_H
#define PLAYER_H

#include <sprite.h>
#include <camera.h>

typedef struct player {
    sprite_t* sprite;
    float x, y;       // world position
    float speed;

    // int health;
} player_t;

// need a valid (pre-initiated) sprite pointer
player_t* player_init( sprite_t* sprite, float x, float y, float speed );

void player_update( player_t* player, const Uint8* keystate );

void player_render( player_t* player, camera_t* camera, SDL_Renderer* renderer );

// free player memory AND player's sprite memory
void player_destroy( player_t* player );

#endif