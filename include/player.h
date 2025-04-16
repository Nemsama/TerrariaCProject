#ifndef PLAYER_H
#define PLAYER_H

#include <common.h>
#include <entity.h>
// dependancies : lower

typedef struct {
    entity_t* entity;

    float mov_acceleration;
    float max_speed;  // blocks per frame
    float jump_speed; // blocks per frame
} player_t;

player_t* player_init( sprite_t* sprite, vector2_t position, float acceleration, float max_speed /* in blocks per seconds */, float jump_speed );
void player_destroy( player_t* player );

void player_update( player_t* player, const Uint8* keystate, world_t* world );

void player_render( player_t* player, camera_t* camera, SDL_Renderer* renderer );

#endif