#ifndef PERLIN_H
#define PERLIN_H

#define PERLIN_WIDTH  100
#define PERLIN_HEIGHT 100

typedef unsigned char byte;

float perlin2d(float x, float y, float freq, int depth, int seed);
float blurred_noise2d(float x, float y, float freq, int depth, int seed, float offset);

float linear_fade(int x, int y, int width, int height, float slope);
float exponential_fade(int x, int y, int width, int height, float amp);

bool get_perlin(float out[PERLIN_WIDTH][PERLIN_HEIGHT], int width, int height, float freq, int depth, int seed);
bool get_binary_perlin(float out[PERLIN_WIDTH][PERLIN_HEIGHT], int width, int height, float freq, int depth, int seed, float threshold);

void apply_threshold(float out[PERLIN_WIDTH][PERLIN_HEIGHT], int width, int height, float threshold);

#endif