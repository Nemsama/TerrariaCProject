#ifndef UTILS_H
#define UTILS_H

#include <common.h>
#include <rect.h>

void prevent_exit( rect_t* rect, rect_t* boundaries );
void prevent_world_exit( rect_t* rect );

#endif