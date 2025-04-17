#include <camera.h>

camera_t* camera_init( vector2_t position, float scale, float speed ) {
    camera_t* camera = (camera_t*)calloc( 1, sizeof(*camera) );
    if ( NULL == camera ) {
        perror("Failed to allocate memory for camera structure");
        return NULL;
    }

    rect_set_pos( &camera->world_rect, position );
    vector2_t camera_shape = vector2_new( CAMERA_BASE_WIDTH * scale, CAMERA_BASE_HEIGHT * scale );
    rect_set_shape( &camera->world_rect, camera_shape );

    camera->scale = scale;
    camera->pixels_per_block = (int)roundf( PIXELS_PER_BLOCK / scale );

    camera->speed = speed * scale;

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
    vector2_mult_to( camera->pixels_per_block, &camera_relative_pos );
    if ( screen_x ) *screen_x = (int)camera_relative_pos.x;
    if ( screen_y ) *screen_y = (int)camera_relative_pos.y;
}

void camera_screentoworld_pos( camera_t* camera, vector2_t* world_pos, int  screen_x, int  screen_y ) {
    if ( camera == NULL || world_pos == NULL ) return;
    vector2_t camera_relative_pos = vector2_new( (float)screen_x, (float)screen_y );
    vector2_mult_to( 1.0f / camera->pixels_per_block, &camera_relative_pos );
    *world_pos = vector2_add( rect_output_pos( &camera->world_rect ), camera_relative_pos );
}
vector2_t camera_get_world_pos( camera_t* camera, int screen_x, int screen_y ) {
    vector2_t world_pos;

    camera_screentoworld_pos( camera, &world_pos, screen_x, screen_y );

    return world_pos;
}

int camera_get_screen_x( camera_t* camera, float world_x ) {
    float camera_relative_pos = world_x - vector2_get_x( camera->world_rect.position );
    return (int)( camera_relative_pos * camera->pixels_per_block );
}
int camera_get_screen_y( camera_t* camera, float world_y ) {
    float camera_relative_pos = world_y - vector2_get_y( camera->world_rect.position );
    return (int)( camera_relative_pos * camera->pixels_per_block );
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
void camera_get_scale( camera_t* camera, float *scale ) {
    if ( scale ) *scale = camera->scale;
}

void camera_set_pos( camera_t* camera, vector2_t position ) {
    if ( camera == NULL ) return;
    camera->world_rect.position = position;
}
void camera_set_pos_center( camera_t* camera, vector2_t position ) {
    if ( camera == NULL ) return;
    camera->world_rect.position = vector2_sub( position, vector2_mult( 0.5f, camera->world_rect.shape ) );
    
}
void camera_set_scale( camera_t* camera, float scale ) {
    if ( camera == NULL || scale < 0.1f ) return;

    vector2_t camera_center;
    camera_get_pos_center( camera, &camera_center );

    camera->scale = scale;
    vector2_t camera_shape = vector2_new( CAMERA_BASE_WIDTH * scale, CAMERA_BASE_HEIGHT * scale );
    rect_set_shape( &camera->world_rect, camera_shape );
    camera->pixels_per_block = (int)roundf( PIXELS_PER_BLOCK / scale );
    camera->speed = CAMERA_SPEED * scale;

    // recenter the camera
    camera_set_pos_center( camera, camera_center );
}
void camera_add_scale( camera_t* camera, float delta_scale ) {
    if ( camera == NULL ) return;
    if ( camera->scale + delta_scale < 0.1f ) return;

    camera_set_scale( camera, camera->scale + delta_scale );
}/* 
void camera_reduce_scale( camera_t* camera, float delta_scale ) {
    if ( camera == NULL ) return;

    camera_set_scale( camera, camera->scale - delta_scale );
} */

void camera_scale_sprite( camera_t* camera, SDL_Rect* sprite_dest_rect, SDL_Rect* sprite_src_rect ) {
    if ( camera == NULL || sprite_dest_rect == NULL || sprite_src_rect == NULL ) return;

    sprite_dest_rect->w = (int)roundf( sprite_src_rect->w / camera->scale );
    sprite_dest_rect->h = (int)roundf( sprite_src_rect->h / camera->scale );
}

void camera_center_sprite( camera_t* camera, SDL_Rect* sprite_dest_rect ) {
    if ( camera == NULL || sprite_dest_rect == NULL ) return;
    
    sprite_dest_rect->x =  WINDOW_WIDTH/2 - sprite_dest_rect->w/2;
    sprite_dest_rect->y = WINDOW_HEIGHT/2 - sprite_dest_rect->h/2;
}

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
    /* 
    if ( keystate[SDL_SCANCODE_KP_2] ) {
        // Zoom in
        // camera_add_scale( camera, -0.01f );
        camera_set_scale( camera, 1.0f );
    }
    if ( keystate[SDL_SCANCODE_KP_8] ) {
        // Zoom out
        // camera_add_scale( camera, 0.01f );
        camera_set_scale( camera, 2.0f );
    } */

    if ( keystate[SDL_SCANCODE_SPACE] ) {
        // Center camera on target
        camera_set_pos_center( camera, target_position );
    }

    // prevent_world_exit( &camera->world_rect );
}
