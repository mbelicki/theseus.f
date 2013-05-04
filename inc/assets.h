#pragma once

#include <SDL/SDL.h>
#include "types.h"
//#include "state.h"

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

//State *create_initial_state();

SDL_Surface *get_screen( const int width
                       , const int height
                       , const char *name
                       );

Assets *load_assets( const SDL_Surface * const screen );
