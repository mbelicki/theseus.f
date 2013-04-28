#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "create.h"
#include "levels.h"

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
    Uint32 base = INTIFY(base_color, context);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            texture[i + width * j] = base;
        }
    }
}

static void fill_trap_tex( const SDL_Surface * const context
                         , Uint32 * const texture
                         , const int width
                         , const int height
                         , const Color base_color
                         )
{
    const Color lighter_color
        = {255, base_color.r + 30, base_color.g + 30, base_color.b + 30};

    const Uint32 base    = INTIFY(base_color,    context);
    const Uint32 lighter = INTIFY(lighter_color, context);

    const int tooth_width  = width  / 4;
    const int tooth_height = height / 2;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            const int is_odd = (i / tooth_width) % 2;

            const int ti = i % tooth_width;
            const int tj = j % tooth_height;
            
            Uint32 color = lighter;
            if (is_odd && tj > ti)
                color = base;
            if (!is_odd && (tj > (tooth_width - ti)))
                color = base;

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

extern void RepaintProceduralTextures( Assets * const assets
                                     , const SDL_Surface * const screen
                                     , const Color wall_color
                                     , const Color floor_color
                                     , const Color trap_color
                                     , const Color string_color
                                     )
{
    assets->level_color = wall_color;

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

    fill_trap_tex( screen
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

    fill_player_tex( screen
                   , assets->enemy_tex
                   , assets->tex_width
                   , assets->tex_height
                   , trap_color
                   , floor_color
                   );
}


SDL_Surface *load_image(const char * const name)
{
    SDL_Surface *image = NULL;
    if((image = IMG_Load(name)) == NULL) {
        printf("-> failed to load %s\n", name);
        return NULL;
    }
    return image;
}

Assets *load_assets(const SDL_Surface * const screen)
{
    if (screen == NULL) return NULL;

    Assets *assets = malloc(sizeof(Assets));
    if (assets == NULL) return NULL;

    assets->tex_width  = 32;
    assets->tex_height = 32;

    const size_t tex_size  = assets->tex_width * assets->tex_height;
    const size_t tex_count = 6;

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
    assets->enemy_tex  = textures + 5 * tex_size;

    assets->image_dangerous = load_image("gfx/dangerous.png");
    assets->image_dead      = load_image("gfx/dead.png");

    const Color wall_color   = {255,  13,  51, 114};
    const Color floor_color  = {255,  65, 113, 191};
    const Color trap_color   = {255, 216,  67,  63};
    const Color string_color = {255,  65, 187, 191};
    
    RepaintProceduralTextures( assets, screen
                             , wall_color, floor_color
                             , trap_color, string_color
                             );

    return assets;
}

State *create_initial_state()
{
    State *state = malloc(sizeof(State));
    if (state == NULL) return NULL;

    state->type = STATE_INTRO;
    state->next_type = state->type;

    state->marquee_amount = 0.0;
    state->is_marquee_closing = 0;

    state->player_pos.x = 1;
    state->player_pos.y = 8;
    state->player_goto = state->player_prev_pos = state->player_pos;

    state->player_move_delta = 0.0;
    state->player_move_speed = 7.0; /* in tiles per second */
    
    state->map_data = NULL;
    state->map_enemies = malloc(sizeof(Enemy) * MAX_ENEMY_COUNT);
    state->map_enemy_count = 0;

    return state;
}
