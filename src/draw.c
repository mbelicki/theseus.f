#include <SDL/SDL.h>

#include "draw.h"

#define INTIFY(c,s) \
    (SDL_MapRGB((s)->format, (c).r, (c).g, (c).b))

void draw( const State * const state
         , SDL_Surface * const screen
         , const Assets * const assets
         )
{
    Uint32 *pixels = screen->pixels;
    
    const Point tile_size = { screen->w / state->map_width
                            , screen->h / state->map_height
                            };

    const Color _black = {255,   0,   0,   0};
    const Color _white = {255, 255, 255, 255};

    const Uint32 black = INTIFY(_black, screen);
    const Uint32 white = INTIFY(_white, screen);

    for (int i = 0; i < screen->w; i++) {
        for (int j = 0; j < screen->h; j++) {
            int x = i % tile_size.x;
            int y = j % tile_size.y;
            
            int current_tile = state->map_data[x + state->map_width * y];
            if (current_tile == 1) {
                pixels[i + screen->w * j] = black;
            } else {
                pixels[i + screen->w * j] = white;
            }
        }
    }

    SDL_UpdateRect(screen, 0, 0, 0, 0);
}
