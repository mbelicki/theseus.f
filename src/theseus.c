#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>

#include "types.h"
#include "create.h"
#include "draw.h"
#include "levels.h"
#include "logic.h"

int get_keyboard(void);
int is_not_done(void);

typedef struct _StateBehavior {
    State *(*process)(State *state, int new_keys, int old_keys, double time);
    State *(*update)(State *state, Assets *assets, double time);
    void (*draw)(const State *state, SDL_Surface *screen, const Assets *assets);
} StateBehavior;

static StateBehavior behaviors[STATE_MAX + 1];

static StateBehavior *get_behavior_table()
{
    StateBehavior intro = {process_splash, update_nop,  draw_intro};
    StateBehavior free  = {process_free,   update_free, draw_free};
    StateBehavior dead  = {process_splash, update_nop,  draw_dead};

    behaviors[STATE_INTRO] = intro; 
    behaviors[STATE_FREE]  = free; 
    behaviors[STATE_LOST]  = dead;

    return behaviors;
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    
    double time = 0;
    int keys    = 0;
    
    unsigned long then = 0;
    unsigned long now  = 0;
    
    State       *state  = create_initial_state();
    SDL_Surface *screen = get_screen(512, 512, "theseus.f");
    Assets      *assets = load_assets(screen);

    if (screen == NULL || state == NULL || assets == NULL) {
        exit(1);
    }

    change_level(state, assets, 0);
    StateBehavior *behaviors = get_behavior_table();

    while (is_not_done()) {
        int old_keys = keys;
        int new_keys = get_keyboard();
        
        then = now;
        now = SDL_GetTicks();
        time = (now - then) / 1000.0;

        StateBehavior behav = behaviors[state->type];
        
        if (is_marquee_on(state)) {
            handle_marquee(state, time);
        } else {
            state = behav.process(state, new_keys, old_keys, time);
            state = behav.update(state, assets, time);
        }
        behav.draw(state, screen, assets);

        keys = new_keys;
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
