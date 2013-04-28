#include <SDL/SDL.h>

#include "levels.h"
#include "generator.h"

//    { $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $ 
//    };

#define $ 1
#define T -1
#define S -3
#define B -4
#define _ 0
#define v -5
#define h -6

static TileType hello[] =
    { $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $
    , $, _, _, _, T, _, $, _, _, _, $, _, $, _, _, _, $ 
    , $, _, $, _, _, _, $, _, $, _, $, _, $, _, $, _, $ 
    , $, _, $, T, T, _, $, $, $, _, $, _, $, $, $, _, $ 
    , $, _, _, _, T, _, _, _, _, _, $, _, _, $, _, v, $ 
    , $, $, $, _, $, T, T, T, _, $, $, $, _, $, $, _, $ 
    , $, _, h, _, _, _, _, T, _, _, _, $, _, _, _, _, $ 
    , $, $, $, $, $, $, _, $, $, $, _, $, $, $, $, _, $ 
    , _, _, _, _, _, $, _, _, _, $, _, T, $, v, T, _, _ 
    , $, $, $, $, _, $, $, $, _, $, _, T, T, _, $, _, $ 
    , $, _, h, _, _, _, _, $, _, $, _, _, _, _, $, _, $ 
    , $, $, $, $, $, $, _, T, _, T, T, T, $, _, $, _, $ 
    , $, _, _, _, _, _, _, T, _, _, _, _, $, _, $, _, $ 
    , $, _, T, T, T, T, T, $, $, $, $, _, $, _, _, _, $ 
    , $, _, T, _, _, _, T, _, _, _, $, _, $, _, $, _, $ 
    , $, _, _, _, T, _, _, _, $, _, _, _, $, _, $, _, $ 
    , $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $ 
    };

static TileType floor_is_lava[] =
    { T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T
    , T, _, _, _, _, _, T, _, T, _, T, _, _, _, T, _, T 
    , T, _, T, T, T, _, T, v, T, _, T, _, T, _, T, _, T 
    , T, _, T, _, _, _, T, _, _, _, T, _, T, _, T, _, T 
    , T, _, T, _, T, T, T, _, T, _, _, _, T, _, T, _, T 
    , T, _, T, _, T, v, T, _, T, T, T, T, T, _, T, _, $ 
    , T, _, T, _, T, _, T, _, T, _, _, _, _, _, _, h, $ 
    , T, _, T, _, T, _, _, _, T, _, T, T, T, $, $, $, $ 
    , _, _, T, _, _, _, T, _, T, _, _, _, S, _, _, _, _ 
    , T, _, T, _, T, _, _, _, T, _, T, T, T, $, $, $, $ 
    , T, _, T, _, T, _, T, _, T, h, _, _, _, _, _, _, $ 
    , T, _, T, _, T, _, T, _, T, T, T, T, T, _, T, _, T 
    , T, _, T, _, T, T, T, _, T, _, _, _, T, _, T, _, T 
    , T, _, T, _, _, _, T, _, _, _, T, _, T, _, T, _, T 
    , T, _, T, T, T, _, T, _, T, _, T, _, T, _, T, _, T 
    , T, _, _, _, _, _, T, v, T, _, T, _, _, _, T, _, T 
    , T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T 
    };

static TileType mutual_core[] =
    { $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $
    , $, v, $, _, _, _, T, _, _, _, T, _, _, _, $, v, $ 
    , $, _, $, _, _, _, T, _, _, _, T, _, _, _, $, _, $ 
    , $, _, $, $, $, _, T, _, _, _, T, _, $, $, $, _, $ 
    , $, _, $, v, $, _, T, _, _, _, T, _, $, v, $, _, $ 
    , $, _, _, _, $, $, $, _, _, _, $, $, $, _, $, _, $ 
    , $, _, $, _, $, v, $, $, $, $, $, v, $, _, $, _, $ 
    , $, _, $, _, $, _, $, _, T, _, $, _, $, _, $, _, $ 
    , _, _, $, _, _, _, _, _, S, _, _, _, _, _, $, _, _ 
    , $, _, $, _, $, _, $, _, T, _, $, _, $, _, $, _, $ 
    , $, _, $, _, $, _, $, $, $, $, $, _, $, _, $, _, $ 
    , $, _, $, _, $, $, $, _, _, _, $, $, $, _, _, _, $ 
    , $, _, $, _, $, _, T, _, _, _, T, _, $, _, $, _, $ 
    , $, _, $, $, $, _, T, _, _, _, T, _, $, $, $, _, $ 
    , $, _, $, _, _, _, T, _, _, _, T, _, _, _, $, _, $ 
    , $, v, $, _, _, _, T, _, _, _, T, _, _, _, $, v, $ 
    , $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $ 
    };

static TileType mess[] =
    { $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $
    , $, _, _, _, $, _, _, _, $, _, _, $, _, _, _, _, $ 
    , $, _, $, _, $, $, $, _, $, _, $, $, _, $, $, $, $ 
    , $, _, $, _, $, _, $, _, $, _, $, _, _, _, _, _, $ 
    , $, _, $, _, _, _, $, _, _, _, $, _, $, $, $, $, $ 
    , $, _, $, $, $, $, $, _, $, _, $, _, _, _, _, _, $ 
    , $, _, _, _, _, $, _, _, $, v, $, _, $, $, _, $, $ 
    , $, _, $, $, $, $, $, $, $, _, $, $, $, _, _, _, $ 
    , _, v, T, _, _, h, _, _, T, _, _, _, T, _, T, _, _ 
    , $, _, $, $, $, $, $, _, $, _, $, _, $, _, $, $, $ 
    , $, _, $, _, _, _, $, _, _, _, $, _, $, _, _, _, $ 
    , $, _, $, _, $, _, $, _, $, $, $, _, $, $, _, $, $ 
    , $, _, $, _, $, _, _, _, _, _, $, _, _, _, _, _, $ 
    , $, _, _, _, $, v, $, $, $, _, $, $, _, $, $, _, $ 
    , $, _, $, _, $, _, $, _, _, _, $, _, _, _, $, _, $ 
    , $, _, $, _, $, _, $, _, $, _, $, _, $, _, $, _, $ 
    , $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $ 
    };

