#include <camera.h>

camera_t* camera_init( vector2_t position, float scale, float speed ) {
    camera_t* camera = (camera_t*)calloc( 1, sizeof(*camera) );
    if ( NULL == camera ) {
        perror("Failed to allocate memory for camera structure");
        return NULL;
    }

    rect_set_pos( &camera->world_rect, position );
    vector2_t camera_shape = vector2_new( (float)WINDOW_WIDTH  / PIXELS_PER_BLOCK,
                                          (float)WINDOW_HEIGHT / PIXELS_PER_BLOCK );
    rect_set_shape( &camera->world_rect, camera_shape );

    camera->scale = scale;

    camera->speed = speed;

    return camera;
}

void camera_destroy( camera_t* camera ) {
    free( camera );
}

bool camera_is_inside( camera_t* camera, vector2_t position ) {
    if ( camera == NULL ) return false;
    return rect_is_inside( &camera->world_rect, position );
}
bool camera_is_seeing( camera_t* camera, rect_t* rect ) {
    return rect_collision( &camera->world_rect, rect );
}

void camera_worldtoscreen_pos( camera_t* camera, vector2_t  world_pos, int *screen_x, int *screen_y ) {
    vector2_t camera_relative_pos = vector2_sub( world_pos, camera->world_rect.position );
    vector2_mult_to( PIXELS_PER_BLOCK, &camera_relative_pos );
    if ( screen_x ) *screen_x = (int)camera_relative_pos.x;
    if ( screen_y ) *screen_y = (int)camera_relative_pos.y;
}

void camera_screentoworld_pos( camera_t* camera, vector2_t* world_pos, int  screen_x, int  screen_y ) {
    if ( camera == NULL || world_pos == NULL ) return;
    vector2_t camera_relative_pos = vector2_new( (float)screen_x, (float)screen_y );
    vector2_mult_to( 1.0f / PIXELS_PER_BLOCK, &camera_relative_pos );
    *world_pos = vector2_add( rect_output_pos( &camera->world_rect ), camera_relative_pos );
}

void camera_get_pos( camera_t* camera, vector2_t* position ) {
    if ( camera == NULL || position == NULL ) return;
    *position = camera->world_rect.position;
}
void camera_get_pos_center( camera_t* camera, vector2_t* position ) {
    if ( camera == NULL || position == NULL ) return;
    *position = vector2_add( camera->world_rect.position, vector2_mult( 0.5f, camera->world_rect.shape ) );
}
void camera_get_shape( camera_t* camera, vector2_t* position ) {
    if ( camera == NULL || position == NULL ) return;
    *position = camera->world_rect.shape;
}
/* void camera_get_scale( camera_t* camera, float *scale ) {
    if ( scale ) *scale = camera->scale;
} */

void camera_set_pos( camera_t* camera, vector2_t position ) {
    if ( camera == NULL ) return;
    camera->world_rect.position = position;
}
void camera_set_pos_center( camera_t* camera, vector2_t position ) {
    if ( camera == NULL ) return;
    camera->world_rect.position = vector2_sub( position, vector2_mult( 0.5f, camera->world_rect.shape ) );
    
}
/* 
void camera_set_scale( camera_t* camera, float scale ) {
    float center_x, center_y;
    camera_get_pos_center( camera, &center_x, &center_y );

    camera->scale = scale;
    camera->width  = 60.0f * scale;
    camera->height = 30.0f * scale;
    PIXELS_PER_BLOCK = 32.0f / scale;

    // recenter the camera
    camera_set_pos_center( camera, center_x, center_y );
} */

void camera_update( camera_t* camera, vector2_t target_position, const Uint8* keystate ) {
    if ( camera == NULL ) return;

    float camera_movement_x = 0.0f;
    float camera_movement_y = 0.0f;
    if ( keystate[SDL_SCANCODE_LEFT] ) {
        camera_movement_x -= camera->speed;
    }
    if ( keystate[SDL_SCANCODE_RIGHT] ) {
        camera_movement_x += camera->speed;
    }
    if ( keystate[SDL_SCANCODE_UP] ) {
        camera_movement_y -= camera->speed;
    }
    if ( keystate[SDL_SCANCODE_DOWN] ) {
        camera_movement_y += camera->speed;
    }
    vector2_add_to( &camera->world_rect.position, vector2_new( camera_movement_x, camera_movement_y ) );

    if ( keystate[SDL_SCANCODE_SPACE] ) {
        // Center camera on target
        camera_set_pos_center( camera, target_position );
    }

    prevent_world_exit( &camera->world_rect );
}
