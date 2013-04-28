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

typedef struct _Enemy {
    Point  position;
    Point  destination;
    double movement_delta;
    int    is_moving_horizontal;
    int    is_going_back;
} Enemy;

#define ENEMY_SPEED 7.0
#define MAX_ENEMY_COUNT 4

typedef enum _TileType { TILE_STRING = -2
                       , TILE_TRAP   = -1
                       , TILE_FLOOR  =  0
                       , TILE_WALL   =  1
                       } TileType;

typedef enum _StateType { STATE_SPLASH
                        , STATE_INTRO
                        , STATE_LOST
                        , STATE_FREE
                        , STATE_MAX = STATE_FREE
                        } StateType;

typedef struct _State {
    StateType type;
    StateType next_type;

    int       map_is_dynamic;
    TileType *map_data;
    int       map_width;
    int       map_height;
    Enemy    *map_enemies;
    size_t    map_enemy_count;

    int current_level_no;

    Point  player_pos;
    Point  player_goto;
    Point  player_prev_pos;
    double player_move_delta;
    double player_move_speed;

    double marquee_amount;
    int    is_marquee_closing;
} State;

#define MARQUEE_SPEED 3.2

typedef enum _Key {
    KEY_UP    = 1,
    KEY_DOWN  = 2,
    KEY_LEFT  = 4,
    KEY_RIGHT = 8
} Key;

typedef struct _Assets {
    int tex_width;
    int tex_height;

    Color level_color;

    Uint32 *wall_tex;
    Uint32 *floor_tex;
    Uint32 *trap_tex;
    Uint32 *string_tex;
    Uint32 *player_tex;
    Uint32 *enemy_tex;

    SDL_Surface *image_dangerous;
    SDL_Surface *image_dead;
} Assets;
