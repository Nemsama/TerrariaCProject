#include <camera.h>

camera_t* camera_init( float x, float y, float scale, float speed ) {
    camera_t* camera = (camera_t*)calloc( 1, sizeof(*camera) );
    if ( NULL == camera ) {
        perror("Failed to allocate memory for camera structure");
        return NULL;
    }

    camera->world_rect.x = x;
    camera->world_rect.y = y;
    camera->world_rect.w = (float)WINDOW_WIDTH  / PIXELS_PER_BLOCK;
    camera->world_rect.h = (float)WINDOW_HEIGHT / PIXELS_PER_BLOCK;

    camera->scale = scale;

    camera->speed = speed;

    return camera;
}

void camera_destroy( camera_t* camera ) {
    free( camera );
}

bool camera_is_inside( camera_t* camera, float world_x, float world_y ) {
    if      ( world_x < camera->world_rect.x ) return false;
    else if ( world_y < camera->world_rect.y ) return false;
    else if ( world_x > ( camera->world_rect.x + camera->world_rect.w ) ) return false;
    else if ( world_y > ( camera->world_rect.y + camera->world_rect.h ) ) return false;
    else return true;
}
bool camera_is_seeing( camera_t* camera, rect_t* rect ) {
    return rect_collision( &camera->world_rect, rect );
}

void camera_worldtoscreen_pos( camera_t* camera, float  world_x, float  world_y, int *screen_x, int *screen_y ) {
    if ( screen_x ) *screen_x = (int)((world_x - camera->world_rect.x) * PIXELS_PER_BLOCK);
    if ( screen_y ) *screen_y = (int)((world_y - camera->world_rect.y) * PIXELS_PER_BLOCK);
}

void camera_screentoworld_pos( camera_t* camera, float *world_x, float *world_y, int  screen_x, int  screen_y ) {
    if ( world_x ) *world_x = (float)screen_x / PIXELS_PER_BLOCK + camera->world_rect.x;
    if ( world_y ) *world_y = (float)screen_y / PIXELS_PER_BLOCK + camera->world_rect.y;
}

void camera_get_pos( camera_t* camera, float *x, float *y ) {
    if ( x ) *x = camera->world_rect.x;
    if ( y ) *y = camera->world_rect.y;
}
void camera_get_pos_center( camera_t* camera, float *x, float *y ) {
    if ( x ) *x = camera->world_rect.x + camera->world_rect.w / 2;
    if ( y ) *y = camera->world_rect.y + camera->world_rect.h / 2;
}
void camera_get_shape( camera_t* camera, float *w, float *h ) {
    if ( w ) *w = camera->world_rect.w;
    if ( h ) *h = camera->world_rect.h;
}
/* void camera_get_scale( camera_t* camera, float *scale ) {
    if ( scale ) *scale = camera->scale;
} */

void camera_set_pos( camera_t* camera, float x, float y ) {
    camera->world_rect.x = x;
    camera->world_rect.y = y;
}
void camera_set_pos_center( camera_t* camera, float x, float y ) {
    camera->world_rect.x = x - camera->world_rect.w / 2;
    camera->world_rect.y = y - camera->world_rect.h / 2;
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

void camera_update( camera_t* camera, float target_x, float target_y, const Uint8* keystate ) {
    if ( camera == NULL ) return;

    if ( keystate[SDL_SCANCODE_SPACE] ) {
        // Center camera on target
        camera_set_pos_center( camera, target_x, target_y );
    }

    if ( keystate[SDL_SCANCODE_LEFT] ) {
        camera->world_rect.x -= camera->speed;
    }
    if ( keystate[SDL_SCANCODE_RIGHT] ) {
        camera->world_rect.x += camera->speed;
    }
    if ( keystate[SDL_SCANCODE_UP] ) {
        camera->world_rect.y -= camera->speed;
    }
    if ( keystate[SDL_SCANCODE_DOWN] ) {
        camera->world_rect.y += camera->speed;
    }
}
