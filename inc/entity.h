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

#define ENTITY_IN(a) ((a).entity)
#define ENTITY_OF(a) ((a)->entity)

#define IS_ENTITY Entity entity

typedef enum _EnemyFlags { ENEMY_MOVING_HORIZONTAL = 1
                         , ENEMY_GOING_BACK        = 2
                         } EnemyFlags;

typedef struct _Enemy {
    IS_ENTITY;
    EnemyFlags flags;
} Enemy;

typedef enum _Item { ITEM_POTATO
                   , ITEM_SWORD
                   , ITEM_POISON
                   , ITEM_CHAINSAW
                   , MAX_ITEM = ITEM_CHAINSAW
                   } Item;

typedef struct _Player {
    IS_ENTITY;
    Item item;
} Player;

/* functions: */

extern Enemy * init_enemy( Enemy * const e
                         , const int x
                         , const int y
                         );

extern Entity * init_entity( Entity * const e
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

