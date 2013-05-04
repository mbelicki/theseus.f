#pragma once

#define INTIFY(c,s) \
    (SDL_MapRGB((s)->format, (c).r, (c).g, (c).b))

typedef struct _Color {
    unsigned char a, r, g, b;
} Color;

typedef struct _Point {
    int x;
    int y;
} Point;

#define POINT_EQ(p1, p2)\
    ((p1).x == (p2).x && (p1).y == (p2).y)

#define MAX_ENEMY_COUNT 8

