#include <utils.h>

void prevent_exit( rect_t* rect, rect_t* boundaries ) {
    if ( !rect || !boundaries ) return;
    vector2_t rect_pos;
    vector2_t rect_shape;
    rect_get_pos( rect, &rect_pos );
    rect_get_shape( rect, &rect_shape );
    vector2_t boundaries_pos;
    vector2_t boundaries_shape;
    rect_get_pos( boundaries, &boundaries_pos );
    rect_get_shape( boundaries, &boundaries_shape );

    if ( rect_pos.x < boundaries_pos.x ) rect_pos.x = boundaries_pos.x;
    if ( rect_pos.y < boundaries_pos.y ) rect_pos.y = boundaries_pos.y;
    if ( ( rect_pos.x + rect_shape.x ) > ( boundaries_pos.x + boundaries_shape.x ) )
        rect_pos.x = ( boundaries_pos.x + boundaries_shape.x ) - rect_shape.x;
    if ( ( rect_pos.y + rect_shape.y ) > ( boundaries_pos.y + boundaries_shape.y ) )
        rect_pos.y = ( boundaries_pos.y + boundaries_shape.y ) - rect_shape.y;
    
    rect_set_pos( rect, rect_pos );
    rect_set_shape( rect, rect_shape );
}

void prevent_world_exit( rect_t* rect ) {
    if ( !rect ) return;
    vector2_t rect_pos;
    vector2_t rect_shape;
    rect_get_pos( rect, &rect_pos );
    rect_get_shape( rect, &rect_shape );

    if ( rect_pos.x < 0 ) rect_pos.x = 0;
    if ( rect_pos.y < 0 ) rect_pos.y = 0;
    if ( ( rect_pos.x + rect_shape.x ) > WORLD_WIDTH )
        rect_pos.x = WORLD_WIDTH - rect_shape.x;
    if ( ( rect_pos.y + rect_shape.y ) > WORLD_HEIGHT )
        rect_pos.y = WORLD_HEIGHT - rect_shape.y;

    rect_set_pos( rect, rect_pos );
    rect_set_shape( rect, rect_shape );
}
