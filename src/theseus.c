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

TileType destination_tile(const State * const state)
{
    return state->map_data[state->player_goto.x 
                + state->map_width * state->player_goto.y];
}

TileType current_tile(const State * const state)
{
    return state->map_data[state->player_pos.x 
                + state->map_width * state->player_pos.y];
}

void set_tile( State * const state
             , const TileType t
             , const int x
             , const int y
             )
{
    state->map_data[x + state->map_width * y] = t;
}

State *process( State * const state
              , const int new_keys
              , const int old_keys
              , const double time
              )
{
    if (state->player_move_delta == 0.0) {
        if (new_keys & KEY_UP) {
            state->player_goto.y -= 1;
            state->player_move_delta = 1.0;
        } else if (new_keys & KEY_DOWN) {
            state->player_goto.y += 1;
            state->player_move_delta = 1.0;
        } else if (new_keys & KEY_LEFT) {
            state->player_goto.x -= 1;
            state->player_move_delta = 1.0;
        } else if (new_keys & KEY_RIGHT) {
            state->player_goto.x += 1;
            state->player_move_delta = 1.0;
        }

        if ( state->player_move_delta == 1.0 ) {
            int destination = destination_tile(state);
            int current    = current_tile(state);

            if (destination > 0){
                state->player_goto = state->player_pos;
                state->player_move_delta = 0.0;
            } else {
                if (current == TILE_FLOOR && destination != TILE_STRING) {
                    set_tile(state, TILE_STRING, 
                             state->player_pos.x, state->player_pos.y);
                } else if (current == TILE_STRING && destination != TILE_FLOOR) {
                    set_tile(state, TILE_FLOOR, 
                             state->player_pos.x, state->player_pos.y);
                }
            }
        }
    }
    
    return state;
}

State *update(State * const state, const double time)
{
    if (    state->player_pos.x == state->player_goto.x 
         && state->player_pos.y == state->player_goto.y) {
        state->player_move_delta = 0.0; 
    } else {
        state->player_move_delta -= state->player_move_speed * time;
        if (state->player_move_delta <= 0.0) {
            state->player_move_delta = 0.0;
            state->player_pos = state->player_goto;
        }
    }
        
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
