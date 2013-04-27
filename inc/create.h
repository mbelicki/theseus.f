#pragma once

#include "types.h"

extern void RepaintProceduralTextures( Assets * const assets
                                     , const SDL_Surface * const screen
                                     , const Color wall_color
                                     , const Color floor_color
                                     , const Color trap_color
                                     , const Color string_color
                                     );

State *create_initial_state();
SDL_Surface *get_screen(const int width, const int height, const char *name);
Assets *load_assets(const SDL_Surface * const screen);
