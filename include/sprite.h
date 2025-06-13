#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <camera.h>
// dependancies : lower

#define DESTROY_TEXTURE true
#define KEEP_TEXTURE false

typedef struct sprite {
    SDL_Texture* texture;
    SDL_Rect src_rect;  // select the part of the texture to draw
    float scale;        // scale the src rect to modify screen size
    SDL_Rect dest_rect; // used to position the sprite on the screen (and stretch the texture)
} sprite_t;

sprite_t* sprite_init( const char* texture_path, SDL_Renderer* renderer, int src_x, int src_y, float scale, int width, int height );
sprite_t* sprite_copy( const sprite_t* sprite/* , SDL_Renderer* renderer */ );
sprite_t* sprite_init_texture( SDL_Texture* texture, int src_x, int src_y, float scale, int width, int height );

SDL_Texture* sprite_destroy( sprite_t* sprite, bool destroy_texture );

void sprite_set_pos( sprite_t* sprite, int x, int y );
void sprite_set_center( sprite_t* sprite, int x, int y );
void sprite_get_pos( sprite_t* sprite, int* x, int* y );
void sprite_get_center( sprite_t* sprite, int* x, int* y );

void sprite_set_scale( sprite_t* sprite, float scale );
void sprite_scale( sprite_t* sprite, float scale_mult );
void sprite_get_shape( sprite_t* sprite, int* width, int* height );

int sprite_get_width ( sprite_t* sprite );
int sprite_get_height( sprite_t* sprite );

// sprite_render only print the sprite's texture on the renderer according to its source and dest rects
void sprite_render( sprite_t* sprite, SDL_Renderer* renderer );
void sprite_render_background( sprite_t* background, camera_t* camera, SDL_Renderer* renderer );

void sprite_set_texture( sprite_t* sprite, SDL_Texture* texture );

#endif