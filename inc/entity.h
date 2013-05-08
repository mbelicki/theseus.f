#pragma once

#include <stdlib.h>
#include "types.h"
#include "map.h"

/* type definitions & macros: */

typedef enum _EntityFlags { ENTITY_HAS_HIT_WALL = 1
                          } EntityFlags;

typedef struct _Entity {
    Point       position;
    Point       destination;
    Point       previous_position;
    double      movement_delta;
    double      speed;
    EntityFlags flags;
} Entity;

#define DEFAULT_SPEED 5.0

typedef enum _EnemyFlags { ENEMY_MOVING_HORIZONTAL = 1
                         , ENEMY_GOING_BACK        = 2
                         } EnemyFlags;

typedef struct _Enemy {
    Entity     entity;
    EnemyFlags flags;
} Enemy;

typedef enum _Item { ITEM_POTATO
                   , ITEM_SWORD
                   , ITEM_POISON
                   , ITEM_CHAINSAW
                   , MAX_ITEM = ITEM_CHAINSAW
                   } Item;

typedef struct _Player {
    Entity entity;
    Item   item;
} Player;

typedef struct _Boulder {
    Entity entity;
} Boulder;

#define PLAYER_SPEED 7.0

/* functions: */

extern Entity * init_entity( Entity * const e
                           , const int x
                           , const int y
                           , const double speed
                           );

extern Enemy * init_enemy( Enemy * const e
                         , const int x
                         , const int y
                         );

extern Player * init_player( Player * const p
                           , const int x
                           , const int y
                           );

extern Boulder * init_boulder( Boulder * const b
                             , const int x
                             , const int y
                             );

extern int is_moving( const Entity * const entity );
extern void cancel_move( Entity * const entity );
extern void complete_move( Entity * const entity );

extern TileType update_entity( Entity * const entity
                             , const Map * const map
                             , const TileType non_walkable
                             , const double time
                             );

