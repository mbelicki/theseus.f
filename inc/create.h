#pragma once

#include "types.h"

State *create_initial_state();
SDL_Surface *get_screen(const int width, const int height, const char *name);
Assets *load_assets(const SDL_Surface * const screen);
