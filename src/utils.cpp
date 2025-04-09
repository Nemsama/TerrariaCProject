#include <utils.h>

void prevent_exit( rect_t* rect, rect_t* boundaries ) {
    if ( rect->x < boundaries->x ) rect->x = boundaries->x;
    if ( rect->y < boundaries->y ) rect->y = boundaries->y;
    if ( ( rect->x + rect->w ) > ( boundaries->x + boundaries->w ) ) rect->x = ( boundaries->x + boundaries->w ) - rect->w;
    if ( ( rect->y + rect->h ) > ( boundaries->y + boundaries->h ) ) rect->y = ( boundaries->y + boundaries->h ) - rect->h;
}

void prevent_world_exit( rect_t* rect ) {
    if ( rect->x < 0 ) rect->x = 0;
    if ( rect->y < 0 ) rect->y = 0;
    if ( ( rect->x + rect->w ) > WORLD_WIDTH ) rect->x = WORLD_WIDTH - rect->w;
    if ( ( rect->y + rect->h ) > WORLD_HEIGHT ) rect->y = WORLD_HEIGHT - rect->h;
}