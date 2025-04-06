#include <sprite.h>


sprite_t* sprite_init( const char* texture_path, SDL_Renderer* renderer, int src_x, int src_y, int width, int height ) {
    // Allocate memory for the sprite structure
    sprite_t* sprite = (sprite_t*)calloc( 1, sizeof(*sprite) );
    if ( sprite == NULL ) {
        perror("Failed to allocate memory for sprite structure");
        exit( EXIT_FAILURE );
    }

    // Load the texture from the file
    SDL_Surface* surface = IMG_Load( texture_path );
    if ( surface == NULL ) {
        perror("Failed to load texture for sprite");
        free(sprite);
        exit( EXIT_FAILURE );
    }

    // Create the texture from the surface
    sprite->texture = SDL_CreateTextureFromSurface( renderer, surface );
    SDL_FreeSurface( surface );

    if ( sprite->texture == NULL ) {
        perror("Failed to create texture from surface for sprite");
        free(sprite);
        exit( EXIT_FAILURE );
    }

    // Set the source rectangle to the entire texture
    sprite->src_rect.x = src_x;
    sprite->src_rect.y = src_y;
    sprite->src_rect.w = width;
    sprite->src_rect.h = height;

    // Set the destination rectangle to the specified position and size
    sprite->dest_rect.x = 0;
    sprite->dest_rect.y = 0;
    sprite->dest_rect.w = width;
    sprite->dest_rect.h = height;

    return sprite;
}

void sprite_free( sprite_t* sprite ) {
    SDL_DestroyTexture( sprite->texture );
    free(sprite);
}

void sprite_set_position( sprite_t* sprite, int x, int y ){
    if ( sprite == NULL ) return;

    sprite->dest_rect.x = x;
    sprite->dest_rect.y = y;
}

void sprite_render( sprite_t* sprite, SDL_Renderer* renderer ){
    // Render the sprite to the screen
    SDL_RenderCopy( renderer, sprite->texture, &sprite->src_rect, &sprite->dest_rect );
}
