#include <SDL/SDL.h>
#include "logic.h"
#include "levels.h"

static TileType tile_at( const size_t i
                       , const size_t j
                       , const State * const state
                       )
{
    return state->map_data[i + state->map_width * j];
}

static TileType destination_tile(const State * const state)
{
    return state->map_data[state->player_goto.x 
                + state->map_width * state->player_goto.y];
}

static TileType antydestination_tile(const State * const state)
{
    const int dx = state->player_goto.x - state->player_pos.x;
    const int dy = state->player_goto.y - state->player_pos.y;

    const int x = state->player_pos.x - dx;
    const int y = state->player_pos.y - dy;

    return state->map_data[x + state->map_width * y];
}

static TileType current_tile(const State * const state)
{
    return state->map_data[state->player_pos.x 
                + state->map_width * state->player_pos.y];
}

static void set_tile( State * const state
                    , const TileType t
                    , const int x
                    , const int y
                    )
{
    state->map_data[x + state->map_width * y] = t;
}

static void reset_player_position(State * const state) 
{
    const Point start_pos = {0, state->map_height / 2};

    state->player_move_delta = 0.0;
    state->player_pos      = start_pos;
    state->player_goto     = start_pos;
    state->player_prev_pos = start_pos;
}

static void clean_map(State * const state)
{
#define AT(x, y) \
    (state->map_data[(x) + state->map_width * (y)])

    for (int i = 0; i < state->map_width; i++) {
        for (int j = 0; j < state->map_height; j++) {
            if (AT(i, j) == TILE_STRING)
                AT(i, j) = TILE_FLOOR;
        }
    }
#undef AT
}

static void change_state(State * const state, const StateType next)
{
    state->next_type = next;
    state->is_marquee_closing = 1;
}

static void die(State * const state)
{
    change_state(state, STATE_LOST);
    reset_player_position(state);
    clean_map(state);
}

extern int is_marquee_on(const State * const state)
{
    return state->is_marquee_closing || state->marquee_amount > 0.0;
}

extern void handle_marquee(State * const state, const double time)
{
    if (state->is_marquee_closing) {
        state->marquee_amount += MARQUEE_SPEED * time;
        if (state->marquee_amount >= 1.0) {
            state->marquee_amount = 1.0;
            state->is_marquee_closing = 0;
            state->type = state->next_type;
        }
    } else if (state->marquee_amount > 0.0) {
        state->marquee_amount -= MARQUEE_SPEED * time;
        if (state->marquee_amount <= 0.0) {
            state->marquee_amount = 0.0;
        }
    }
}

extern State *update_nop(State *state, Assets *assets, double time)
{
    /* this procedure was intentionally left blank */
    return state;
}

extern State *process_splash( State *state
                            , int new_keys
                            , int old_keys
                            , double time
                            )
{
    if (old_keys & KEY_UP) {
        if ((new_keys & KEY_UP) == 0) {
            change_state(state, STATE_FREE);
        }
    }
    return state;
}

extern State *process_free( State * const state
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
            const int destination = destination_tile(state);
            const int current     = current_tile(state);
            const int antydest    = antydestination_tile(state);

            if ((state->player_goto.x != state->player_prev_pos.x 
                  || state->player_goto.y != state->player_prev_pos.y)
                &&
                  (destination > 0 /* wall ahead */
                   || (destination  == TILE_STRING 
                        && antydest == TILE_STRING))
               ){

                state->player_goto = state->player_pos;
                state->player_move_delta = 0.0;
            } else {
                if (current == TILE_FLOOR && destination != TILE_STRING) {
                    set_tile(state, TILE_STRING, 
                             state->player_pos.x, state->player_pos.y);
                } else if (    current     == TILE_STRING 
                            && destination != TILE_FLOOR) {
                    set_tile(state, TILE_FLOOR, 
                             state->player_pos.x, state->player_pos.y);
                }
            }
        }
    }
    
    return state;
}

static void update_enemy( Enemy * const enemy
                        , const State * const state
                        , double time
                        )
{
    if (enemy->movement_delta == 0.0) {
        if (enemy->is_moving_horizontal) {
            if (enemy->is_going_back) {
                enemy->destination.x += 1;
            } else {
                enemy->destination.x -= 1;
            }
        } else {
            if (enemy->is_going_back) {
                enemy->destination.y += 1;
            } else {
                enemy->destination.y -= 1;
            }
        }
        const int destination = tile_at(enemy->destination.x, 
                                        enemy->destination.y, state);
        if (destination != 0) {
            enemy->movement_delta = 0.0;
            enemy->destination = enemy->position;
            enemy->is_going_back = !enemy->is_going_back;
        } else {
            enemy->movement_delta = 1.0;
        }
    } else {
        enemy->movement_delta -= ENEMY_SPEED * time;
        if (enemy->movement_delta <= 0.0) {
            enemy->movement_delta = 0.0;
            enemy->position = enemy->destination;
        }
    }
}

extern State *update_free( State * const state
                         , Assets * const assets
                         , const double time
                         )
{
    if (    state->player_pos.x == state->player_goto.x 
         && state->player_pos.y == state->player_goto.y) {
        state->player_move_delta = 0.0; 
    } else {
        state->player_move_delta -= state->player_move_speed * time;
        if (state->player_move_delta <= 0.0) {
            state->player_move_delta = 0.0;
            state->player_prev_pos = state->player_pos;
            state->player_pos = state->player_goto;
        }
    }

    if (state->player_pos.x == state->map_width - 1) {
        change_level(state, assets, 1);
        state->player_pos.x = 0;
        state->player_goto = state->player_prev_pos = state->player_pos;
    }

    const int current = current_tile(state);
    if (current == TILE_TRAP) {
        die(state);
    }

    for (int i = 0; i < state->map_enemy_count; i++) {
        Enemy *enemy = state->map_enemies + i;
        update_enemy(enemy, state, time);
        if (POINT_EQ(state->player_pos, enemy->position)) {
            die(state);
        }
    }
        
    return state;
}

