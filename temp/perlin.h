#ifndef PERLIN_H
#define PERLIN_H

typedef unsigned char byte;

bool get_perlin(float *out, int width, int height, float freq, int depth);
bool get_binary_perlin(float *out, int width, int height, float freq, int depth, float threshold);

#endif