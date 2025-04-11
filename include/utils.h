#ifndef UTILS_H
#define UTILS_H

#include <common.h>
#include <rect.h>
#include <vector.h>

// Prevents the rectangle from going outside the boundaries
// returns true if the rectangle was moved
bool prevent_exit( rect_t* rect, rect_t* boundaries );
bool prevent_world_exit( rect_t* rect );

#endif