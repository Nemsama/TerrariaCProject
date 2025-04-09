#include <rect.h>

rect_t* rect_init( vector2_t position, vector2_t shape ) {
    rect_t* rect = (rect_t*)calloc( 1, sizeof(*rect) );
    if ( NULL == rect ) {
        perror("Failed to allocate memory for rect structure");
        return NULL;
    }

    rect->position = position;
    rect->shape = shape;

    return rect;
}
void rect_destroy( rect_t* rect ) {
    free( rect );
}

void rect_set_pos( rect_t* rect, vector2_t position ) {
    if ( NULL == rect ) return;

    rect->position = position;
}
void rect_set_shape( rect_t* rect, vector2_t shape ) {
    if ( NULL == rect ) return;

    rect->shape = shape;
}
void rect_get_pos( rect_t* rect, vector2_t* position ) {
    if ( NULL == rect ) return;

    if ( position ) *position = rect->position;
}
vector2_t rect_output_pos( rect_t* rect ) {
    if ( NULL == rect ) return vector2_zero();

    return rect->position;
}
void rect_get_shape( rect_t* rect, vector2_t* shape ) {
    if ( NULL == rect ) return;

    if ( shape ) *shape = rect->shape;
}
vector2_t rect_output_shape( rect_t* rect ) {
    if ( NULL == rect ) return vector2_zero();

    return rect->shape;
}


bool rect_is_inside( const rect_t* rect, const vector2_t position ) {
    if ( NULL == rect ) return false;

    if      ( position.x < rect->position.x ) return false;
    else if ( position.y < rect->position.y ) return false;
    else if ( position.x > ( rect->position.x + rect->shape.x ) ) return false;
    else if ( position.y > ( rect->position.y + rect->shape.y ) ) return false;
    else return true;
}

bool rect_collision( const rect_t* rect1, const rect_t* rect2 ) {
    if ( NULL == rect1 || NULL == rect2 ) return false;

    if      ( rect1->position.x > ( rect2->position.x + rect2->shape.x ) ) return false;
    else if ( rect1->position.y > ( rect2->position.y + rect2->shape.y ) ) return false;
    else if ( ( rect1->position.x + rect1->shape.x ) < rect2->position.x ) return false;
    else if ( ( rect1->position.y + rect1->shape.y ) < rect2->position.y ) return false;
    else return true;
}