#ifndef GENERATION_H
#define GENERATION_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include <common.h>
#include <perlin.h>

void flip_terrain( int* output, int* input, int size );
void convert_terrain_to_world_height( int* world_height, int size, int surface_height );

void plain( int* terrain, int size, int starting_height );
void rough_plain( int* terrain, int size, int starting_height );

void up_slope( int* terrain, int size, int starting_height );
void down_slope( int* terrain, int size, int starting_height );
void sharp_up_slope( int* terrain, int size, int starting_height );
void sharp_down_slope( int* terrain, int size, int starting_height );

void small_mountain( int* terrain, int size, int starting_height );
void high_mountain( int* terrain, int size, int starting_height );
void down_mountain( int* terrain, int size, int starting_height );
void up_mountain( int* terrain, int size, int starting_height );

void generate_world_height( int* world_height, int size, int starting_height, int up_limit, int down_limit );

// paint world with perlin

void smooth_transition( Uint8 ** world_mask, int transition_height, int offset, float frequency, int depth, float start_threshold, float end_threshold, long unsigned int seed );

void paint_with_perlin( Uint8 ** world_mask, float frequency, int depth, float threshold, long unsigned int seed, int start_height, int end_height );
/*
void paint_surface_perlin    ( Uint8 ** world_mask, long unsigned int seed );
void paint_underground_perlin( Uint8 ** world_mask, long unsigned int seed );
void paint_cavern_perlin     ( Uint8 ** world_mask, long unsigned int seed );
*/

// void dig_caves_perlin( Uint8 ** world_mask, long unsigned int seed );

float get_box_filter_value     ( Uint8 ** world_mask, int x, int y );
float get_gaussian_filter_value( Uint8 ** world_mask, int x, int y );

#endif