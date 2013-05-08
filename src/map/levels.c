#include "map/levels.h"
#include "entity.h"

//    { $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
//    , $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $ 
//    };

#define SPAWNER_V_ENEMY -5
#define SPAWNER_H_ENEMY -6
#define SPAWNER_BOULDER -7

#define $ TILE_WALL
#define T TILE_TRAP
#define S TILE_TRADER
#define B TILE_BOSS
#define _ TILE_FLOOR
#define v SPAWNER_V_ENEMY
#define h SPAWNER_H_ENEMY
#define b SPAWNER_BOULDER

static TileType hello[] =
    { $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $
    , $, _, _, _, T, _, $, _, _, _, $, _, $, _, _, _, $ 
    , $, _, $, _, _, _, $, _, $, _, $, _, $, _, $, _, $ 
    , $, _, $, T, T, _, $, $, $, _, $, _, $, $, $, _, $ 
    , $, _, _, _, T, _, _, _, _, _, $, _, _, $, _, v, $ 
    , $, $, $, _, $, T, T, T, _, $, $, $, _, $, $, _, $ 
    , $, _, h, _, _, _, _, T, _, _, _, $, _, _, _, _, $ 
    , $, $, $, $, $, $, _, $, $, $, _, $, $, $, $, _, $ 
    , _, _, _, _, _, $, _, _, _, $, _, T, $, v, T, _, _ 
    , $, $, $, $, _, $, $, $, _, $, _, T, T, _, $, _, $ 
    , $, _, h, _, _, _, _, $, _, $, _, _, _, _, $, _, $ 
    , $, $, $, $, $, $, _, T, _, T, T, T, $, _, $, _, $ 
    , $, _, _, _, _, _, _, T, _, _, _, _, $, _, $, _, $ 
    , $, _, T, T, T, T, T, $, $, $, $, _, $, _, _, _, $ 
    , $, _, T, _, _, _, T, _, _, _, $, _, $, _, $, _, $ 
    , $, _, _, _, T, _, _, _, $, _, _, _, $, _, $, _, $ 
    , $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $ 
    };

static TileType floor_is_lava[] =
    { T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T
    , T, _, _, _, _, _, T, _, T, _, T, _, _, _, T, _, T 
    , T, _, T, T, T, _, T, v, T, _, T, _, T, _, T, _, T 
    , T, _, T, _, _, _, T, _, _, _, T, _, T, _, T, _, T 
    , T, _, T, _, T, T, T, _, T, _, _, _, T, _, T, _, T 
    , T, _, T, _, T, v, T, _, T, T, T, T, T, _, T, _, $ 
    , T, _, T, _, T, _, T, _, T, _, _, _, _, _, _, h, $ 
    , T, _, T, _, T, _, _, _, T, _, T, T, T, $, $, $, $ 
    , _, _, T, _, _, _, T, _, T, _, _, _, S, _, _, _, _ 
    , T, _, T, _, T, _, _, _, T, _, T, T, T, $, $, $, $ 
    , T, _, T, _, T, _, T, _, T, h, _, _, _, _, _, _, $ 
    , T, _, T, _, T, _, T, _, T, T, T, T, T, _, T, _, T 
    , T, _, T, _, T, T, T, _, T, _, _, _, T, _, T, _, T 
    , T, _, T, _, _, _, T, _, _, _, T, _, T, _, T, _, T 
    , T, _, T, T, T, _, T, _, T, _, T, _, T, _, T, _, T 
    , T, _, _, _, _, _, T, v, T, _, T, _, _, _, T, _, T 
    , T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T 
    };

