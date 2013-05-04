#pragma once

#include <stdlib.h> /* size_t */

/* forward type declarations */
struct _Point;
struct _Enemy;


typedef enum _TileType { TILE_FLOOR  =  0
                       , TILE_WALL   =  1
                       , TILE_TRAP   =  2
                       , TILE_STRING =  4
                       , TILE_TRADER =  8
                       , TILE_BOSS   = 16
                       } TileType;

typedef enum _MapFlags { MAP_DYNAMIC = 1
                       } MapFlags;


typedef struct _Map {
    TileType *data;
    size_t    width;
    size_t    height;
    
    MapFlags  flags;

    struct _Enemy *inital_enemy_states;
    size_t enemies_count;
} Map;

extern void set_tile( Map * const map
                    , const TileType t
                    , const struct _Point p
                    );

extern TileType tile_at( const Map * const map
                       , const struct _Point p
                       );

extern void substitute_tiles( Map * const map
                            , const TileType old_tile
                            , const TileType new_tile
                            );

extern void clear_map( Map * const map );
extern void change_level( Map * const map, const int level );
