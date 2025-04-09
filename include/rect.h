#ifndef RECT_H
#define RECT_H

#include <stdlib.h>
#include <stdio.h>
#include <vector.h>

typedef struct rect {
    vector2_t position;
    vector2_t shape;
} rect_t;

rect_t* rect_init( vector2_t position, vector2_t shape );
void rect_destroy( rect_t* rect );
void rect_set_pos( rect_t* rect, vector2_t position );
void rect_set_shape( rect_t* rect, vector2_t shape );
void rect_get_pos( rect_t* rect, vector2_t* position );
vector2_t rect_output_pos( rect_t* rect );
void rect_get_shape( rect_t* rect, vector2_t* shape );
vector2_t rect_output_shape( rect_t* rect );

bool rect_is_inside( const rect_t* rect, const vector2_t position );
bool rect_collision( const rect_t* rect1, const rect_t* rect2 );

#endif