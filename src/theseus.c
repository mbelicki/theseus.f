#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>

#include "types.h"
#include "create.h"
#include "draw.h"

int get_keyboard(void);
State *process(State *state, int new_keys, int old_keys, double time);
State *update(State *state, double time);

int is_not_done(void);

int main(int argc, char **argv)
{
    double time    = 0;
    int keys       = 0;
    
    unsigned long then = 0;
    unsigned long now  = 0;
    
    State       *state  = create_initial_state();
    SDL_Surface *screen = get_screen(512, 512, "theseus");
    Assets      *assets = load_assets(screen);
   
    if (screen == NULL || state == NULL || assets == NULL) {
        exit(1);
    }

    while (is_not_done()) {
        int old_keys = keys;
        int new_keys = get_keyboard();
        
        then = now;
        now = SDL_GetTicks();
        time = (now - then) / 1000.0;

        state = process(state, new_keys, old_keys, time);
        state = update(state, time);
        draw(state, screen, assets);
    }
    
    printf("errorous errand occured: %s\n", SDL_GetError());
    exit(0);
}

int is_not_done(void)
{
    SDL_Event event = {0};
    SDL_PollEvent(&event);
    return event.type != SDL_QUIT;
}

State *process( State * const state
              , const int new_keys
              , const int old_keys
              , const double time
              )
{
    if (new_keys & KEY_UP) {
        state->player_pos.y -= 1;
    } else if (new_keys & KEY_DOWN) {
        state->player_pos.y += 1;
    } else if (new_keys & KEY_LEFT) {
        state->player_pos.x -= 1;
    } else if (new_keys & KEY_RIGHT) {
        state->player_pos.x += 1;
    }

    return state;
}

State *update(State * const state, const double time)
{
    return state;
}

int get_keyboard()
{
    int keys = 0;
    Uint8 *rawkeys = SDL_GetKeyState(NULL);
    
    keys |= KEY_UP    * rawkeys[SDLK_UP];
    keys |= KEY_DOWN  * rawkeys[SDLK_DOWN];
    keys |= KEY_LEFT  * rawkeys[SDLK_LEFT];
    keys |= KEY_RIGHT * rawkeys[SDLK_RIGHT];

    return keys;
}
