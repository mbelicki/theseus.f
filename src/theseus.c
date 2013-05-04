#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>

#include "types.h"
#include "state.h"
#include "assets.h"
#include "draw.h"
#include "logic.h"

int get_keyboard(void);
int is_not_done(const State * const state);

typedef struct _StateBehavior {
    State *(*process)(State *state, int new_keys, int old_keys, double time);
    State *(*update)(State *state, double time);
    void (*draw)(const State *state, SDL_Surface *screen, const Assets *assets);
} StateBehavior;

static StateBehavior behaviors[ STATE_COUNT ];

static StateBehavior *get_behavior_table()
{
    StateBehavior splash = {process_splash, update_nop,  draw_splash};
    StateBehavior intro  = {process_splash, update_nop,  draw_intro};
    StateBehavior free   = {process_free,   update_free, draw_free};
    StateBehavior trade  = {process_splash, update_nop,  draw_trade};
    StateBehavior boss   = {process_splash, update_nop,  draw_boss};
    StateBehavior dead   = {process_splash, update_nop,  draw_dead};
    StateBehavior won    = {process_splash, update_nop,  draw_won};
    StateBehavior over   = {process_splash, update_nop,  draw_over};

    behaviors[STATE_SPLASH] = splash; 
    behaviors[STATE_INTRO]  = intro; 
    behaviors[STATE_FREE]   = free;
    behaviors[STATE_TRADE]  = trade; 
    behaviors[STATE_BOSS]   = boss;
    behaviors[STATE_LOST]   = dead;
    behaviors[STATE_OVER]   = over;
    behaviors[STATE_WON]    = won;

    return behaviors;
}

#ifdef __MINGW32__
int WinMain(int argc, char **argv)
#else
int main(int argc, char **argv)
#endif
{
    srand(time(NULL));
    
    double time = 0;
    int keys    = 0;
    
    unsigned long then = 0;
    unsigned long now  = 0;
    
    State       *state  = create_initial_state();
    SDL_Surface *screen = get_screen( 512, 512, "theseus.f" );
    Assets      *assets = load_assets( screen );

    if (screen == NULL || state == NULL || assets == NULL) {
        exit(1);
    }

    StateBehavior *behaviors = get_behavior_table();

    while ( is_not_done( state ) ) {
        int old_keys = keys;
        int new_keys = get_keyboard();
        
        then = now;
        now = SDL_GetTicks();
        time = (now - then) / 1000.0;

        StateBehavior behav = behaviors[state->type];
        
        if ( is_marquee_on( state ) ) {
            handle_marquee(state, time);
        } else {
            state = behav.process(state, new_keys, old_keys, time);
            state = behav.update(state, time);
        }
        behav.draw(state, screen, assets);

        keys = new_keys;
        
        int delay = 17 - (long)(now - then);
        SDL_Delay(delay > 0 ? delay : 0);
    }
    
    printf("errorous errand occured: %s\n", SDL_GetError());
    exit(0);
}

int is_not_done( const State * const state )
{
    SDL_Event event = {0};
    SDL_PollEvent( & event );
    return event.type != SDL_QUIT && state->requested_quit == 0;
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
