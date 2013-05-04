#include "entity.h"

extern Entity * init_entity( Entity * const e
                           , const int x
                           , const int y
                           ) {
    e->position.x = e->destination.x = x; 
    e->position.y = e->destination.y = y; 
    e->previous_position = e->position;
    e->speed = DEFAULT_SPEED;
    e->movement_delta = 0.0;
    e->flags = 0;
    return e;
}

extern Enemy * init_enemy( Enemy * const e
                         , const int x
                         , const int y
                         ) {
    init_entity( & e->entity, x, y );
    e->flags = 0;
    return e;
}
extern Player * init_player( Player * const p
                           , const int x
                           , const int y
                           ) {
    init_entity( & p->entity, x, y );
    p->item = ITEM_POTATO;
    return p;
}

extern int is_moving( const Entity * const entity )
{
    return entity->movement_delta > 0.0;
}

extern void cancel_move( Entity * const entity )
{
      entity->destination = entity->position;
      entity->movement_delta = 0.0;
}

extern void complete_move( Entity * const entity )
{
      entity->previous_position = entity->position;
      entity->position = entity->destination;
      entity->movement_delta = 0.0;
}

extern TileType update_entity( Entity * const entity
                             , const Map * const map
                             , const TileType non_walkable
                             , const double time
                             )
{
    const TileType dest_tile = tile_at( map, entity->destination );
    
    int is_destination_reached = POINT_EQ( entity->destination
                                         , entity->position
                                         );
    /* start movement if not started already but position and destination
     * differ */
    if ( entity->movement_delta <= 0.0 
         && is_destination_reached == 0) 
    {
        entity->movement_delta = 1.0;
    }

    if ( is_moving( entity ) ) {
        if ( dest_tile & non_walkable ) {
            cancel_move( entity );
            entity->flags |= ENTITY_HAS_HIT_WALL;
        } else {
            entity->movement_delta -= entity->speed * time;
            if ( entity->movement_delta <= 0.0 ) {
                complete_move( entity );
            }
        }
    }

    return dest_tile;
}
