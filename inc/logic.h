#pragma once

#include "state.h"

extern State *create_initial_state();

extern int is_marquee_on( const State * const state );
extern void handle_marquee( State * const state, const double time );

extern State  *update_nop( State * const state, const double time );
extern State *update_free( State * const state, const double time);

extern State *process_splash( State * const state
                            , const int new_keys
                            , const int old_keys
                            );

extern State *process_free( State * const state
                          , const int new_keys
                          , const int old_keys
                          );

