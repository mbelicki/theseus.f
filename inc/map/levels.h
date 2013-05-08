#pragma once

#include "types.h"
#include "map.h"

#define RANDOM_BETWEEN_COUNT 1

extern void find_entities( Map * const map );

extern TileType *pick_nonrandom( const int level );
