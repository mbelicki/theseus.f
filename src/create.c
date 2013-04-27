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

Assets *load_assets()
{
    Assets *assets = malloc(sizeof(Assets));
    if (assets == NULL) return NULL;

    return assets;
}

static void fill_map_data( int * const map_data
                          , const int width
                          , const int height
                          )
{
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            map_data[i + width * j] = 0;
        }
    }
}

State *create_initial_state()
{
    State *state = malloc(sizeof(State));
    if (state == NULL) return NULL;

    state->map_width  = 32;
    state->map_height = 32;
    const int map_size = state->map_width * state->map_height;

    state->map_data = malloc(sizeof(int) * map_size);
    if (state->map_data == NULL){
        free(state);
        return NULL;
    }

    fill_map_data( state->map_data
                 , state->map_width
                 , state->map_height
                 );

    return state;
}
