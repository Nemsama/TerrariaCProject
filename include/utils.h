#ifndef UTILS_H
#define UTILS_H

#include <common.h>
#include <rect.h>
#include <vector.h>

float sign( float x );

float running_force( float speed, float acceleration, float max_speed );

// Prevents the rectangle from going outside the boundaries
// returns true if the rectangle was moved
bool prevent_exit( rect_t* rect, rect_t* boundaries );
bool prevent_world_exit( rect_t* rect );

#endif