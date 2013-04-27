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
        if ((new_keys & KEY_UP) != 0) {
            state->type = STATE_FREE;
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
        if (destination > 0) {
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

    for (int i = 0; i < state->map_enemy_count; i++) {
        update_enemy(&state->map_enemies[i], state, time);
    }
        
    return state;
}
