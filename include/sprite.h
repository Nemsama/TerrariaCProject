#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct sprite {
    SDL_Texture* texture;
    SDL_Rect src_rect; // select the part of the texture to draw
    SDL_Rect dest_rect; // used to position the sprite on the screen (and stretch the texture)
} sprite_t;

sprite_t* sprite_init( const char* texture_path, SDL_Renderer* renderer, int src_x, int src_y, int width, int height );
sprite_t* sprite_init_texture( SDL_Texture* texture, int src_x, int src_y, int width, int height );

void sprite_destroy( sprite_t* sprite );

void sprite_set_pos( sprite_t* sprite, int x, int y );
void sprite_get_pos( sprite_t* sprite, int* x, int* y );

void sprite_set_scale( sprite_t* sprite, int width, int height );
void sprite_scale( sprite_t* sprite, float scale_x, float scale_y );
void sprite_get_scale( sprite_t* sprite, int* width, int* height );

void sprite_render( sprite_t* sprite, SDL_Renderer* renderer );

void sprite_set_texture( sprite_t* sprite, SDL_Texture* texture );

#endif