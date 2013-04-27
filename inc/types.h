#pragma once

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
    int something_will_be_here_later;
} Assets;
