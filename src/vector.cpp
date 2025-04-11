#include <vector.h>

vector2_t vector2_zero( void ) {
    return (vector2_t){ 0.0f, 0.0f };
}
vector2_t vector2_one( void ) {
    return (vector2_t){ 1.0f, 1.0f };
}
vector2_t vector2_new( float x, float y ) {
    return (vector2_t){ x, y };
}
vector2_t vector2_copy( vector2_t v ) {
    return (vector2_t){ v.x, v.y };
}
float vector2_get_x( vector2_t v ) {
    return v.x;
}
float vector2_get_y( vector2_t v ) {
    return v.y;
}
void vector2_get_int_values( vector2_t v, int* x, int* y ) {
    if ( x ) *x = v.x;
    if ( y ) *y = v.y;
}
float vector2_length( vector2_t v ) {
    return sqrtf( v.x * v.x + v.y * v.y );
}
float vector2_distance( vector2_t v1, vector2_t v2 ) {
    return sqrtf( (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) );
}
float vector2_length2( vector2_t v ) {
    return (v.x * v.x + v.y * v.y);
}
float vector2_distance2( vector2_t v1, vector2_t v2 ) {
    return ( (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) );
}

vector2_t vector2_add( vector2_t v1, vector2_t v2 ) {
    return (vector2_t){ v1.x + v2.x, v1.y + v2.y };
}
vector2_t vector2_sub( vector2_t v1, vector2_t v2 ) {
    return (vector2_t){ v1.x - v2.x, v1.y - v2.y };
}
vector2_t vector2_mult( float a, vector2_t v ) {
    return (vector2_t){ a * v.x, a * v.y };
}
void vector2_add_to( vector2_t* vdest, vector2_t v2 ) {
    vdest->x += v2.x;
    vdest->y += v2.y;
}
void vector2_sub_to( vector2_t* vdest, vector2_t v2 ) {
    vdest->x -= v2.x;
    vdest->y -= v2.y;
}
void vector2_mult_to( float a, vector2_t* vdest ) {
    vdest->x *= a;
    vdest->y *= a;
}
vector2_t vector2_div( vector2_t v, float a ) {
    return (vector2_t){ v.x / a, v.y / a };
}
void vector2_div_to( vector2_t* vdest, float a ) {
    vdest->x /= a;
    vdest->y /= a;
}
vector2_t vector2_normalized( vector2_t v ) {
    float length = vector2_length( v );
    if ( length == 0.0f ) return vector2_zero();
    return (vector2_t){ v.x / length, v.y / length };
}
void vector2_normalize( vector2_t* v ) {
    float length = vector2_length( *v );
    if ( length == 0.0f ) return;
    v->x /= length;
    v->y /= length;
}

float vector2_dot( vector2_t v1, vector2_t v2 ) {
    return (v1.x * v2.x + v1.y * v2.y);
}

vector2_t vector2_project_x( vector2_t v ) {
    return vector2_new( v.x, 0.0f );
}
vector2_t vector2_project_y( vector2_t v ) {
    return vector2_new( 0.0f, v.y );
}
void vector2_project_x_to( vector2_t* v ) {
    v->y = 0.0f;
}
void vector2_project_y_to( vector2_t* v ) {
    v->x = 0.0f;
}

float vector2_compare( vector2_t v1, vector2_t v2 ) {
    return ( v1.x - v2.x ) + ( v1.y - v2.y );
}
bool vector2_is_zero( vector2_t v ) {
    return ( v.x == 0.0f && v.y == 0.0f );
}
bool vector2_is_equal( vector2_t v1, vector2_t v2 ) {
    return ( v1.x == v2.x && v1.y == v2.y );
}

void vector2_print( vector2_t v ) {
    printf("(%f, %f)", v.x, v.y);
}
