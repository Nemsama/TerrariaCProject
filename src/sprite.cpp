#include <sprite.h>


sprite_t* sprite_init( const char* texture_path, SDL_Renderer* renderer, int src_x, int src_y, int texture_width, int texture_height ) {
    // Allocate memory for the sprite structure
    sprite_t* sprite = (sprite_t*)calloc( 1, sizeof(*sprite) );
    if ( sprite == NULL ) {
        perror("Failed to allocate memory for sprite structure");
        return NULL;
    }

    // Load the texture from the file
    SDL_Surface* surface = IMG_Load( texture_path );
    if ( surface == NULL ) {
        perror("Failed to load texture for sprite");
        free(sprite);
        return NULL;
    }

    // Create the texture from the surface
    sprite->texture = SDL_CreateTextureFromSurface( renderer, surface );
    SDL_FreeSurface( surface );

    if ( sprite->texture == NULL ) {
        perror("Failed to create texture from surface for sprite");
        free(sprite);
        return NULL;
    }

    // Set the source rectangle to the entire texture
    sprite->src_rect.x = src_x;
    sprite->src_rect.y = src_y;
    sprite->src_rect.w = texture_width;
    sprite->src_rect.h = texture_height;

    // Set the destination rectangle to the specified position and size
    sprite->dest_rect.x = 0;
    sprite->dest_rect.y = 0;
    sprite->dest_rect.w = texture_width;
    sprite->dest_rect.h = texture_height;

    return sprite;
}

void sprite_destroy( sprite_t* sprite ) {
    if ( !sprite ) return;

    SDL_DestroyTexture( sprite->texture );
    free(sprite);
}

void sprite_set_pos( sprite_t* sprite, int x, int y ) {
    if ( sprite == NULL ) return;

    sprite->dest_rect.x = x;
    sprite->dest_rect.y = y;
}

void sprite_set_scale( sprite_t* sprite, int width, int height ) {
    if ( sprite == NULL ) return;

    sprite->dest_rect.w = width;
    sprite->dest_rect.h = height;
}
void sprite_scale(  sprite_t* sprite, float scale_x, float scale_y ) {
    if ( sprite == NULL ) return;

    sprite->dest_rect.w *= scale_x;
    sprite->dest_rect.h *= scale_y;
}

void sprite_render( sprite_t* sprite, SDL_Renderer* renderer ) {
    // Render the sprite to the screen
    SDL_RenderCopy( renderer, sprite->texture, &sprite->src_rect, &sprite->dest_rect );
}

void sprite_set_texture( sprite_t* sprite, SDL_Texture* texture ) {
    if ( sprite == NULL ) return;

    // Destroy the old texture
    SDL_DestroyTexture( sprite->texture );

    // Set the new texture
    sprite->texture = texture;
}

void sprite_set_texture_path( sprite_t* sprite, const char* texture_path, SDL_Renderer* renderer ) {
    if ( sprite == NULL ) return;

    // Load the new texture from the file
    SDL_Surface* surface = IMG_Load( texture_path );
    if ( surface == NULL ) {
        perror("Failed to load texture for sprite");
        return;
    }

    // Create the new texture from the surface
    SDL_Texture* new_texture = SDL_CreateTextureFromSurface( renderer, surface );
    SDL_FreeSurface( surface );

    if ( new_texture == NULL ) {
        perror("Failed to create texture from surface for sprite");
        return;
    }

    // Set the new texture
    sprite_set_texture( sprite, new_texture );
}
