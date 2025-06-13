#include <sprite.h>


sprite_t* sprite_init( const char* texture_path, SDL_Renderer* renderer, int src_x, int src_y, float scale, int texture_width, int texture_height ) {
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

    sprite->scale = scale;

    // Set the destination rectangle to the specified position and size
    sprite->dest_rect.x = 0;
    sprite->dest_rect.y = 0;
    sprite->dest_rect.w = texture_width;
    sprite->dest_rect.h = texture_height;

    return sprite;
}
sprite_t* sprite_init_texture( SDL_Texture* texture, int src_x, int src_y, float scale, int width, int height ) {
    if ( texture == NULL ) {
        perror("Texture is NULL");
        return NULL;
    }
    // Allocate memory for the sprite structure
    sprite_t* sprite = (sprite_t*)calloc( 1, sizeof(*sprite) );
    if ( sprite == NULL ) {
        perror("Failed to allocate memory for sprite structure");
        return NULL;
    }

    // Set the texture
    sprite->texture = texture;

    // Set the source rectangle to the entire texture
    sprite->src_rect.x = src_x;
    sprite->src_rect.y = src_y;
    sprite->src_rect.w = width;
    sprite->src_rect.h = height;

    sprite->scale = scale;

    // Set the destination rectangle to the specified position and size
    sprite->dest_rect.x = 0;
    sprite->dest_rect.y = 0;
    sprite->dest_rect.w = width;
    sprite->dest_rect.h = height;

    return sprite;
}

SDL_Texture* copy_texture( SDL_Texture* source, SDL_Renderer* renderer ) {
    int w, h;
    Uint32 format;
    int access;

    // Get information about the source texture
    if (SDL_QueryTexture(source, &format, &access, &w, &h) != 0) {
        SDL_Log("SDL_QueryTexture failed: %s", SDL_GetError());
        return NULL;
    }

    // Create a new texture with the same format and size
    SDL_Texture* copy = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_TARGET, w, h);
    if (!copy) {
        SDL_Log("SDL_CreateTexture failed: %s", SDL_GetError());
        return NULL;
    }

    // Save the current render target
    SDL_Texture* old_target = SDL_GetRenderTarget(renderer);

    // Set the new texture as the render target
    SDL_SetRenderTarget(renderer, copy);
    
    /* 
    // Clear it if you want (optional)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    */
    
    // Copy the source texture onto the new texture
    SDL_RenderCopy(renderer, source, NULL, NULL);

    // Restore the previous render target
    SDL_SetRenderTarget(renderer, old_target);

    return copy;
}

sprite_t* sprite_copy( const sprite_t* sprite/* , SDL_Renderer* renderer */ ) {
    SDL_Texture* new_texture = sprite->texture;
    // the texture is the same but will be printed on the screen at different position depending on the used sprite
    return sprite_init_texture( new_texture, sprite->src_rect.x, sprite->src_rect.y, sprite->scale, sprite->src_rect.h, sprite->src_rect.h );
}

SDL_Texture* sprite_destroy( sprite_t* sprite, bool destroy_texture ) {
    if ( !sprite ) return NULL;

    if ( destroy_texture ) {
        SDL_DestroyTexture( sprite->texture );
        free(sprite);
        return NULL;
    }
    else {
        SDL_Texture* texture = sprite->texture;
        free(sprite);
        return texture;
    }
}

void sprite_set_pos( sprite_t* sprite, int x, int y ) {
    if ( sprite == NULL ) return;

    sprite->dest_rect.x = x;
    sprite->dest_rect.y = y;
}
void sprite_set_center( sprite_t* sprite, int x, int y ) {
    if ( sprite == NULL ) return;

    sprite->dest_rect.x = x - (sprite->dest_rect.w / 2);
    sprite->dest_rect.y = y - (sprite->dest_rect.h / 2);
}
void sprite_get_pos( sprite_t* sprite, int* x, int* y ) {
    if ( sprite == NULL ) return;

    if ( x ) *x = sprite->dest_rect.x;
    if ( y ) *y = sprite->dest_rect.y;
}
void sprite_get_center( sprite_t* sprite, int* x, int* y ) {
    if ( sprite == NULL ) return;

    if ( x ) *x = sprite->dest_rect.x + (sprite->dest_rect.w / 2);
    if ( y ) *y = sprite->dest_rect.y + (sprite->dest_rect.h / 2);
}

void sprite_set_scale( sprite_t* sprite, float scale ) {
    if ( sprite == NULL ) return;

    sprite->scale = scale;

    sprite->dest_rect.w = sprite->scale * sprite->src_rect.w;
    sprite->dest_rect.h = sprite->scale * sprite->src_rect.h;
}
void sprite_scale( sprite_t* sprite, float scale_mult ) {
    if ( sprite == NULL ) return;

    sprite->scale *= scale_mult;
}
void sprite_get_shape( sprite_t* sprite, int* width, int* height ) {
    if ( sprite == NULL ) return;

    if ( width ) *width = sprite->dest_rect.w;
    if ( height ) *height = sprite->dest_rect.h;
}
int sprite_get_width ( sprite_t* sprite ) {
    if ( sprite ) return sprite->dest_rect.w;
    return 0;
}
int sprite_get_height( sprite_t* sprite ) {
    if ( sprite ) return sprite->dest_rect.h;
    return 0;
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

void sprite_render( sprite_t* sprite, SDL_Renderer* renderer ) {
    if ( sprite == NULL || renderer == NULL ) return;

    SDL_RenderCopy( renderer, sprite->texture, &sprite->src_rect, &sprite->dest_rect );
}

void sprite_render_background( sprite_t* background, camera_t* camera, SDL_Renderer* renderer ) {
    if ( background == NULL || camera == NULL || renderer == NULL ) return;
    
    // set the right scale for the background
    SDL_Rect window_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    camera_scale_sprite( camera, &background->dest_rect, background->scale, &window_rect );

    // center the background
    camera_center_sprite( camera, &background->dest_rect );

    // Render the background to the screen
    SDL_RenderCopy( renderer, background->texture, &background->src_rect, &background->dest_rect );
}
