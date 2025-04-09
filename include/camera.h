#ifndef CAMERA_H
#define CAMERA_H

#include <common.h>
#include <rect.h>
#include <utils.h>

typedef struct camera {
    rect_t world_rect; // world position
    float scale;
    float speed;
} camera_t;

camera_t* camera_init( float x, float y, float scale, float speed );
void camera_destroy( camera_t* camera );

// void camera_update( camera_t* camera, const Uint8* keystate );

void camera_worldtoscreen_pos( camera_t* camera, float  world_x, float  world_y, int *screen_x, int *screen_y );

void camera_screentoworld_pos( camera_t* camera, float *world_x, float *world_y, int  screen_x, int  screen_y );

bool camera_is_inside( camera_t* camera, float world_x, float world_y );
bool camera_is_seeing( camera_t* camera, rect_t* rect );

void camera_set_pos( camera_t* camera, float x, float y );
void camera_set_pos_center( camera_t* camera, float x, float y );
// void camera_set_scale( camera_t* camera, float scale );

void camera_get_pos( camera_t* camera, float *x, float *y );
void camera_get_pos_center( camera_t* camera, float *x, float *y );
void camera_get_shape( camera_t* camera, float *w, float *h );
// void camera_get_scale( camera_t* camera, float *scale );

void camera_update( camera_t* camera, float target_x, float target_y, const Uint8* keystate );

#endif