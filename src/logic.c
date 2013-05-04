#include <SDL/SDL.h>
#include "logic.h"
#include "levels.h"
#include "state.h"
#include "map.h"

static TileType destination_tile(const State * const state)
{
    const Point dst = ENTITY_IN( state->player ).destination;
    return state->map.data[ dst.x + state->map.width * dst.y ];
}

static TileType antydestination_tile(const State * const state)
{
    const Point dst = ENTITY_IN( state->player ).destination;
    const Point pos = ENTITY_IN( state->player ).position;

    const int dx = dst.x - pos.x;
    const int dy = dst.y - pos.y;

    const int x = pos.x - dx;
    const int y = pos.y - dy;

    return state->map.data[ x + state->map.width * y ];
}

static TileType current_tile(const State * const state)
{
    const Point pos = ENTITY_IN( state->player ).position;
    return state->map.data[ pos.x + state->map.width * pos.y ];
}

static inline void reset_player_position(State * const state) 
{
    init_entity( & ENTITY_IN( state->player ), 0, state->map.height / 2);
}

static void clean_map(Map * const map)
{
    substitute_tiles( map, TILE_STRING, TILE_FLOOR );
}

static void change_state(State * const state, const StateType next)
{
    state->next_type = next;
    state->is_marquee_closing = 1;
}

static void die(State * const state)
{
    change_state( state, STATE_LOST );
    reset_player_position( state );
    clean_map( & state->map );
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
    int up_pressed   = old_keys & KEY_UP && (new_keys & KEY_UP) == 0;
    int down_pressed = old_keys & KEY_DOWN && (new_keys & KEY_DOWN) == 0;


    if ( state->type == STATE_TRADE ) {
        if ( up_pressed ) {
            state->player.item = state->trader_item;
            state->trader_item = (state->trader_item + 1);// % (MAX_ITEM + 1);
            set_tile( & state->map, TILE_FLOOR, 
                      ENTITY_IN( state->player ).position );
            change_state( state, STATE_FREE );
        } else if ( down_pressed ) {
            state->trader_item = (state->trader_item + 1);// % (MAX_ITEM + 1);
            set_tile( & state->map, TILE_FLOOR, 
                      ENTITY_IN( state->player ).position );
            change_state( state, STATE_FREE );
        }
    } else if ( state->type == STATE_BOSS ) {
        if ( up_pressed ) {
            StateType next
                = state->player.item == ITEM_POTATO ? STATE_WON : STATE_OVER;
            change_state( state, next );
        }
    } else if (state->type == STATE_WON || state->type == STATE_OVER) {
        if (up_pressed) {
            state->requested_quit = 1;
        }
    } else {
        if (old_keys & KEY_UP && (new_keys & KEY_UP) == 0) {
            StateType next
                = state->type == STATE_SPLASH ? STATE_INTRO : STATE_FREE;
            change_state(state, next);
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
    Entity * const player_entity = & ENTITY_IN( state->player );
    int moved = 0;

    if ( is_moving( player_entity ) == 0 ) {
        if (new_keys & KEY_UP) {
            player_entity->destination.y -= 1;
            moved = 1;
        } else if (new_keys & KEY_DOWN) {
            player_entity->destination.y += 1;
            moved = 1;
        } else if (new_keys & KEY_LEFT) {
            player_entity->destination.x -= 1;
            moved = 1;
        } else if (new_keys & KEY_RIGHT) {
            player_entity->destination.x += 1;
            moved = 1;
        }
    }

    if ( moved ) {
        const int destination = destination_tile(state);
        const int current     = current_tile(state);
        const int antydest    = antydestination_tile(state);

        //const int neighbors_string = has_neighboring( & state->map
        //                                            , TILE_STRING
        //                                            , player_entity->position
        //                                            );

        if ( ( ! POINT_EQ( player_entity->destination
                         , player_entity->previous_position
                         ) )
             &&
               ( destination  == TILE_STRING 
                 && antydest == TILE_STRING ) )
        {
            cancel_move( player_entity );
        } else {
            if (current == TILE_FLOOR && destination != TILE_STRING) {
                set_tile( & state->map, TILE_STRING, 
                          player_entity->position);
            } else if (    current     == TILE_STRING 
                        && destination != TILE_FLOOR) {
                set_tile( & state->map, TILE_FLOOR, 
                          player_entity->position);
            }
        }
    }
    
    return state;
}


static void update_enemy( Enemy * const enemy
                        , const State * const state
                        , const double time
                        )
{
    const int is_horizontal = enemy->flags & ENEMY_MOVING_HORIZONTAL;
    const int is_going_back = enemy->flags & ENEMY_GOING_BACK;
    
    /* update the target testination */
    if ( is_moving( & ENTITY_OF( enemy ) ) == 0 ) {
        const int delta = is_going_back ? 1 : -1;
        if ( is_horizontal ) {
            ENTITY_OF( enemy ).destination.x += delta;
        } else {
            ENTITY_OF( enemy ).destination.y += delta;
        }
    }
    /* do the walking */
    const TileType non_walkable = TILE_WALL | TILE_STRING | TILE_TRAP;
    update_entity( & ENTITY_OF( enemy ), & state->map, non_walkable, time );
    /* handle wall hit */
    if ( ENTITY_OF( enemy ).flags & ENTITY_HAS_HIT_WALL ) {
        if ( is_going_back ) {
            enemy->flags &= ~ENEMY_GOING_BACK;
        } else {
            enemy->flags |= ENEMY_GOING_BACK;
        }
        ENTITY_OF( enemy ).flags &= ~ENTITY_HAS_HIT_WALL;
    }
}

extern State *update_free( State * const state
                         , Assets * const assets
                         , const double time
                         )
{
    Entity * const player_entity = & ENTITY_IN( state->player );

    const TileType non_walkable = TILE_WALL;
    TileType dst_tile = update_entity( player_entity, & state->map, 
                                       non_walkable, time );

    const int current = current_tile(state);
    if (current == TILE_TRAP) {
        die(state);
    } else if (current == TILE_TRADER) {
        change_state(state, STATE_TRADE);
    } else if (current == TILE_BOSS) {
        change_state(state, STATE_BOSS);
    }

    if (player_entity->position.x == state->map.width - 1) {
        change_level(state, assets, state->current_level_no + 1);
        player_entity->position.x = 0;
        player_entity->destination 
            = player_entity->previous_position = player_entity->position;
    }

    for (int i = 0; i < state->map_enemy_count; i++) {
        Enemy *enemy = state->map_enemies + i;
        update_enemy(enemy, state, time);
        if ( POINT_EQ( ENTITY_IN( state->player ).position
                     , ENTITY_OF(enemy ).position
                     ) ) {
            die(state);
        }
    }
        
    return state;
}

