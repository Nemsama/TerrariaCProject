#ifndef ENTITY_H
#define ENTITY_H

#include <common.h>
#include <sprite.h>
#include <camera.h>
#include <rect.h>
#include <utils.h>

typedef struct entity {
    sprite_t* sprite;
    rect_t world_rect;
    float movement_speed;

    // int health;

    bool is_grounded;
    float mass;
    float friction;
    vector2_t velocity;
    vector2_t acceleration;
} entity_t;

// need a valid (pre-initiated) sprite (pointer)
entity_t* entity_init( sprite_t* sprite, vector2_t position, float mass, float speed );

void entity_set_pos( entity_t* entity, vector2_t position );
void entity_get_pos( entity_t* entity, vector2_t* position );
vector2_t entity_output_pos( entity_t* entity );

void entity_render( entity_t* entity, camera_t* camera, SDL_Renderer* renderer );
void entity_add_force( entity_t* entity, vector2_t force );
void entity_apply_force( entity_t* entity );

// free entity memory AND entity's sprite memory
void entity_destroy( entity_t* entity );

#endif