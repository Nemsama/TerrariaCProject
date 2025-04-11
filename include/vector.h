#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <stdio.h>

typedef struct vector2D {
    float x;
    float y;
} vector2_t;

vector2_t vector2_zero( void );
vector2_t vector2_one( void );
vector2_t vector2_new( float x, float y );
vector2_t vector2_copy( vector2_t v );
float vector2_get_x( vector2_t v );
float vector2_get_y( vector2_t v );
void  vector2_get_int_values( vector2_t v, int* x, int* y );
float vector2_length( vector2_t v );
float vector2_distance( vector2_t v1, vector2_t v2 );
float vector2_length2( vector2_t v );
float vector2_distance2( vector2_t v1, vector2_t v2 );
// float vector2_dot( vector2_t v1, vector2_t v2 );

vector2_t vector2_add( vector2_t v1, vector2_t v2 );
void vector2_add_to( vector2_t* vdest, vector2_t v2 );
vector2_t vector2_sub( vector2_t v1, vector2_t v2 );
void vector2_sub_to( vector2_t* vdest, vector2_t v2 );
vector2_t vector2_mult( float a, vector2_t v );
void vector2_mult_to( float a, vector2_t* vdest );
vector2_t vector2_div( vector2_t v, float a );
void vector2_div_to( vector2_t* vdest, float a );
vector2_t vector2_normalized( vector2_t v );
void vector2_normalize( vector2_t* v );
float vector2_dot( vector2_t v1, vector2_t v2 );

vector2_t vector2_project_x( vector2_t v );
vector2_t vector2_project_y( vector2_t v );
void vector2_project_x_to( vector2_t* v );
void vector2_project_y_to( vector2_t* v );

float vector2_compare( vector2_t v1, vector2_t v2 );
bool vector2_is_zero( vector2_t v );
bool vector2_is_equal( vector2_t v1, vector2_t v2 );

void vector2_print( vector2_t v );

#endif