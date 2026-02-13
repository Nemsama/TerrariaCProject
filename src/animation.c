#include <animation.h>


void set_frame( sprite_t* sprite, int animation_index, int frame_number ) {
    sprite->src_rect.x = animation_index * sprite->src_rect.w;
    sprite->src_rect.y = frame_number    * sprite->src_rect.h;
}

void animate( sprite_t* sprite, int nb_frames ) {
    sprite->src_rect.y = ( sprite->src_rect.y + sprite->src_rect.h ) % ( nb_frames * sprite->src_rect.h );
}

void set_animation( sprite_t* sprite, int animation_index ) {
    sprite->src_rect.x = animation_index * sprite->src_rect.w;
}
