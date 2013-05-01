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

typedef enum _EntityFlags { ENTITY_HAS_HIT_WALL = 1
                          } EntityFlags;

typedef struct _Entity {
    Point       position;
    Point       destination;
    double      movement_delta;
    EntityFlags flags;
} Entity;

static inline Entity * init_entity( Entity * const e
                                  , const int x
                                  , const int y) {
    e->position.x = e->destination.x = x; 
    e->position.y = e->destination.y = y; 
    e->movement_delta = 0.0;
    e->flags = 0;
    return e;
}

#define ENTITY_IN(a) ((a).entity)
#define ENTITY_OF(a) ((a)->entity)

#define IS_ENTITY Entity entity

typedef enum _EnemyFlags { ENEMY_MOVING_HORIZONTAL = 1
                         , ENEMY_GOING_BACK        = 2
                         } EnemyFlags;

typedef struct _Enemy {
    IS_ENTITY;
    EnemyFlags flags;
} Enemy;

static inline Enemy * init_enemy( Enemy * const e
                                , const int x
                                , const int y) {
    init_entity( & e->entity, x, y );
    e->flags = 0;
    return e;
}

typedef enum _Item { ITEM_POTATO
                   , ITEM_SWORD
                   , ITEM_POISON
                   , ITEM_CHAINSAW
                   , MAX_ITEM = ITEM_CHAINSAW
                   } Item;

typedef struct _Player {
    IS_ENTITY;
} Player;

#define ENEMY_SPEED 5.0
#define MAX_ENEMY_COUNT 8

typedef enum _TileType { TILE_FLOOR  =  0
                       , TILE_WALL   =  1
                       , TILE_TRAP   =  2
                       , TILE_STRING =  4
                       , TILE_TRADER =  8
                       , TILE_BOSS   = 16
                       } TileType;

typedef enum _MapFlags { MAP_DYNAMIC = 1
                       } MapFlags;

typedef struct _Map {
    TileType *data;
    size_t    width;
    size_t    height;
    
    MapFlags  flags;
} Map;

typedef enum _StateType { STATE_SPLASH
                        , STATE_INTRO
                        , STATE_LOST
                        , STATE_WON
                        , STATE_OVER
                        , STATE_BOSS
                        , STATE_TRADE
                        , STATE_FREE
                        , STATE_MAX = STATE_FREE
                        } StateType;

typedef struct _State {
    StateType type;
    StateType next_type;

    Map       map;
    Enemy    *map_enemies;
    size_t    map_enemy_count;

    int current_level_no;

    Point  player_pos;
    Point  player_goto;
    Point  player_prev_pos;
    double player_move_delta;
    double player_move_speed;
    Item   player_item;

    Item   trader_item;

    double marquee_amount;
    int    is_marquee_closing;

    int requested_quit;
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
    Color splash_color;

    Uint32 *wall_tex;
    Uint32 *floor_tex;
    Uint32 *trap_tex;
    Uint32 *string_tex;
    Uint32 *player_tex;
    Uint32 *enemy_tex;

    SDL_Surface *image_dangerous;
    SDL_Surface *image_trader;
    SDL_Surface *image_boss;
    SDL_Surface *image_font;
} Assets;
