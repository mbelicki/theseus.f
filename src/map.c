#include "map.h"
#include "entity.h"

#include "map/generator.h"
#include "map/levels.h"

extern void set_tile( Map * const map
                    , const TileType t
                    , const Point p
                    )
{
    if ( 0 > p.x || 0 > p.y )
        return;
    else if ( map->width <= p.x || map->height <= p.y )
        return;

    map->data[ p.x + map->width * p.y ] = t;
}

extern TileType tile_at( const Map * const map
                       , const Point p
                       )
{
    if ( 0 > p.x || 0 > p.y )
        return 0;
    else if ( map->width <= p.x || map->height <= p.y )
        return 0;

    return map->data[ p.x + map->width * p.y ];
}

extern void substitute_tiles( Map * const map
                            , const TileType old_tile
                            , const TileType new_tile
                            ) {
#define AT(x, y) \
    (map->data[(x) + map->width * (y)])

    for (int i = 0; i < map->width; i++) {
        for (int j = 0; j < map->height; j++) {
            if (AT(i, j) == old_tile)
                AT(i, j) = new_tile;
        }
    }
#undef AT
}

extern void clean_map_data( Map * const map )
{
    if ( map->flags & MAP_DYNAMIC ) {
        free( map->data );
    }

    free( map->inital_enemy_states );
    free( map->inital_boudler_states );

    map->data = NULL;
    map->inital_enemy_states = NULL;
    map->inital_boudler_states = NULL;

    map->width = map->height = 0;
    map->flags = 0;
    map->enemies_count = 0;
    map->boulders_count = 0;
}

extern void change_level( Map * const map, const int level )
{
    map->width  = 17;
    map->height = 17;
    const size_t map_size = map->width * map->height;

    if (level % ( RANDOM_BETWEEN_COUNT + 1 ) == 1 ) {
        map->flags = 0;
        map->data = pick_nonrandom(level);
    } else {
        map->flags = MAP_DYNAMIC;
        map->data = malloc(sizeof(TileType) * map_size);
        if ( map->data == NULL ) return;
        fill_with_maze( map->data, map->width, map->height );
    }
    find_entities( map );
}
