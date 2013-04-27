#include <SDL/SDL.h>

#include "create.h"

SDL_Surface *get_screen( const int width
                       , const int height
                       , const char * const title
                       )
{
    int result = 0;
    
    result = SDL_Init(SDL_INIT_EVERYTHING);
    if (result < 0) return NULL;

    atexit(SDL_Quit);

    SDL_Surface *screen
        = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_HWPALETTE);

    if (screen == NULL) return NULL;

    SDL_WM_SetCaption(title, NULL);

    return screen;
}

static void fill_wall_tex( const SDL_Surface * const context
                         , Uint32 * const texture
                         , const int width
                         , const int height
                         , const Color base_color
                         )
{
    Uint32 color = INTIFY(base_color, context);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            texture[i + width * j] = color;
        }
    }
}

static void fill_player_tex( const SDL_Surface * const context
                           , Uint32 * const texture
                           , const int width
                           , const int height
                           , const Color base_color
                           , const Color alt_color
                           )
{
    Uint32 player_color = INTIFY(base_color, context);
    Uint32 back_color   = INTIFY(alt_color, context);

    const int a =  width / 2;
    const int b = height / 2;
    const int r = (width + height) / 4;
    const int r_sq = r * r;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int x = i - a;
            int y = j - b;
            Uint32 color = x * x + y * y < r_sq ? player_color : back_color;
            texture[i + width * j] = color;
        }
    }
}

Assets *load_assets(const SDL_Surface * const screen)
{
    if (screen == NULL) return NULL;

    Assets *assets = malloc(sizeof(Assets));
    if (assets == NULL) return NULL;

    assets->tex_width  = 32;
    assets->tex_height = 32;

    const size_t tex_size  = assets->tex_width * assets->tex_height;
    const size_t tex_count = 5;

    Uint32 *textures = malloc(sizeof(Uint32) * tex_count * tex_size);
    if (textures == NULL) {
        free(assets);
        return NULL;
    }

    assets->wall_tex   = textures;
    assets->floor_tex  = textures + tex_size;
    assets->trap_tex   = textures + 2 * tex_size;
    assets->string_tex = textures + 3 * tex_size;
    assets->player_tex = textures + 4 * tex_size;

    const Color wall_color   = {255,   0,   0,   0};
    const Color floor_color  = {255, 255, 255, 255};
    const Color trap_color   = {255, 255,   0,   0};
    const Color string_color = {255, 255, 255, 128};

    fill_wall_tex( screen
                 , assets->wall_tex
                 , assets->tex_width
                 , assets->tex_height
                 , wall_color
                 );

    fill_wall_tex( screen
                 , assets->floor_tex
                 , assets->tex_width
                 , assets->tex_height
                 , floor_color
                 );

    fill_wall_tex( screen
                 , assets->trap_tex
                 , assets->tex_width
                 , assets->tex_height
                 , trap_color
                 );

    fill_wall_tex( screen
                 , assets->string_tex
                 , assets->tex_width
                 , assets->tex_height
                 , string_color
                 );

    fill_player_tex( screen
                   , assets->player_tex
                   , assets->tex_width
                   , assets->tex_height
                   , wall_color
                   , floor_color
                   );

    return assets;
}

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

State *create_initial_state()
{
    State *state = malloc(sizeof(State));
    if (state == NULL) return NULL;

    state->player_pos.x = 0;
    state->player_pos.y = 8;
    state->player_goto = state->player_prev_pos = state->player_pos;

    state->player_move_delta = 0.0;
    state->player_move_speed = 5.0;

    state->map_width  = 16;
    state->map_height = 16;
    const int map_size = state->map_width * state->map_height;

    state->map_data = malloc(sizeof(TileType) * map_size);
    if (state->map_data == NULL){
        free(state);
        return NULL;
    }

    fill_map_data( state->map_data
                 , state->map_width
                 , state->map_height
                 );
    state->map_data = level_0;

    return state;
}
