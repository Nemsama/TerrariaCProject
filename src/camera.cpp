#include <camera.h>

camera_t* camera_init( float x, float y, float scale ) {
    camera_t* camera = (camera_t*)calloc( 1, sizeof(*camera) );
    if ( NULL == camera ) {
        perror("Failed to allocate memory for camera structure");
        return NULL;
    }

    camera->x = x;
    camera->y = y;
    camera->scale = scale;

    camera->width  = WINDOW_WIDTH  / PIXELS_PER_BLOCK;
    camera->height = WINDOW_HEIGHT / PIXELS_PER_BLOCK;

    return camera;
}

void camera_destroy( camera_t* camera ) {
    free( camera );
}

bool camera_is_inside( camera_t* camera, float world_x, float world_y ) {
    if ( world_x < camera->x ) return false;
    else if ( world_x > ( camera->x + camera->width  ) ) return false;
    else if ( world_y < camera->y ) return false;
    else if ( world_y > ( camera->y + camera->height ) ) return false;
    else return true;
}
bool camera_is_seeing( camera_t* camera, SDL_Rect rect ) {
    SDL_Rect camera_rect = {(int)camera->x, (int)camera->y, (int)camera->width, (int)camera->height};

    return SDL_HasIntersection( &rect, &camera_rect );
}

void camera_worldtoscreen_pos( camera_t* camera, float  world_x, float  world_y, int *screen_x, int *screen_y ) {
    if ( screen_x ) *screen_x = (int)((world_x - camera->x) * PIXELS_PER_BLOCK);
    int upward_y = (int)((world_y - camera->y) * PIXELS_PER_BLOCK);
    if ( screen_y ) *screen_y = (int)( camera->height * PIXELS_PER_BLOCK ) - upward_y;
}

void camera_screentoworld_pos( camera_t* camera, float *world_x, float *world_y, int  screen_x, int  screen_y ) {
    if ( world_x ) *world_x = (float)screen_x / PIXELS_PER_BLOCK + camera->x;
    int upward_y = (int)( camera->height * PIXELS_PER_BLOCK ) - screen_y;
    if ( world_y ) *world_y = (float)upward_y / PIXELS_PER_BLOCK + camera->y;
}

void camera_get_pos( camera_t* camera, float *x, float *y ) {
    if ( x ) *x = camera->x;
    if ( y ) *y = camera->y;
}
void camera_get_pos_center( camera_t* camera, float *x, float *y ) {
    if ( x ) *x = camera->x + (camera->width / 2);
    if ( y ) *y = camera->y + (camera->height / 2);
}
/* void camera_get_scale( camera_t* camera, float *scale ) {
    if ( scale ) *scale = camera->scale;
} */

void camera_set_pos( camera_t* camera, float x, float y ) {
    camera->x = x;
    camera->y = y;
}
void camera_set_pos_center( camera_t* camera, float x, float y ) {
    camera->x = x - camera->width /2;
    camera->y = y - camera->height/2;
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
