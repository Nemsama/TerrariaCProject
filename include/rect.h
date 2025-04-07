#ifndef RECT_H
#define RECT_H

#include <stdlib.h>
#include <stdio.h>

typedef struct rect {
    float x, y;          // position
    float w, h;          // size
} rect_t;

rect_t* rect_init( float x, float y, float w, float h );
void rect_destroy( rect_t* rect );
void rect_set_pos( rect_t* rect, float x, float y );
void rect_set_size( rect_t* rect, float w, float h );
void rect_get_pos( rect_t* rect, float *x, float *y );
void rect_get_size( rect_t* rect, float *w, float *h );

bool rect_is_inside( rect_t* rect, float x, float y );
bool rect_collision( rect_t* rect1, rect_t* rect2 );

#endif