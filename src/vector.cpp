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

void vector2_print( vector2_t v ) {
    printf("(%f, %f)", v.x, v.y);
}
