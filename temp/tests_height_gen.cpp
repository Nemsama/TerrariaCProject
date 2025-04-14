#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

// plain parameters : 
//        up_proba = 0.1f;
//      down_proba = 0.1f;
// min_level_width = 6;
// max_jump_height = 1;
//      jump_proba = 0.0f;

// rough plain parameters : 
//        up_proba = 0.18f;
//      down_proba = 0.18f;
// min_level_width = 1;
// max_jump_height = 2;
//      jump_proba = 0.8f;

// slope parameters :
//        up_proba = 0.45f;
//      down_proba = 0.1f;
// min_level_width = 1;
// max_jump_height = 5;
//      jump_proba = 0.05f;

// sharp slope parameters :
//        up_proba = 0.8f;
//      down_proba = 0.0f;
// min_level_width = 1;
// max_jump_height = 5;
//      jump_proba = 0.4f;

void random_terrain( int* terrain, int size, int starting_height, float up_proba, float down_proba, int min_level_width, int max_jump_height, float jump_proba ) {
    int i = 0;
    int level_width = 1;
    int height = starting_height;
    float random_value;

    while ( i < size ) {
        if ( level_width < min_level_width ) {
            terrain[i] = height;
            level_width++;
            i++;
            continue;
        }

        random_value = (float)rand() / (float)RAND_MAX;
        if ( random_value < up_proba ) {
            height++;
            for ( int j = 1; j < max_jump_height; j++ ) {
                if ( rand()/(float)RAND_MAX < jump_proba ) height++;
            }
            terrain[i] = height;
            level_width = 1;
        }
        else if ( random_value < up_proba + down_proba ) {
            height--;
            for ( int j = 1; j < max_jump_height; j++ ) {
                if ( rand()/(float)RAND_MAX < jump_proba ) height--;
            }
            terrain[i] = height;
            level_width = 1;
        }
        else {
            terrain[i] = height;
        }
        i++;
    }
}

// good mountain parameters :
//            size = 50;
// mountain_height = 30.0f;
//     noise_proba = 0.3f;
//        flatness = 0.1f;
//     left_offset = 0;
//    right_offset = 0;

// high mountain parameters :
//            size = 55 : 60;
// mountain_height = 30.0f;
//     noise_proba = 0.3f;
//        flatness = 0.1f;
//     left_offset = -3;
//    right_offset =  3;

float mountain_func( float x, float height, float width ) {
    // used function is x(1-x) rescaled with height and width
    return 4 * height/width * x * ( 1 - x/width );
}
// flatness is the probability to offset the mountain in the opposite direction (downward when going up and upward when going down)
// negative flatness will have the opposite effect
void random_mountain( int* terrain, int size, int starting_height, int mountain_height, float noise_proba, float flatness, int left_offset, int right_offset ) {
    int i = 0;
    float x = left_offset;
    float mountain_width = (float)size + left_offset - right_offset - 1;
    int vertical_offset = 0;
    int first_height = starting_height - (int)mountain_func( x, mountain_height, mountain_width );

    while ( i < size ) {
        if ( rand()/(float)RAND_MAX < noise_proba ) {
            vertical_offset += rand()%2 == 0 ? -1 : 1;
        }
        if ( rand()/(float)RAND_MAX < fabs( flatness ) ) {
            if ( flatness > 0 ) {
                vertical_offset += ( x > size/2.0f ) ? 1 : -1;
            }
            else {
                vertical_offset += ( x > size/2.0f ) ? -1 : 1;
            }
        }

        terrain[i] = first_height + (int)mountain_func( x, mountain_height, mountain_width ) + vertical_offset;
        i++;
        x += 1.0f;
    }
}




void show_terrain( int* terrain, int size, int window_width = 800, int window_height = 600 ) {
    if ( terrain == NULL ) return;

    int block_size = window_width / size;

    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    SDL_Window* window = SDL_CreateWindow("Terrain", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
    if ( window == NULL ) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if ( renderer == NULL ) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect;
    rect.w = block_size-1;
    rect.h = block_size-1;
    for ( int i = 0; i < size; i++ ) {
        rect.x = i * block_size;
        rect.y = window_height - terrain[i] * block_size;
        for ( int j = 0; j < terrain[i]; j++ ) {
            rect.y += block_size;

            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_RenderPresent(renderer);
    while (1) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        if ( event.type == SDL_QUIT ) {
            break;
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void test_terrain_func( void ) {
    int terrain[100];
    int size = 8;
    int starting_height = 4;
    int window_width = 300;
    int window_height = 800;
    float      up_proba = 0.8f;
    float    down_proba = 0.0f;
    int min_level_width = 1;
    int max_jump_height = 5;
    float    jump_proba = 0.4f;

    random_terrain( terrain, size, starting_height, up_proba, down_proba, min_level_width, max_jump_height, jump_proba );

    show_terrain( terrain, size, window_width, window_height );
}
void test_mountain_func( void ) {
    int terrain[100];
    int size = 50;
    int starting_height = 3;
    int window_width = 800;
    int window_height = 600;
    float mountain_height = 30.0f;
    float noise_proba = 0.3f;
    float flatness = 0.1f;
    int left_offset = -3;
    int right_offset = -25;

    random_mountain( terrain, size, starting_height, mountain_height, noise_proba, flatness, left_offset, right_offset );

    show_terrain( terrain, size, window_width, window_height );
}

int main( int argc, char* argv[] ) {
    srand( time(NULL) );
    // test_terrain_func();
    test_mountain_func();
    return 0;
}
