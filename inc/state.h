#pragma once

#include "types.h"
#include "entity.h"

typedef enum _StateType { STATE_SPLASH
                        , STATE_INTRO
                        , STATE_LOST
                        , STATE_WON
                        , STATE_OVER
                        , STATE_BOSS
                        , STATE_TRADE
                        , STATE_FREE

                        , STATE_COUNT = STATE_FREE + 1
                        } StateType;

typedef struct _State {
    StateType type;
    StateType next_type;

    Map       map;

    Enemy    *enemies;
    size_t    enemies_count;

    Boulder  *boulders;
    size_t    boulders_count;

    int current_level_no;

    Player player;

    Item   trader_item;

    double marquee_amount;
    int    is_marquee_closing;

    int requested_quit;
} State;

#define MARQUEE_SPEED 3.2

typedef enum _Key {
    KEY_UP    = 1,
    KEY_DOWN  = 2,
    KEY_LEFT  = 4,
    KEY_RIGHT = 8
} Key;

