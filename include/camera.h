#ifndef CAMERA_H
#define CAMERA_H

#include <common.h>
// dependancies : lower

#define CAMERA_BASE_WIDTH  (float)WINDOW_WIDTH  / PIXELS_PER_BLOCK
#define CAMERA_BASE_HEIGHT (float)WINDOW_HEIGHT / PIXELS_PER_BLOCK

typedef struct camera {
    rect_t world_rect; // world position
    float scale;
    int pixels_per_block;
    float speed;
} camera_t;

camera_t* camera_init( vector2_t position, float scale, float speed );
void camera_destroy( camera_t* camera );

// void camera_update( camera_t* camera, const Uint8* keystate );

void camera_worldtoscreen_pos( camera_t* camera, vector2_t  world_pos, int *screen_x, int *screen_y );

void camera_screentoworld_pos( camera_t* camera, vector2_t* world_pos, int  screen_x, int  screen_y );

bool camera_is_inside( camera_t* camera, vector2_t position );
bool camera_is_seeing( camera_t* camera, rect_t* rect );

void camera_set_pos( camera_t* camera, vector2_t position );
void camera_set_pos_center( camera_t* camera, vector2_t position );
void camera_set_scale( camera_t* camera, float scale );

void camera_get_pos( camera_t* camera, vector2_t* position );
void camera_get_pos_center( camera_t* camera, vector2_t* position );
void camera_get_shape( camera_t* camera, vector2_t* shape );
void camera_get_scale( camera_t* camera, float *scale );
void camera_add_scale( camera_t* camera, float scale );

void camera_scale_sprite( camera_t* camera, SDL_Rect* sprite_dest_rect, SDL_Rect* sprite_src_rect );
void camera_center_sprite( camera_t* camera, SDL_Rect* sprite_dest_rect );

void camera_update( camera_t* camera, vector2_t target_position, const Uint8* keystate );

#endif