static TileType mutual_core[] =
    { $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $
    , $, v, $, _, _, _, T, _, _, _, T, _, _, _, $, v, $ 
    , $, _, $, _, _, _, T, _, _, _, T, _, _, _, $, _, $ 
    , $, _, $, $, $, _, T, _, _, _, T, _, $, $, $, _, $ 
    , $, _, $, v, $, _, T, _, _, _, T, _, $, v, $, _, $ 
    , $, _, _, _, $, $, $, _, _, _, $, $, $, _, $, _, $ 
    , $, _, $, _, $, v, $, $, $, $, $, v, $, _, $, _, $ 
    , $, _, $, _, $, _, $, _, T, _, $, _, $, _, $, _, $ 
    , _, _, $, _, _, _, _, _, S, _, _, _, _, _, $, _, _ 
    , $, _, $, _, $, _, $, _, T, _, $, _, $, _, $, _, $ 
    , $, _, $, _, $, _, $, $, $, $, $, _, $, _, $, _, $ 
    , $, _, $, _, $, $, $, _, _, _, $, $, $, _, _, _, $ 
    , $, _, $, _, $, _, T, _, _, _, T, _, $, _, $, _, $ 
    , $, _, $, $, $, _, T, _, _, _, T, _, $, $, $, _, $ 
    , $, _, $, _, _, _, T, _, _, _, T, _, _, _, $, _, $ 
    , $, v, $, _, _, _, T, _, _, _, T, _, _, _, $, v, $ 
    , $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $ 
    };

static TileType mess[] =
    { $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $
    , $, _, _, _, $, _, _, _, $, _, _, $, _, _, _, _, $ 
    , $, _, $, _, $, $, $, _, $, _, $, $, _, $, $, $, $ 
    , $, _, $, _, $, _, $, _, $, _, $, _, _, _, _, _, $ 
    , $, _, $, _, _, _, $, _, $, _, $, _, $, $, $, $, $ 
    , $, _, $, $, $, $, $, _, $, _, $, _, _, _, _, _, $ 
    , $, _, _, _, _, _, _, _, $, v, $, _, $, $, _, $, $ 
    , $, _, $, _, $, b, b, $, $, _, $, $, $, _, _, _, $ 
    , _, v, T, _, _, h, _, _, T, _, _, _, T, _, T, _, _ 
    , $, _, $, _, $, _, $, $, $, _, $, _, $, _, $, $, $ 
    , $, _, b, _, b, _, _, _, _, _, $, _, $, _, _, _, $ 
    , $, _, $, _, $, _, $, _, $, $, $, _, $, $, _, $, $ 
    , $, _, $, _, $, _, $, _, _, _, $, _, _, _, _, _, $ 
    , $, _, $, _, $, v, $, $, $, _, $, $, _, $, $, _, $ 
    , $, _, _, _, _, _, $, _, _, _, $, _, _, _, $, _, $ 
    , $, _, $, _, $, _, $, _, $, _, $, _, $, _, $, _, $ 
    , $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $ 
    };

static TileType sliders[] =
    { $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $
    , $, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, $ 
    , $, _, $, $, $, $, $, $, $, $, $, $, $, _, $, $, $ 
    , $, _, $, h, _, _, _, _, _, _, _, _, _, _, _, _, $ 
    , $, _, $, $, $, _, $, T, $, T, $, T, $, $, $, $, $ 
    , $, _, $, _, h, _, _, _, _, _, _, _, _, _, _, _, $ 
    , $, _, $, $, $, $, $, T, $, T, $, T, $, _, $, $, $ 
    , $, _, $, _, _, h, _, _, _, _, _, _, _, _, $, _, $ 
    , _, _, $, $, $, $, $, _, $, $, $, $, $, $, $, _, _ 
    , $, T, $, _, _, _, $, _, $, _, _, _, $, S, _, _, $ 
    , $, _, $, _, $, _, $, _, _, _, $, $, $, _, $, $, $ 
    , $, _, $, _, $, _, _, _, $, $, $, _, _, _, _, _, $ 
    , $, _, $, _, $, _, $, _, _, _, _, v, $, $, $, $, $ 
    , $, _, $, _, $, _, $, T, $, T, $, _, $, _, _, _, $ 
    , $, _, _, _, $, _, $, _, $, _, $, _, _, _, $, _, $ 
    , $, _, $, _, $, _, _, _, _, _, $, _, $, _, $, _, $ 
    , $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $ 
    };

