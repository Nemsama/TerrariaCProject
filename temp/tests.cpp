#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>

typedef struct a {
    int x;
    int y;
} a_t;

void hellow_a(a_t a) {
    printf("Hello a: %d %d\n", a.x, a.y);
}

int main() {
    printf("0 = %d\n", (int)(-0.2));
}