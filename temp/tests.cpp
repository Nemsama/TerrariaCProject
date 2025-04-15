#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>

#include "perlin.h"

#define PIXEL_SIZE 3

// g++ temp/tests.cpp temp/perlin.cpp -o temp/tests -lSDL2 -lm

float gaussian_weight( int x, int y ) {
    if ( x == 0 && y == 0 ) return 4.0f;
    else if ( fabs( x ) + fabs( y ) == 1 ) return 2.0f;
    else return 1.0f;
}
float get_value( float out[WIDTH][HEIGHT], int x, int y ) {
    if ( x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT ) return 0.5f;
    return out[x][y];
}
float gaussian_blur( float out[WIDTH][HEIGHT], int x, int y ) {
    if (!out) return 0.0f;

    float sum = 0.0f;
    float weight_sum = 0.0f;
    float weight;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            weight = gaussian_weight( i, j );
            sum += get_value( out, x+i, y+j ) * weight;
            weight_sum += weight;
        }
    }

    return sum / weight_sum;
}
float box_blur( float out[WIDTH][HEIGHT], int x, int y ) {
    if (!out) return 0.0f;

    float sum = 0.0f;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            sum += get_value( out, x+i, y+j );
        }
    }

    return sum / 9.0f;
}
void apply_blur( float out[WIDTH][HEIGHT], int width, int height, int filter ) {
    if (!out) return;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if ( filter == 1 ) out[x][y] = gaussian_blur( out, x, y );
            if ( filter == 0 ) out[x][y] = box_blur( out, x, y );
            out[x][y] = roundf( out[x][y] );
        }
    }
}

#define BOX_BLUR 0
#define GAUSSIAN_BLUR 1
int main( int argc, char* argv[] ) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Perlin Noise", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    float noise[WIDTH][HEIGHT];
    int seed = time(NULL) % 256;
    
    get_perlin(noise, WIDTH, HEIGHT, 0.05f, 5, seed);
    apply_threshold(noise, WIDTH, HEIGHT, 0.67f);
    apply_blur(noise, WIDTH, HEIGHT, BOX_BLUR);

    SDL_Rect rect;
    rect.w = PIXEL_SIZE;
    rect.h = PIXEL_SIZE;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            Uint8 color = (Uint8)(noise[x][y] * 255);
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            rect.x = x * PIXEL_SIZE;
            rect.y = y * PIXEL_SIZE;
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_RenderPresent(renderer);
    
    while (1) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// pretty good cave parameters :
// frequency = 0.08f;
// depth     = 5;
// threshold = 0.63f;


// surface stone parameters :
// frequency = 0.1f;
// depth     = 5;
// threshold = 0.71f;

// underground stone parameters :
// frequency = 0.16f;
// depth     = 5;
// threshold = 0.62f;

// cavern dirt parameters :
// frequency = 0.09f;
// depth     = 5;
// threshold = 0.7f;
