#include <SDL/SDL.h>
#include <string.h> /* memcpy */

#include "logic.h"
#include "state.h"
#include "map.h"


extern State *create_initial_state()
{
    State *state = malloc(sizeof(State));
    if (state == NULL) return NULL;

    state->type = STATE_SPLASH;
    state->next_type = state->type;

    state->requested_quit = 0;

    state->marquee_amount = 0.0;
    state->is_marquee_closing = 0;

    init_player( & state->player, 0, 8 );
    state->trader_item = ITEM_SWORD;

    change_level( & state->map, 0 );

    return state;
}

static Boulder * boulder_at(const State * const state, const Point p)
{
    for (int i = 0; i < state->boulders_count; i++) {
        Boulder * boulder = state->boulders + i;
        int eq = POINT_EQ( boulder->entity.position, p );
        if ( eq ) return boulder;
    }
    return NULL;
}

static TileType antydestination_tile(const State * const state)
{
    const Point dst = state->player.entity.destination;
    const Point pos = state->player.entity.position;

    const int dx = dst.x - pos.x;
    const int dy = dst.y - pos.y;

    const int x = pos.x - dx;
    const int y = pos.y - dy;

    return state->map.data[ x + state->map.width * y ];
}

static void reset_entities( State * const state )
{
    const size_t e_size = sizeof( Enemy ) * state->map.enemies_count;
    /* if additional space is needed then realloc */
    if (state->enemies_count < state->map.enemies_count) {
        free( state->enemies );
        state->enemies = malloc( e_size );
        if ( state->enemies == NULL ) return;
    }     
    memcpy( state->enemies, state->map.inital_enemy_states, e_size );
    state->enemies_count = state->map.enemies_count;

    const size_t b_size = sizeof( Boulder ) * state->map.boulders_count;
    /* if additional space is needed then realloc */
    if (state->boulders_count < state->map.boulders_count) {
        free( state->boulders );
        state->boulders = malloc( b_size );
        if ( state->boulders == NULL ) return;
    }
    memcpy( state->boulders, state->map.inital_boudler_states, b_size );
    state->boulders_count = state->map.boulders_count;
}

static inline void reset_player_position( State * const state ) 
{
    init_entity( & state->player.entity
               , 0, state->map.height / 2
               , PLAYER_SPEED
               );
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
    reset_entities( state );
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

extern State *update_nop( State * const state, double time )
{
    /* this procedure was intentionally left blank */
    return state;
}

extern State *process_splash( State *state
                            , int new_keys
                            , int old_keys
                            )
{
    int up_pressed   = old_keys & KEY_UP && (new_keys & KEY_UP) == 0;
    int down_pressed = old_keys & KEY_DOWN && (new_keys & KEY_DOWN) == 0;


    if ( state->type == STATE_TRADE ) {
        if ( up_pressed ) {
            state->player.item = state->trader_item;
            state->trader_item = (state->trader_item + 1);// % (MAX_ITEM + 1);
            set_tile( & state->map, TILE_FLOOR
                    , state->player.entity.position
                    );
            change_state( state, STATE_FREE );
        } else if ( down_pressed ) {
            state->trader_item = (state->trader_item + 1);// % (MAX_ITEM + 1);
            set_tile( & state->map, TILE_FLOOR
                    , state->player.entity.position
                    );
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
                          )
{
    Entity * const player_entity = & state->player.entity;
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
        const int destination = tile_at( & state->map
                                       , player_entity->destination );
        const int current     = tile_at( & state->map
                                       , player_entity->position );
        const int antydest    = antydestination_tile(state);

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

        Boulder * b = boulder_at( state, player_entity->destination );
        if ( b != NULL ) {
            int i = player_entity->destination.x - player_entity->position.x;
            int j = player_entity->destination.y - player_entity->position.y;
            Point boulder_dest;
            boulder_dest.x = player_entity->destination.x + i;
            boulder_dest.y = player_entity->destination.y + j;
            TileType boulder_dest_tile = tile_at( & state->map, boulder_dest);
            if ( boulder_dest_tile == TILE_FLOOR ) {
                b->entity.destination = boulder_dest;
            } else {
                cancel_move( player_entity );
            }
        }
    }
    
    return state;
}

static void swap_enemy_direction( Enemy * const enemy )
{
    if ( enemy->flags & ENEMY_GOING_BACK ) {
        enemy->flags &= ~ENEMY_GOING_BACK;
    } else {
        enemy->flags |= ENEMY_GOING_BACK;
    }
}

static void update_enemy( Enemy * const enemy
                        , const Map * const map
                        , const double time
                        )
{
    const int is_horizontal = enemy->flags & ENEMY_MOVING_HORIZONTAL;
    const int is_going_back = enemy->flags & ENEMY_GOING_BACK;
    
    /* update the target testination */
    if ( is_moving( & enemy->entity ) == 0 ) {
        const int delta = is_going_back ? 1 : -1;
        if ( is_horizontal ) {
            enemy->entity.destination.x += delta;
        } else {
            enemy->entity.destination.y += delta;
        }
    }
    /* do the walking */
    const TileType non_walkable = TILE_WALL | TILE_STRING | TILE_TRAP;
    update_entity( & enemy->entity, map, non_walkable, time );
    /* handle wall hit */
    if ( enemy->entity.flags & ENTITY_HAS_HIT_WALL ) {
        swap_enemy_direction( enemy );
        enemy->entity.flags &= ~ENTITY_HAS_HIT_WALL;
    }
}

extern State *update_free( State * const state
                         , const double time
                         )
{
    Entity * const player_entity = & state->player.entity;

    const TileType non_walkable = TILE_WALL;
    TileType dst_tile = update_entity( player_entity, & state->map, 
                                       non_walkable, time );

    const int current = tile_at( & state->map, player_entity->position );
    if (current == TILE_TRAP) {
        die(state);
    } else if (current == TILE_TRADER) {
        change_state(state, STATE_TRADE);
    } else if (current == TILE_BOSS) {
        change_state(state, STATE_BOSS);
    }

    if (player_entity->position.x == state->map.width - 1) {
        state->current_level_no += 1;

        clean_map_data( & state-> map );
        change_level( & state->map, state->current_level_no );
        reset_entities( state );
        
        player_entity->position.x = 0;
        player_entity->destination 
            = player_entity->previous_position = player_entity->position;
    }

    for (int i = 0; i < state->enemies_count; i++) {
        Enemy *enemy = state->enemies + i;
        update_enemy( enemy, & state->map, time );
        if ( POINT_EQ( state->player.entity.position
                     , enemy->entity.position
                     ) ) {
            die(state);
        }

        Boulder * b = boulder_at( state, enemy->entity.destination );
        if ( b != NULL ) {
            cancel_move( & enemy->entity );
            swap_enemy_direction( enemy );
        }
    }

    for (int i = 0; i < state->boulders_count; i++) {
        Boulder *boulder = state->boulders + i;
        update_entity( & boulder->entity, & state->map, 0, time );
    }
        
    return state;
}