static TileType sliders[] =
    { $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $
    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
    , $, _, $, $, $, $, $, $, $, $, $, $, $, _, $, $, $ 
    , $, _, $, h, _, _, _, _, _, _, _, _, _, _, _, _, $ 
    , $, _, $, $, $, _, $, T, $, T, $, T, $, $, $, $, $ 
    , $, _, $, _, h, _, _, _, _, _, _, _, _, _, _, _, $ 
    , $, _, $, $, $, $, $, T, $, T, $, T, $, _, $, $, $ 
    , $, _, $, _, _, h, _, _, _, _, _, _, _, _, $, _, $ 
    , _, _, $, $, $, $, $, _, $, $, $, $, $, $, $, _, _ 
    , $, T, $, _, _, _, $, _, $, _, _, _, $, S, _, _, $ 
    , $, _, $, _, $, _, $, _, _, _, $, $, $, _, $, $, $ 
    , $, _, $, _, $, _, _, _, $, $, $, _, _, _, _, _, $ 
    , $, _, $, _, $, _, $, _, _, _, _, v, $, $, $, $, $ 
    , $, _, $, _, $, _, $, T, $, T, $, _, $, _, _, _, $ 
    , $, _, _, _, $, _, $, _, $, _, $, _, _, _, $, _, $ 
    , $, _, $, _, $, _, _, _, _, _, $, _, $, _, $, _, $ 
    , $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $ 
    };

static TileType arena[] =
    { $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $
    , $, _, _, _, _, _, _, _, _, _, h, _, _, _, _, _, $ 
    , $, _, $, T, $, _, $, $, $, $, $, _, $, T, $, _, $ 
    , $, _, $, _, _, _, h, _, _, _, h, _, _, _, $, _, $ 
    , $, _, $, _, $, $, $, $, $, $, $, _, $, _, $, _, $ 
    , $, _, $, _, $, _, _, _, _, _, $, _, $, _, $, _, $ 
    , $, _, $, _, $, _, $, $, $, _, $, _, $, _, $, _, $ 
    , $, _, $, _, $, _, $, _, $, _, $, _, $, _, $, _, $ 
    , _, _, $, _, $, _, $, _, B, _, $, _, $, _, $, _, $ 
    , $, _, $, _, $, _, $, _, $, _, $, _, $, _, $, _, $ 
    , $, _, $, _, $, _, $, _, $, $, $, _, $, _, $, _, $ 
    , $, _, $, _, $, _, $, _, _, _, _, _, $, _, $, _, $ 
    , $, _, $, _, $, _, $, $, $, $, $, $, $, _, $, _, $ 
    , $, _, $, _, _, _, h, _, _, _, h, _, _, _, $, _, $ 
    , $, v, $, T, $, _, $, $, $, $, $, _, $, T, $, v, $ 
    , $, _, _, _, _, _, h, _, _, _, _, _, _, _, _, _, $ 
    , $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $ 
    };

#undef $
#undef T
#undef S
#undef B
#undef _
#undef v
#undef h

static void find_enemies(State * const state)
{
    int e = 0;

#define MAP_AT(x, y)\
    (state->map_data[(x) + state->map_width * (y)])

    for (int i = 0; i < state->map_width; i++) {
        for (int j = 0; j < state->map_height; j++) {
            const int tile = MAP_AT(i, j);
            if (tile == -5) {
                Enemy enemy = {{i, j}, {i, j}, 0.0, 0, 0};
                state->map_enemies[e] = enemy;
                MAP_AT(i, j) = TILE_FLOOR;
                e++;
            } else if (tile == -6) {
                Enemy enemy = {{i, j}, {i, j}, 0.0, 1, 0};
                state->map_enemies[e] = enemy;
                MAP_AT(i, j) = TILE_FLOOR;
                e++;
            }

            if (e == MAX_ENEMY_COUNT) break;
        }
    }
#undef MAP_AT

    state->map_enemy_count = e;
}

#define RANDOM_BETWEEN 1

static TileType *pick_nonrandom(const int level)
{
    const int k = level / (RANDOM_BETWEEN + 1);
    switch (k) {
        case 0:  return mess; 
        case 1:  return hello;
        case 2:  return sliders; 
        case 3:  return mutual_core; 
        case 4:  return floor_is_lava; 
        default: return arena; 
    }
}

void change_level( State * const state
                 , Assets * const assets
                 , const int level
                 )
{
    if (state->map_data != NULL && state->map_is_dynamic) {
        free(state->map_data);
    }
        
    state->map_width  = 17;
    state->map_height = 17;
    const int map_size = state->map_width * state->map_height;

    if (level % (RANDOM_BETWEEN + 1) == 1) {
        state->map_data = pick_nonrandom(level);
        state->map_is_dynamic = 0;
    } else {
        state->map_is_dynamic = 1;
        state->map_data = malloc(sizeof(TileType) * map_size);
        if (state->map_data == NULL) return;

        fill_with_maze(state->map_data, state->map_width, state->map_height);
    }
    
    find_enemies(state);

    state->current_level_no = level;
}
