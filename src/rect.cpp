#include <rect.h>

rect_t* rect_init( float x, float y, float w, float h ) {
    rect_t* rect = (rect_t*)calloc( 1, sizeof(*rect) );
    if ( NULL == rect ) {
        perror("Failed to allocate memory for rect structure");
        return NULL;
    }

    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;

    return rect;
}
void rect_destroy( rect_t* rect ) {
    free( rect );
}

void rect_set_pos( rect_t* rect, float x, float y ) {
    if ( NULL == rect ) return;

    rect->x = x;
    rect->y = y;
}
void rect_set_size( rect_t* rect, float w, float h ) {
    if ( NULL == rect ) return;

    rect->w = w;
    rect->h = h;
}
void rect_get_pos( rect_t* rect, float *x, float *y ) {
    if ( NULL == rect ) return;

    if ( x ) *x = rect->x;
    if ( y ) *y = rect->y;
}
void rect_get_size( rect_t* rect, float *w, float *h ) {
    if ( NULL == rect ) return;

    if ( w ) *w = rect->w;
    if ( h ) *h = rect->h;
}


bool rect_is_inside( rect_t* rect, float x, float y ) {
    if ( NULL == rect ) return false;

    if      ( x < rect->x ) return false;
    else if ( y < rect->y ) return false;
    else if ( x > ( rect->x + rect->w ) ) return false;
    else if ( y > ( rect->y + rect->h ) ) return false;
    else return true;
}

bool rect_collision( rect_t* rect1, rect_t* rect2 ) {
    if ( NULL == rect1 || NULL == rect2 ) return false;

    if      ( rect1->x > (rect2->x + rect2->w) ) return false;
    else if ( rect1->y > (rect2->y + rect2->h) ) return false;
    else if ( (rect1->x + rect1->w) < rect2->x ) return false;
    else if ( (rect1->y + rect1->h) < rect2->y ) return false;
    else return true;
}