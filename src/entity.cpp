#include <entity.h> 

entity_t* entity_init( sprite_t* sprite, vector2_t position ) {
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
    vector2_t shape = vector2_new(
        (float)entity->sprite->dest_rect.w / PIXELS_PER_BLOCK, 
        (float)entity->sprite->dest_rect.h / PIXELS_PER_BLOCK 
    );
    rect_set_shape( &entity->world_rect, shape );

    entity->is_grounded = false;
    // entity->mass = mass;
    entity->velocity = vector2_zero();
    entity->acceleration = vector2_zero();

    return entity;
}

SDL_Texture* entity_destroy( entity_t* entity, bool destroy_texture ) {
    if ( !entity ) return NULL;

    SDL_Texture* texture = sprite_destroy( entity->sprite, destroy_texture );
    free( entity );

    return texture;
}

void entity_get_shape( entity_t* entity, vector2_t* shape ) {
    if ( !entity ) return;

    rect_get_shape( &entity->world_rect, shape );
}
vector2_t entity_output_shape( entity_t* entity ) {
    if ( !entity ) return vector2_zero();

    return rect_output_shape( &entity->world_rect );
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

int  entity_get_sprite_x     ( entity_t* entity, camera_t* camera ) {
    return camera_get_screen_x( camera, vector2_get_x( entity->world_rect.position ) );
}
int  entity_get_sprite_y     ( entity_t* entity, camera_t* camera ) {
    return camera_get_screen_y( camera, vector2_get_y( entity->world_rect.position ) );
}
// void entity_get_sprite_pos   ( entity_t* entity, camera_t* camera, int* x, int* y );
int  entity_get_sprite_width ( entity_t* entity, camera_t* camera ) {
    camera_scale_sprite( camera, &entity->sprite->dest_rect, entity->sprite->scale, &entity->sprite->src_rect );

    return entity->sprite->dest_rect.w;
}
int  entity_get_sprite_height( entity_t* entity, camera_t* camera ) {
    camera_scale_sprite( camera, &entity->sprite->dest_rect, entity->sprite->scale, &entity->sprite->src_rect );

    return entity->sprite->dest_rect.h;
}

void entity_render( entity_t* entity, camera_t* camera, SDL_Renderer* renderer ) {
    if ( !entity || !camera || !renderer ) return;
    if ( !camera_is_seeing( camera, &entity->world_rect ) ) return;

    int entity_screen_x = entity_get_sprite_x( entity, camera );
    int entity_screen_y = entity_get_sprite_y( entity, camera );
    sprite_set_pos( entity->sprite, entity_screen_x, entity_screen_y );

    // set the right scale for the sprite
    camera_scale_sprite( camera, &entity->sprite->dest_rect, entity->sprite->scale, &entity->sprite->src_rect );

    sprite_render( entity->sprite, renderer );
}
void entity_render_center( entity_t* entity, camera_t* camera, SDL_Renderer* renderer ) {
    if ( !entity || !camera || !renderer ) return;
    if ( !camera_is_seeing( camera, &entity->world_rect ) ) return;

    int entity_screen_x, entity_screen_y;
    camera_worldtoscreen_pos( camera, entity_output_pos( entity ), &entity_screen_x, &entity_screen_y );
    entity_screen_x -= sprite_get_width ( entity->sprite ) / 2;
    entity_screen_y -= sprite_get_height( entity->sprite ) / 2;
    sprite_set_pos( entity->sprite, entity_screen_x, entity_screen_y );

    // set the right scale for the sprite
    camera_scale_sprite( camera, &entity->sprite->dest_rect, entity->sprite->scale, &entity->sprite->src_rect );

    sprite_render( entity->sprite, renderer );
}

void entity_add_force( entity_t* entity, vector2_t force ) {
    if ( !entity ) return;

    // vector2_div_to( &force, entity->mass ); // F = m * a => a = F / m
    vector2_add_to( &entity->acceleration, force );
}
void entity_apply_force( entity_t* entity ) {
    if ( !entity ) return;

    // apply friction
    // vector2_t friction = vector2_mult( -entity->friction, entity->velocity );
    // entity_add_force( entity, friction );

    vector2_add_to( &entity->velocity, entity->acceleration );

    vector2_add_to( &entity->world_rect.position, entity->velocity );

    // if ( prevent_world_exit( &entity->world_rect ) ) {
    //     entity->velocity = vector2_zero();
    // }

    // printf("player accel: "); vector2_print( entity->acceleration ); puts("");
    entity->acceleration = vector2_zero(); // reset acceleration
}


// attract the entity towards the position with an amplitude 'force'
void entity_attract( entity_t* entity, vector2_t position, float force ) {
    vector2_t direction = vector2_direction( entity->world_rect.position, position );

    entity_add_force( entity, vector2_mult( force, direction ) );
}

void entity_impulse( entity_t* entity, vector2_t impulse ) {
    if ( !entity ) return;

    vector2_add_to( &entity->velocity, impulse );
}

void get_colliding_coords( entity_t* entity, int* colliding_left, int* colliding_top, int* colliding_right, int* colliding_bottom ) {
    vector2_t temp;
    entity_get_pos( entity, &temp );
    if ( colliding_left ) *colliding_left   = (int)vector2_get_x( temp );
    if ( colliding_top  ) *colliding_top    = (int)vector2_get_y( temp );
    
    vector2_add_to( &temp, entity_output_shape( entity ) );
    if ( colliding_right  ) *colliding_right  = (int)vector2_get_x( temp );
    if ( colliding_bottom ) *colliding_bottom = (int)vector2_get_y( temp );
}
void entity_push( entity_t* entity, int colliding_right, int colliding_bottom, vector2_t displacement ) {
    if ( !entity ) return;
    
    vector2_t shape;

    entity->is_grounded = false;

    if ( vector2_is_zero( displacement ) ) return;
    // printf("pushing entity : "); vector2_print( displacement ); puts("");

    entity_get_shape( entity, &shape );
    if ( vector2_get_y( displacement ) > 0 ) {
        entity->world_rect.position.y  = (int)entity->world_rect.position.y + 1.0f;
        entity->velocity.y = 0;
    }
    else if ( vector2_get_y( displacement ) < 0 ) {
        entity->world_rect.position.y  = (float)colliding_bottom - vector2_get_y( shape ) - 0.01f;
        entity->velocity.y = 0;
        entity->is_grounded = true;
    }
    if ( vector2_get_x( displacement ) > 0 ) {
        entity->world_rect.position.x  = (int)entity->world_rect.position.x + 1.0f;
        entity->velocity.x = 0;
    }
    else if ( vector2_get_x( displacement ) < 0 ) {
        entity->world_rect.position.x  = (float)colliding_right - vector2_get_x( shape ) - 0.01f;
        entity->velocity.x = 0;
    }
}
// return the displacement vector
vector2_t check_corner_collisions( entity_t* entity, world_t* world, int colliding_left, int colliding_top, int colliding_right, int colliding_bottom ) {
    if ( !entity || !world ) return vector2_zero();

    vector2_t colliding_boundary;
    vector2_t entity_corner_pos;
    vector2_t entity_shape = entity_output_shape( entity );

    vector2_t depth = vector2_zero();
    vector2_t displacement = vector2_zero();
    if ( AIR != world_output_block( world, colliding_left, colliding_top ) ) {
        entity->is_grounded = true;

        colliding_boundary = vector2_new( colliding_left+1.0f, colliding_top+1.0f );
        entity_corner_pos = entity_output_pos( entity );
        depth = vector2_sub( colliding_boundary, entity_corner_pos );

        // printf("topleft collision, depth: "); vector2_print( depth ); puts("");

        if ( vector2_get_x( entity->velocity ) < 0 && vector2_get_y( entity->velocity ) < 0 ) {
            if ( fabs( vector2_get_x( depth ) ) > fabs( vector2_get_y( depth ) ) ) {
                vector2_add_to( &displacement, vector2_project_y( depth ) );
            }
            else {
                vector2_add_to( &displacement, vector2_project_x( depth ) );
            }
        }
        else {
            if ( vector2_get_x( entity->velocity ) >= 0 ) {
                vector2_add_to( &displacement, vector2_project_y( depth ) );
            }
            else { // vector2_get_y( entity->velocity ) >= 0
                vector2_add_to( &displacement, vector2_project_x( depth ) );
            }
        }
    }
    if ( AIR != world_output_block( world, colliding_right, colliding_top ) ) {
        entity->is_grounded = true;

        colliding_boundary = vector2_new( colliding_right, colliding_top+1.0f );
        entity_corner_pos = vector2_add( entity_output_pos( entity ), vector2_project_x( entity_shape ) );
        depth = vector2_sub( colliding_boundary, entity_corner_pos );

        // printf("topright collision, depth: "); vector2_print( depth ); puts("");

        if ( vector2_get_x( entity->velocity ) > 0 && vector2_get_y( entity->velocity ) < 0 ) {
            if ( fabs( vector2_get_x( depth ) ) > fabs( vector2_get_y( depth ) ) ) {
                vector2_add_to( &displacement, vector2_project_y( depth ) );
            }
            else {
                vector2_add_to( &displacement, vector2_project_x( depth ) );
            }
        }
        else {
            if ( vector2_get_x( entity->velocity ) <= 0 ) {
                vector2_add_to( &displacement, vector2_project_y( depth ) );
            }
            else { // vector2_get_y( entity->velocity ) >= 0
                vector2_add_to( &displacement, vector2_project_x( depth ) );
            }
        }
    }
    if ( AIR != world_output_block( world, colliding_left, colliding_bottom ) ) {
        entity->is_grounded = true;

        colliding_boundary = vector2_new( colliding_left+1.0f, colliding_bottom );
        entity_corner_pos = vector2_add( entity_output_pos( entity ), vector2_project_y( entity_shape ) );
        depth = vector2_sub( colliding_boundary, entity_corner_pos );

        // printf("bottomleft collision, depth: "); vector2_print( depth ); puts("");

        if ( vector2_get_x( entity->velocity ) < 0 && vector2_get_y( entity->velocity ) > 0 ) {
            if ( fabs( vector2_get_x( depth ) ) > fabs( vector2_get_y( depth ) ) ) {
                vector2_add_to( &displacement, vector2_project_y( depth ) );
            }
            else {
                vector2_add_to( &displacement, vector2_project_x( depth ) );
            }
        }
        else {
            if ( vector2_get_x( entity->velocity ) >= 0 ) {
                vector2_add_to( &displacement, vector2_project_y( depth ) );
            }
            else { // vector2_get_y( entity->velocity ) >= 0
                vector2_add_to( &displacement, vector2_project_x( depth ) );
            }
        }
    }
    if ( AIR != world_output_block( world, colliding_right, colliding_bottom ) ) {
        entity->is_grounded = true;

        colliding_boundary = vector2_new( colliding_right, colliding_bottom );
        entity_corner_pos = vector2_add( entity_output_pos( entity ), entity_shape );
        depth = vector2_sub( colliding_boundary, entity_corner_pos );

        // printf("bottomright collision, depth: "); vector2_print( depth ); puts("");

        if ( vector2_get_x( entity->velocity ) > 0 && vector2_get_y( entity->velocity ) > 0 ) {
            if ( fabs( vector2_get_x( depth ) ) > fabs( vector2_get_y( depth ) ) ) {
                vector2_add_to( &displacement, vector2_project_y( depth ) );
            }
            else {
                vector2_add_to( &displacement, vector2_project_x( depth ) );
            }
        }
        else {
            if ( vector2_get_x( entity->velocity ) <= 0 ) {
                vector2_add_to( &displacement, vector2_project_y( depth ) );
            }
            else { // vector2_get_y( entity->velocity ) >= 0
                vector2_add_to( &displacement, vector2_project_x( depth ) );
            }
        }
    }

    return displacement;
}
void entity_apply_collisions( entity_t* entity, world_t* world ) {
    if ( !entity || !world ) return;

    int colliding_left , colliding_top;
    int colliding_right, colliding_bottom;
    get_colliding_coords( entity, &colliding_left, &colliding_top, &colliding_right, &colliding_bottom );

    // check for collisions within the border of the entity (no corners)
    vector2_t displacement = vector2_zero();
    world_get_collisions( world, colliding_left, colliding_top, colliding_right, colliding_bottom, &displacement );
    // printf("collisions displacement: "); vector2_print( displacement ); puts("");

    // move the entity
    if ( !vector2_is_zero( displacement ) ) {
        // printf("entity is grounded\n");
        entity_push( entity, colliding_right, colliding_bottom, displacement );
        return;
    }

    // check for collisions within the corners of the entity
    displacement = check_corner_collisions( entity, world, colliding_left, colliding_top, colliding_right, colliding_bottom );
    // printf("corner collisions displacement: "); vector2_print( displacement ); puts("");
    
    if ( fabs( vector2_get_x( displacement ) ) > fabs( vector2_get_y( displacement ) ) ) {
        entity_push( entity, colliding_right, colliding_bottom, vector2_project_x( displacement ) );
    }
    else {
        entity_push( entity, colliding_right, colliding_bottom, vector2_project_y( displacement ) );
    }

}

