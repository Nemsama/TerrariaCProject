#include <entity.h> 

entity_t* entity_init( sprite_t* sprite, vector2_t position, float mass, float speed ) {
    if ( !sprite ) {
        perror("An initialized sprite is needed for an entity");
        return NULL;
    }

    entity_t* entity = (entity_t*)calloc( 1, sizeof(*entity) );
    if ( entity == NULL ) {
        perror("Failed to allocate memory for entity structure");
        return NULL;
    }
    
    entity->sprite = sprite;
    rect_set_pos( &entity->world_rect, position );
    vector2_t shape = { 
        (float)entity->sprite->dest_rect.w / PIXELS_PER_BLOCK, 
        (float)entity->sprite->dest_rect.h / PIXELS_PER_BLOCK 
    };
    rect_set_shape( &entity->world_rect, shape );

    entity->movement_speed = speed;

    entity->is_grounded = false;
    entity->mass = mass;
    entity->velocity = vector2_zero();
    entity->acceleration = vector2_zero();

    return entity;
}

void entity_set_pos( entity_t* entity, vector2_t position ) {
    if ( !entity ) return;

    rect_set_pos( &entity->world_rect, position );
}
void entity_get_pos( entity_t* entity, vector2_t* position ) {
    if ( !entity ) return;

    rect_get_pos( &entity->world_rect, position );
}
vector2_t entity_output_pos( entity_t* entity ) {
    if ( !entity ) return vector2_zero();

    return rect_output_pos( &entity->world_rect );
}

void entity_destroy(entity_t* entity) {
    if ( !entity ) return;

    sprite_destroy( entity->sprite );
    free( entity );
}

void entity_render( entity_t* entity, camera_t* camera, SDL_Renderer* renderer ) {
    if ( !entity || !camera || !renderer ) return;
    if ( !camera_is_seeing( camera, &entity->world_rect ) ) return;

    int entity_screen_x, entity_screen_y;
    camera_worldtoscreen_pos( camera, entity_output_pos( entity ), &entity_screen_x, &entity_screen_y );
    sprite_set_pos( entity->sprite, entity_screen_x, entity_screen_y );

    sprite_render( entity->sprite, renderer );
}

void entity_add_force( entity_t* entity, vector2_t force ) {
    if ( !entity ) return;

    vector2_mult_to( 1/entity->mass, &force );
    vector2_add_to( &entity->acceleration, force );
}
void entity_apply_force( entity_t* entity ) {
    if ( !entity ) return;

    vector2_add_to( &entity->velocity, entity->acceleration ); // don't use friction for now

    vector2_add_to( &entity->world_rect.position, entity->velocity );
}
