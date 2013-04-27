#include <SDL/SDL.h>

#include "draw.h"

static void draw_player( const State * const state
                       , SDL_Surface * const screen
                       , const Assets * const assets
                       , const Point tile_size
                       )
{
    Uint32 *pixels = screen->pixels;
    
    const int off_x = tile_size.x / 2;
    const int off_y = tile_size.y / 2;


    const Point player_pos  = state->player_pos;
    const Point player_goto = state->player_goto;
    const double delta = (1 - state->player_move_delta);
    const int player_i
        = (int)(tile_size.x * (player_pos.x 
                                + delta * (player_goto.x - player_pos.x)));
    const int player_j
        = (int)(tile_size.y * (player_pos.y 
                                + delta * (player_goto.y - player_pos.y)));
    
    for (int i = 0; i < assets->tex_width; i++) {
        for (int j = 0; j < assets->tex_height; j++) {
            const int x = player_i + i - off_x;
            const int y = player_j + j - off_y;
            pixels[x + screen->w * y]
                = assets->player_tex[i + assets->tex_width * j];
        }
    }

}

static void draw_map( const State * const state
                    , SDL_Surface * const screen
                    , const Assets * const assets
                    , const Point tile_size
                    )
{
    Uint32 *pixels = screen->pixels;
    
    const int off_x = tile_size.x / 2;
    const int off_y = tile_size.y / 2;

    for (int i = 0; i < screen->w; i++) {
        for (int j = 0; j < screen->h; j++) {
            int x = (i + off_x) / tile_size.x;
            int y = (j + off_y) / tile_size.y;

            int u = (i + off_x) % assets->tex_width;
            int v = (j + off_y) % assets->tex_height;
            
            int current_tile = state->map_data[x + state->map_width * y];
            Uint32 *texture = NULL;
            if (current_tile == TILE_WALL) {
                texture = assets->wall_tex;
            } else if (current_tile == TILE_TRAP) {
                texture = assets->trap_tex;
            } else if (current_tile == TILE_STRING) {
                texture = assets->string_tex;
            } else {
                texture = assets->floor_tex;
            }

            if (texture == NULL) continue;
            pixels[i + screen->w * j] = texture[u + assets->tex_width * v];
        }
    }
}

static void draw_image( SDL_Surface * const screen
                      , SDL_Surface * const image
                      )
{
    SDL_BlitSurface(image, NULL, screen, NULL);
}

extern void draw_free( const State * const state   
                     , SDL_Surface * const screen
                     , const Assets * const assets
                     )
{
    const Point tile_size = { screen->w / (state->map_width  - 1)
                            , screen->h / (state->map_height - 1)
                            };
    
    draw_map(state, screen, assets, tile_size);
    draw_player(state, screen, assets, tile_size);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

extern void draw_dang( const State * const state   
                     , SDL_Surface * const screen
                     , const Assets * const assets
                     )
{
    draw_image(screen, assets->image_dangerous);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}
