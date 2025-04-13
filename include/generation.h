#ifndef GENERATION_H
#define GENERATION_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

void flip_terrain( int* output, int* input, int size );
void convert_terrain_to_world_height( int* world_height, int size, int surface_height );

void plain( int* terrain, int size, int starting_height );
void rough_plain( int* terrain, int size, int starting_height );

void up_slope( int* terrain, int size, int starting_height );
void down_slope( int* terrain, int size, int starting_height );
void sharp_up_slope( int* terrain, int size, int starting_height );
void sharp_down_slope( int* terrain, int size, int starting_height );

void small_mountain( int* terrain, int size, int starting_height );
void big_mountain( int* terrain, int size, int starting_height );
void down_mountain( int* terrain, int size, int starting_height );
void up_mountain( int* terrain, int size, int starting_height );

void generate_world_height( int* world_height, int size, int starting_height, int up_limit, int down_limit );

#endif