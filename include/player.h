#ifndef PLAYER_H
#define PLAYER_H

#include <common.h>
#include <entity.h>
#include <item.h>
// dependancies : lower

#define INVENTORY_SLOT_PATH "assets/inventory_slot.png"

#define PLAYER_INVENTORY_WIDTH 10
#define PLAYER_INVENTORY_HEIGHT 1
#define PLAYER_INVENTORY_SIZE PLAYER_INVENTORY_WIDTH * PLAYER_INVENTORY_HEIGHT

typedef struct {
    entity_t* entity;

    // movements handling
    float mov_acceleration;
    float max_speed;  // blocks per frame
    float jump_speed; // blocks per frame

    // inventory handling
    int hand_index;
    item_t* hand_item;
    item_t* inventory[PLAYER_INVENTORY_SIZE];
} player_t;

player_t* player_init( sprite_t* sprite, vector2_t position, float acceleration, float max_speed /* in blocks per seconds */, float jump_speed );
void player_destroy( player_t* player );
void player_destroy_inventory( player_t* player );

void    init_inventory_slot_texture( SDL_Renderer* renderer );
void destroy_inventory_slot_texture( void );

void player_left_click( player_t* player, camera_t* camera, vector2_t world_pos, world_t* world );

void player_update( player_t* player, const Uint8* keystate, world_t* world );

void player_render( player_t* player, camera_t* camera, SDL_Renderer* renderer );
void player_render_inventory( player_t* player, SDL_Renderer* renderer );

void player_print_inventory( player_t* player );

#endif