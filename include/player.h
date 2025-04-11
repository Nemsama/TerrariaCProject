#ifndef PLAYER_H
#define PLAYER_H

#include <common.h>
#include <entity.h>

typedef entity_t player_t;

void player_update( player_t* player, const Uint8* keystate, world_t* world );

#endif