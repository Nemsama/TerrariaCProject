#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>

#include "perlin.h"

#define PIXEL_SIZE 4


float gaussian_weight( int x, int y ) {
    if ( x == 0 && y == 0 ) return 4.0f;
    else if ( fabs( x ) + fabs( y ) == 1 ) return 2.0f;
    else return 1.0f;
}
float gaussian_blur( float out[WIDTH][HEIGHT], int x, int y ) {
    if (!out) return 0.0f;

    float sum = 0.0f;
    float weight_sum = 0.0f;
    float weight;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            weight = gaussian_weight( i, j );
            sum += out[x + i][y + j] * weight;
            weight_sum += weight;
        }
    }

    return sum / weight_sum;
}
void apply_gaussian_blur( float out[WIDTH][HEIGHT], int width, int height ) {
    if (!out) return;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            out[x][y] = gaussian_blur( out, x, y );
            out[x][y] = roundf( out[x][y] );
        }
    }
}

int main( int argc, char* argv[] ) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Perlin Noise", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    float noise[WIDTH][HEIGHT];
    int seed = time(NULL) % 256;
    
    get_perlin(noise, WIDTH, HEIGHT, 0.16f, 5, seed);
    apply_threshold(noise, WIDTH, HEIGHT, 0.71f);
    // apply_gaussian_blur(noise, WIDTH, HEIGHT);

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

// surface stone parameters :
// frequency = 0.1f;
// depth     = 4;
// threshold = 0.71f;

// underground stone parameters :
// frequency = 0.16f;
// depth     = 5;
// threshold = 0.62f;

// cavern dirt parameters :
// frequency = 0.09f;
// depth     = 5;
// threshold = 0.7f;
