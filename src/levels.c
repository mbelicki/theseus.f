#include <SDL/SDL.h>

#include "levels.h"
#include "generator.h"


static TileType level_0[] =
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 
    , 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 
    , 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1 
    , 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 
    , 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1 
    , 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1 
    , 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1 
    , 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1 
    , 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0,-1, 0, 1, 1 
    , 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1 
    , 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1 
    , 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1 
    , 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 
    , 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 
    , 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1 
    , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 
    };

static TileType level_1[] =
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 
    , 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 
    , 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1 
    , 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1 
    , 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1 
    , 1, 0, 1, 1,-1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1 
    , 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1 
    , 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1 
    , 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0,-1, 0, 1, 1 
    , 1, 0, 1, 1,-1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1 
    , 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1 
    , 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1 
    , 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 
    , 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 
    , 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1 
    , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 
    };

static void fill_map_data( TileType * const map_data
                         , const int width
                         , const int height
                         )
{
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            map_data[i + width * j]
                 = i == 0 || j == 0 || i == width - 1 || j == height - 1;
        }
    }

    map_data[5 + width *  7] = TILE_WALL;
    map_data[6 + width *  7] = TILE_WALL;
    map_data[7 + width *  7] = TILE_WALL;
    map_data[7 + width *  8] = TILE_WALL;
    map_data[7 + width *  9] = TILE_TRAP;
    map_data[7 + width * 10] = TILE_WALL;
    map_data[7 + width * 12] = TILE_WALL;
}

void change_level( State * const state
                 , Assets * const assets
                 , const int level
                 )
{
    if (state->map_data != NULL && state->map_is_dynamic) {
        free(state->map_data);
    }
        
    state->map_width  = 16;
    state->map_height = 16;
    const int map_size = state->map_width * state->map_height;

    //state->map_data = malloc(sizeof(TileType) * map_size);
    //if (state->map_data == NULL) return;
    
    if (level == 0) {
        state->map_data = level_0;
        state->map_is_dynamic = 0;
    } else {
        //state->map_data = level_1;
        //state->map_is_dynamic = 0;
        state->map_data = malloc(sizeof(TileType) * map_size);
        if (state->map_data == NULL) return;

        fill_with_maze(state->map_data, state->map_width, state->map_height);
    }

    state->current_level_no = level;
}
