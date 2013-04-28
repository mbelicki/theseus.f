#include <SDL/SDL.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "draw.h"

static void draw_enemy( Enemy * const enemy
                      , SDL_Surface * const screen
                      , const Assets * const assets
                      , const Point tile_size
                      )
{
    Uint32 *pixels = screen->pixels;
    
    const int off_x = tile_size.x / 2;
    const int off_y = tile_size.y / 2;

    const Point position    = enemy->position;
    const Point destination = enemy->destination;
    const double delta = (1 - enemy->movement_delta);
    const int enemy_i
        = (int)(tile_size.x * (position.x 
                                + delta * (destination.x - position.x)));
    const int enemy_j
        = (int)(tile_size.y * (position.y 
                                + delta * (destination.y - position.y)));
  
#ifdef PROCEDURAL_ENEMY    
    for (int i = 0; i < assets->tex_width; i++) {
        for (int j = 0; j < assets->tex_height; j++) {
            const int x = enemy_i + i - off_x;
            const int y = enemy_j + j - off_y;
            pixels[x + screen->w * y]
                = assets->enemy_tex[i + assets->tex_width * j];
        }
    }
#else
    SDL_Rect src = {192, 216, 32, 32};
    SDL_Rect dst = {enemy_i - off_x, enemy_j - off_y, 32, 32};
    
    SDL_UnlockSurface(screen);
    SDL_BlitSurface(assets->image_font, &src, screen, &dst);
    SDL_LockSurface(screen);
#endif

}

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
    
#ifdef PROCEDURAL_PLAYER    
    for (int i = 0; i < assets->tex_width; i++) {
        for (int j = 0; j < assets->tex_height; j++) {
            const size_t x = player_i + i - off_x;
            const size_t y = player_j + j - off_y;
            pixels[x + screen->w * y]
                = assets->player_tex[i + assets->tex_width * j];
        }
    }
#else
    SDL_Rect src = {288, 216, 32, 32};
    SDL_Rect dst = {player_i - off_x, player_j - off_y, 32, 32};
    
    SDL_UnlockSurface(screen);
    SDL_BlitSurface(assets->image_font, &src, screen, &dst);
    SDL_LockSurface(screen);
#endif
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

    /* an ugly hack: */
    Point trader_loc = {0, 0};
    Point boss_loc   = {0, 0};

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
            }else if (current_tile == TILE_TRADER) {
                trader_loc.x = x;
                trader_loc.y = y;
            }else if (current_tile == TILE_BOSS) {
                boss_loc.x = x;
                boss_loc.y = y;
            } else {
                texture = assets->floor_tex;
            }

            if (texture == NULL) continue;
            pixels[i + screen->w * j] = texture[u + assets->tex_width * v];
        }
    }

    if (trader_loc.x != 0 || trader_loc.y != 0) {
        const int i = tile_size.x * trader_loc.x - off_x;
        const int j = tile_size.y * trader_loc.y - off_y;

        SDL_Rect src = {256, 216, 32, 32};
        SDL_Rect dst = {i, j, 32, 32};
        
        SDL_UnlockSurface(screen);
        SDL_BlitSurface(assets->image_font, &src, screen, &dst);
        SDL_LockSurface(screen);
    }

    if (boss_loc.x != 0 || boss_loc.y != 0) {
        const int i = tile_size.x * boss_loc.x - off_x;
        const int j = tile_size.y * boss_loc.y - off_y;

        SDL_Rect src = {224, 216, 32, 32};
        SDL_Rect dst = {i, j, 32, 32};
        
        SDL_UnlockSurface(screen);
        SDL_BlitSurface(assets->image_font, &src, screen, &dst);
        SDL_LockSurface(screen);
    }
}

static void fill_screen(SDL_Surface * const screen, const Color fill_color)
{
    Uint32 color = INTIFY(fill_color, screen);
    SDL_FillRect(screen, NULL, color);
}

#define GLYPH_WIDTH  32
#define GLYPH_HEIGHT 36

static Point measure_text(const char * const text)
{
    Point dimmensions = {strlen(text) * GLYPH_WIDTH, GLYPH_HEIGHT};
    return dimmensions;
}

static void draw_key( SDL_Surface * const screen
                    , SDL_Surface * const font
                    , const Point destination
                    , const Key key
                    )
{
    int u = 0;
    int v = 216;
    
    switch (key){
        case KEY_DOWN:  u += 32; break;
        case KEY_LEFT:  u += 64; break;
        case KEY_RIGHT: u += 96; break;
        default: case KEY_UP:    break;
    }

    SDL_Rect src  = {u, v, GLYPH_WIDTH, GLYPH_HEIGHT};
    SDL_Rect dest = {destination.x, destination.y, GLYPH_WIDTH, GLYPH_HEIGHT};
    
    SDL_UnlockSurface(screen);
    SDL_BlitSurface(font, &src, screen, &dest);
    SDL_LockSurface(screen);
}

