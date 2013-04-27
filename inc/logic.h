#pragma once

#include "types.h"

State *update_nop(State *state, Assets *assets, double time);

State *process_splash(State *state, int new_keys, int old_keys, double time);

State *process_free(State *state, int new_keys, int old_keys, double time);
State *update_free(State *state, Assets *assets, double time);
