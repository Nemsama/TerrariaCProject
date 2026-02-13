#include <utils.h>

float sign( float x ) {
    if ( x > 0 ) return 1.0f;
    if ( x < 0 ) return -1.0f;
    return 0.0f;
}

float running_force( float speed, float acceleration, float max_speed ) {
    return acceleration * ( max_speed - speed );
}

bool prevent_exit( rect_t* rect, rect_t* boundaries ) {
    if ( !rect || !boundaries ) return false;
    bool moved = false;

    vector2_t rect_pos;
    vector2_t rect_shape;
    rect_get_pos( rect, &rect_pos );
    rect_get_shape( rect, &rect_shape );
    vector2_t boundaries_pos;
    vector2_t boundaries_shape;
    rect_get_pos( boundaries, &boundaries_pos );
    rect_get_shape( boundaries, &boundaries_shape );

    if ( rect_pos.x < boundaries_pos.x ) { rect_pos.x = boundaries_pos.x; moved = true; }
    if ( rect_pos.y < boundaries_pos.y ) { rect_pos.y = boundaries_pos.y; moved = true; }
    if ( ( rect_pos.x + rect_shape.x ) > ( boundaries_pos.x + boundaries_shape.x ) ) {
        rect_pos.x = ( boundaries_pos.x + boundaries_shape.x ) - rect_shape.x;
        moved = true;
    }
    if ( ( rect_pos.y + rect_shape.y ) > ( boundaries_pos.y + boundaries_shape.y ) ) {
        rect_pos.y = ( boundaries_pos.y + boundaries_shape.y ) - rect_shape.y;
        moved = true;
    }

    if ( moved ) {
        rect_set_pos( rect, rect_pos );
    }
    return moved;
}

bool prevent_world_exit( rect_t* rect ) {
    if ( !rect ) return false;
    bool moved = false;

    vector2_t rect_pos;
    vector2_t rect_shape;
    rect_get_pos( rect, &rect_pos );
    rect_get_shape( rect, &rect_shape );

    if ( rect_pos.x < 0 ) { rect_pos.x = 0; moved = true; }
    if ( rect_pos.y < 0 ) { rect_pos.y = 0; moved = true; }
    if ( ( rect_pos.x + rect_shape.x ) > WORLD_WIDTH ) {
        rect_pos.x = WORLD_WIDTH - rect_shape.x;
        moved = true;
    }
    if ( ( rect_pos.y + rect_shape.y ) > WORLD_HEIGHT ) {
        rect_pos.y = WORLD_HEIGHT - rect_shape.y;
        moved = true;
    }

    if ( moved ) {
        rect_set_pos( rect, rect_pos );
    }
    return moved;
}
