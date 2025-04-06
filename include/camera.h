#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

typedef struct camera {
    float x, y;             // world position
    float scale;

    float width, height;    // world scale (only modified via scale)
    float pixels_per_block; // (only modified via scale)
} camera_t;

camera_t* camera_init( float x, float y, float scale );
void camera_destroy( camera_t* camera );

void camera_worldtoscreen_pos( camera_t* camera, float  world_x, float  world_y, int *screen_x, int *screen_y );

void camera_screentoworld_pos( camera_t* camera, float *world_x, float *world_y, int  screen_x, int  screen_y );

bool camera_is_inside( camera_t* camera, float world_x, float world_y );
bool camera_is_seeing( camera_t* camera, SDL_Rect rect );

void camera_set_pos( camera_t* camera, float x, float y );
void camera_set_pos_center( camera_t* camera, float x, float y );
// void camera_set_scale( camera_t* camera, float scale );

void camera_get_pos( camera_t* camera, float *x, float *y );
void camera_get_pos_center( camera_t* camera, float *x, float *y );
// void camera_get_scale( camera_t* camera, float *scale );

#endif