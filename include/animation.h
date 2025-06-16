#ifndef ANIMATION_H
#define ANIMATION_H

#include <sprite.h>

void set_frame( sprite_t* sprite, int animation_index, int frame_number );
void animate( sprite_t* sprite, int nb_frames );

void set_animation( sprite_t* sprite, int animation_index );

#endif