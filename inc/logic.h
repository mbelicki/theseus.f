#pragma once

#include "state.h"

int is_marquee_on(const State * const state);
void handle_marquee(State * const state, const double time);

State *update_nop(State *state, Assets *assets, double time);

State *process_splash(State *state, int new_keys, int old_keys, double time);

State *process_free(State *state, int new_keys, int old_keys, double time);
State *update_free(State *state, Assets *assets, double time);