static void draw_text( SDL_Surface * const screen
                     , SDL_Surface * const font
                     , const Point destination
                     , const char * const text
                     )
{
    SDL_UnlockSurface(screen);
    
    int x = destination.x;
    int y = destination.y;

    for (const char *p = text; *p != 0; p++) {
        const char c = *p - 1;
        if (c == ' ' - 1) {
            x +=GLYPH_WIDTH;
            continue;
        }
        if (c < 32 || c > 126) continue;
        
        int u = ((((int)c) - 32) % 16) * GLYPH_WIDTH;
        int v = ((((int)c) - 32) / 16) * GLYPH_HEIGHT;

        SDL_Rect char_rect = {u, v, GLYPH_WIDTH, GLYPH_HEIGHT};
        SDL_Rect dest_rect = {x, y, GLYPH_WIDTH, GLYPH_HEIGHT};
        SDL_BlitSurface(font, &char_rect, screen, &dest_rect);
        x += GLYPH_WIDTH;
    }

    SDL_LockSurface(screen);
}

static void draw_image( SDL_Surface * const screen
                      , SDL_Surface * const image
                      )
{
    SDL_UnlockSurface(screen);
    SDL_BlitSurface(image, NULL, screen, NULL);
    SDL_LockSurface(screen);
}

static void draw_marquee( SDL_Surface * const screen
                        , const Color fill_color
                        , const double amount
                        )
{
    const Uint32 color = INTIFY(fill_color, screen);
    const double k = amount > 1.0 ? 1.0 : (amount < 0.0 ? 0.0 : amount);
    const int width = (int)((screen->w / 2) * k);
    
    /* left side */
    SDL_Rect lrect = {0, 0, width, screen->h}; /* x, y, w, h */
    SDL_FillRect(screen, &lrect, color);
    /* right side */
    SDL_Rect rrect = {screen->w - width, 0, width, screen->h}; /* x, y, w, h */
    SDL_FillRect(screen, &rrect, color);
}

static void handle_marquee( const State * const state
                          , SDL_Surface * const screen
                          , const Assets * const assets 
                          )
{
    const double amount = state->marquee_amount;
    if (amount <= 0.0) return;
    
    const double k = (1.0 - cos(amount * 3.1415)) / 2;

    const Color color = assets->level_color;
    draw_marquee(screen, color, k);
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

    for (int i = 0; i < state->map_enemy_count; i++) {
        draw_enemy(&state->map_enemies[i], screen, assets, tile_size);
    }

    handle_marquee(state, screen, assets);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

extern void draw_dead( const State * const state   
                     , SDL_Surface * const screen
                     , const Assets * const assets
                     )
{
    fill_screen(screen, assets->splash_color);

    const char * text = "YOU DIED";
    const Point size = measure_text(text);

    Point dest = {(screen->w - size.x) / 2, (screen->h - size.y) / 2};
    draw_text(screen, assets->image_font, dest, text);

    Point key = {(screen->w - 32) / 2, (screen->h - 36) / 2 + 64};
    draw_key(screen, assets->image_font, key, KEY_UP);

    handle_marquee(state, screen, assets);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

extern void draw_splash( const State * const state   
                       , SDL_Surface * const screen
                       , const Assets * const assets
                       )
{
    fill_screen(screen, assets->splash_color);

    const char * title = "THESEUS.F";
    const Point size = measure_text(title);

    Point dest = {(screen->w - size.x) / 2, (screen->h - size.y) / 2};
    draw_text(screen, assets->image_font, dest, title);

    Point key = {(screen->w - 32) / 2, (screen->h - 36) / 2 + 64};
    draw_key(screen, assets->image_font, key, KEY_UP);

    handle_marquee(state, screen, assets);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

extern void draw_intro( const State * const state   
                      , SDL_Surface * const screen
                      , const Assets * const assets
                      )
{
    draw_image(screen, assets->image_dangerous);

    handle_marquee(state, screen, assets);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

extern void draw_trade( const State * const state   
                      , SDL_Surface * const screen
                      , const Assets * const assets
                      )
{
    draw_image(screen, assets->image_trader);

    const char * text = "A WILD TAKING RAT APPEARS";
    Point size = measure_text(text);

    const Point dest = {16, (screen->h - size.y) / 2};
    draw_text(screen, assets->image_font, dest, text);

    char says[64]; 
    sprintf( says, "\"GIVE ME YOUR %s AND I'LL GIVE YOU THIS %s\""
           , "POATATO", "SWORD");
    size = measure_text(says);

    const Point says_dest = {128, (screen->h - size.y) / 2};
    draw_text(screen, assets->image_font, says_dest, text);

    handle_marquee(state, screen, assets);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}
