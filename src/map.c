#include "map.h"

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
