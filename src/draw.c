#include <SDL/SDL.h>

#include "draw.h"

void draw( const State * const state
         , SDL_Surface * const screen
         , const Assets * const assets
         )
{
    Uint32 *pixels = screen->pixels;
    
    const Point tile_size = { screen->w / state->map_width
                            , screen->h / state->map_height
                            };

    for (int i = 0; i < screen->w; i++) {
        for (int j = 0; j < screen->h; j++) {
            int x = i / tile_size.x;
            int y = j / tile_size.y;

            int u = i % assets->tex_width;
            int v = j % assets->tex_height;
            
            int current_tile = state->map_data[x + state->map_width * y];
            Uint32 *texture = NULL;
            if (current_tile == 1) {
                texture = assets->wall_tex;
            } else {
                texture = assets->floor_tex;
            }

            if (texture == NULL) 
                continue;
            pixels[i + screen->w * j] = texture[u + assets->tex_width * v];
        }
    }

    SDL_UpdateRect(screen, 0, 0, 0, 0);
}
