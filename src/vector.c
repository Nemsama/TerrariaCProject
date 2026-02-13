#include <vector.h>

static float sign( float a ) {
    if      ( a > 0.0f ) return  1.0f;
    else if ( a < 0.0f ) return -1.0f;
    else return 0.0f;
}

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
void  vector2_set_x( vector2_t* v, float x ) {
    if ( v ) v->x = x;
}
void  vector2_set_y( vector2_t* v, float y ) {
    if ( v ) v->y = y;
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
vector2_t vector2_normalised( vector2_t v ) {
    float length = vector2_length( v );
    if ( length == 0.0f ) return vector2_zero();
    return (vector2_t){ v.x / length, v.y / length };
}
void vector2_normalise( vector2_t* v ) {
    float length = vector2_length( *v );
    if ( length == 0.0f ) return;
    v->x /= length;
    v->y /= length;
}

void  vector2_cart_to_polar( vector2_t* v_cart ) {
    if ( v_cart ) *v_cart = vector2_get_polar( *v_cart );
}
void  vector2_polar_to_cart( vector2_t* v_polar ) {
    if ( v_polar ) *v_polar = vector2_get_cart( *v_polar );
}
// output the polar coordinates of the supposed cartesian input vector
vector2_t vector2_get_polar( vector2_t v_cart ) {
    vector2_t output;

    output.x = vector2_length( v_cart );

    if ( v_cart.x == 0.0f ) output.y = M_PI/2.0f * sign( v_cart.y );
    else {
        output.y = atanf( v_cart.y / v_cart.x );
        if ( v_cart.x >  0 ) return output;
        if ( v_cart.y >  0 ) output.y += M_PI;
        if ( v_cart.y <= 0 ) output.y -= M_PI;
    }

    return output;
}
// output the cartesian coordinates of the supposed polar input vector
vector2_t vector2_get_cart ( vector2_t v_polar ) {
    vector2_t output;
    output.x = cosf( vector2_get_y( v_polar ) );
    output.y = sinf( vector2_get_y( v_polar ) );

    vector2_mult_to( vector2_get_x(v_polar), &output );

    return output;
}

float vector2_dot( vector2_t v1, vector2_t v2 ) {
    return (v1.x * v2.x + v1.y * v2.y);
}

// returns the normed direction from v1 to v2
vector2_t vector2_direction( vector2_t v1, vector2_t v2 ) {
    vector2_t direction;

    direction = vector2_sub( v2, v1 );

    vector2_normalise( &direction );

    return direction;
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

vector2_t vector2_random_on_unit_sphere( void ) {
    vector2_t out = vector2_one();

    out.y = 2*M_PI * rand()/(float)RAND_MAX;

    vector2_polar_to_cart( &out );

    return out;
}

void vector2_print( vector2_t v ) {
    printf("(%f, %f)", v.x, v.y);
}