static TileType arena[] =
    { $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $
    , $, _, _, _, _, _, _, _, _, _, h, _, _, _, _, _, $ 
    , $, _, $, T, $, _, $, $, $, $, $, _, $, T, $, _, $ 
    , $, _, $, _, _, _, h, _, _, _, h, _, _, _, $, _, $ 
    , $, _, $, _, $, $, $, $, $, $, $, _, $, _, $, _, $ 
    , $, _, $, _, $, _, _, _, _, _, $, _, $, _, $, _, $ 
    , $, _, $, _, $, _, $, $, $, _, $, _, $, _, $, _, $ 
    , $, _, $, _, $, _, $, _, $, _, $, _, $, _, $, _, $ 
    , _, _, $, _, $, _, $, _, B, _, $, _, $, _, $, _, $ 
    , $, _, $, _, $, _, $, _, $, _, $, _, $, _, $, _, $ 
    , $, _, $, _, $, _, $, _, $, $, $, _, $, _, $, _, $ 
    , $, _, $, _, $, _, $, _, _, _, _, _, $, _, $, _, $ 
    , $, _, $, _, $, _, $, $, $, $, $, $, $, _, $, _, $ 
    , $, _, $, _, _, _, h, _, _, _, h, _, _, _, $, _, $ 
    , $, v, $, T, $, _, $, $, $, $, $, _, $, T, $, v, $ 
    , $, _, _, _, _, _, h, _, _, _, _, _, _, _, _, _, $ 
    , $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $, $ 
    };

#undef $
#undef T
#undef S
#undef B
#undef _
#undef v
#undef h
#undef b

/* TODO: na na na na refactor me */
extern void find_entities( Map * const map )
{
    const size_t chunk_size = 8;
    const size_t enemy_size = sizeof( Enemy );
    const size_t boulder_size = sizeof( Boulder );

    size_t enemy_buffer_capacity = chunk_size;
    size_t boulder_buffer_capacity = chunk_size;
    size_t enemy_count = 0;
    size_t boulder_count = 0;

    Enemy enemy;
    Boulder boulder;

    Enemy *enemy_buffer = malloc( enemy_size * enemy_buffer_capacity );
    Boulder *boulder_buffer = malloc( boulder_size * enemy_buffer_capacity );
    if ( enemy_buffer == NULL || boulder_buffer == NULL ) return;

#define MAP_AT(x, y)\
    (map->data[(x) + map->width * (y)])

    for ( int i = 0; i < map->width; i++ ) {
        for ( int j = 0; j < map->height; j++ ) {
            const int tile = MAP_AT(i, j);
            
            if ( tile == SPAWNER_V_ENEMY || tile == SPAWNER_H_ENEMY ) {
                MAP_AT(i, j) = TILE_FLOOR;
                
                init_enemy( &enemy, i, j );
                if (tile == SPAWNER_H_ENEMY)
                    enemy.flags |= ENEMY_MOVING_HORIZONTAL;
                /* growing enemy_buffer */
                if ( enemy_count >= enemy_buffer_capacity ) {
                    enemy_buffer_capacity += chunk_size;
                    enemy_buffer = realloc( enemy_buffer, 
                                    enemy_size * enemy_buffer_capacity );
                    if (enemy_buffer == NULL) return;
                }
                
                enemy_buffer[ enemy_count ] = enemy;
                enemy_count = enemy_count + 1;
            } else if ( tile == SPAWNER_BOULDER ) {
                MAP_AT(i, j) = TILE_FLOOR;
                
                init_boulder( &boulder, i, j );
                /* growing enemy_buffer */
                if ( boulder_count >= boulder_buffer_capacity ) {
                    boulder_buffer_capacity += chunk_size;
                    boulder_buffer = realloc( boulder_buffer, 
                                    boulder_size * boulder_buffer_capacity );
                    if (boulder_buffer == NULL) return;
                }
                
                boulder_buffer[ boulder_count ] = boulder;
                boulder_count = boulder_count + 1;
            }
        }
    }

#undef MAP_AT
    /* trim enemy_buffer and save it */
    enemy_buffer = realloc( enemy_buffer, enemy_size * enemy_count );
    boulder_buffer = realloc( boulder_buffer, boulder_size * boulder_count );
    //if ( enemy_buffer == NULL && boulder_buffer == NULL ) return;
    
    map->inital_enemy_states = enemy_buffer;
    map->inital_boudler_states = boulder_buffer;
    map->enemies_count = enemy_count;
    map->boulders_count = boulder_count;
}

extern TileType *pick_nonrandom( const int level )
{
    const int k = level / ( RANDOM_BETWEEN_COUNT + 1 );
    switch ( k ) {
        case 0:  return mess; 
        case 1:  return hello;
        case 2:  return sliders; 
        case 3:  return mutual_core; 
        case 4:  return floor_is_lava; 
        default: return arena; 
    }
}

