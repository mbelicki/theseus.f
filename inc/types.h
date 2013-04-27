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

typedef struct _State {

    int       *map_data;
    int        map_width;
    int        map_height;

    Point      player_pos;
} State;

typedef enum _Key {
    KEY_UP    = 1,
    KEY_DOWN  = 2,
    KEY_LEFT  = 4,
    KEY_RIGHT = 8
} Key;

typedef struct _Assets {
    int tex_width;
    int tex_height;

    Uint32 *wall_tex;
    Uint32 *floor_tex;
} Assets;
