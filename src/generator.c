#include <stdlib.h>
#include <SDL/SDL.h>

#include "generator.h"

typedef enum _dir {
    DIR_UP    = 1,
    DIR_DOWN  = 2,
    DIR_LEFT  = 4,
    DIR_RIGHT = 8
} Dir;

static Point dir2point(const Dir dir)
{
    Point point = {0, 0};
    switch (dir) {
        case DIR_UP:    
            point.x = 0;
            point.y = 1;
            return point;
        case DIR_DOWN:
            point.x = 0;
            point.y = -1;
            return point;
        case DIR_LEFT:
            point.x = -1;
            point.y = 0;
            return point;
        case DIR_RIGHT:
            point.x = 1;
            point.y = 0;
            return point;
        default:
            return point;
    }
}

static Dir opposite(const Dir dir)
{
    switch (dir) {
        case DIR_UP:    return DIR_DOWN;
        case DIR_DOWN:  return DIR_UP;
        case DIR_LEFT:  return DIR_RIGHT;
        case DIR_RIGHT: return DIR_LEFT;
        default: return DIR_UP;
    }
}

static void shuffle(Dir * const array, const size_t n)
{
    if (n == 0) return; 

    for (int i = 0; i < n - 1; i++) 
    {
      const int j = i + rand() / (RAND_MAX / (n - i) + 1);
      const int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
}

typedef struct _PointStack {
    Point *data;
    size_t capacity;
    size_t top_index;
} PointStack;

static PointStack *build_stack(const size_t capacity)
{
    Point *points = malloc(sizeof(Point) * capacity);
    PointStack *stack = malloc(sizeof(PointStack));

    if (points == NULL || stack == NULL) return NULL;

    stack->data = points;
    stack->capacity = capacity;
    stack->top_index = 0;

    return stack;
}

static void destory_stack(PointStack * const stack)
{
    free(stack->data);
    free(stack);
}

static void push(PointStack * const stack, const Point point)
{
    size_t top = stack->top_index;

    if (top == stack->capacity - 1) return;

    stack->data[top] = point;
    stack->top_index = top + 1;
}

static Point pop(PointStack * const stack)
{
    size_t top = stack->top_index;
    Point point = {0, 0};

    if (top == 0) return point;

    stack->top_index = top - 1;
    return stack->data[stack->top_index];
}

static Point peek(PointStack * const stack)
{
    size_t top = stack->top_index;
    Point point = {0, 0};

    if (top == 0) return point;

    top = top - 1;
    return stack->data[top];
}

static size_t count(PointStack * const stack)
{
    return stack->top_index;
}

static void render_maze( const Dir * const grid
                       , const size_t grid_width
                       , const size_t grid_height
                       , TileType * const map_data
                       , const size_t map_width
                       , const size_t map_height
                       );

static void fill_grid( Dir * const grid
                     , const size_t gird_width
                     , const size_t grid_height 
                     , const TileType * const map_data
                     , const size_t width
                     , const size_t height
                     );

void fill_with_maze( TileType * const map_data
                   , const size_t width
                   , const size_t height
                   )
{
    const Point grid_size = {width / 2, height / 2};

    Dir * grid = malloc(sizeof(Dir) * grid_size.x * grid_size.y);
    if (grid == NULL) return;

    for (int i = 0; i < grid_size.x * grid_size.y; i++) {
        grid[i] = 0;
    }
    
    fill_grid  (grid, grid_size.x, grid_size.y, map_data, width, height);
    
    render_maze(grid, grid_size.x, grid_size.y, map_data, width, height);
}

static void fill_grid( Dir * const grid
                     , const size_t grid_width
                     , const size_t grid_height 
                     , const TileType * const map_data
                     , const size_t width
                     , const size_t height
                     )
{
    const Point grid_size = {grid_width, grid_height};

    Dir directions[] = {DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT};
    PointStack *active_cells = build_stack(grid_size.x * grid_size.y);

    Point current_cell;
    current_cell.x = rand() % grid_size.x;
    current_cell.y = rand() % grid_size.y;

    push(active_cells, current_cell);

#define GRID_AT(i, j)\
    (grid[(i) + grid_size.x * (j)])

    while (count(active_cells) > 0) {
        int inactivate = 1;
        current_cell = peek(active_cells);
        int cx = current_cell.x;
        int cy = current_cell.y;
        shuffle(directions, 4);

        for (int i = 0; i < 4; i++) {
            Dir dir = directions[i];
            Point dir_vec = dir2point(dir);
            int nx = current_cell.x + dir_vec.x;
            int ny = current_cell.y + dir_vec.y;
            if (   nx >= 0 && nx < grid_size.x
                && ny >= 0 && ny < grid_size.y
                && GRID_AT(nx, ny) == 0
               ) {

                GRID_AT(cx, cy) |= dir;
                GRID_AT(nx, ny) |= opposite(dir);

                Point new_active = {nx, ny};
                push(active_cells, new_active);
                inactivate = 0;
                break;
            }
        }

        if (inactivate) pop(active_cells);
    }

#undef GRID_AT

    destory_stack(active_cells);
}

static void render_maze( const Dir * const grid
                       , const size_t grid_width
                       , const size_t grid_height
                       , TileType * const map_data
                       , const size_t map_width
                       , const size_t map_height
                       )
{
    for (int i = 0; i < map_width * map_height; i++) {
        map_data[i] = TILE_FLOOR;
    }

#define MAP_AT(x, y) \
    (map_data[(x) + map_width * (y)])    
    /* insides */   
    for (int i = 0; i < grid_width; i++) {
        for (int j = 0; j < grid_height; j++) {
            Dir dir = grid[i + grid_width * j];

            MAP_AT(i * 2, j * 2) = TILE_WALL;
            if ((dir & DIR_DOWN) == 0)
                MAP_AT(i * 2 + 1, j * 2)
                    = rand() % 8 == 0 ? TILE_TRAP : TILE_WALL;
            
            if ((dir & DIR_LEFT) == 0)
                MAP_AT(i * 2, j * 2 + 1)
                    = rand() % 8 == 0 ? TILE_TRAP : TILE_WALL;
            
            if (   dir == DIR_LEFT || dir == DIR_RIGHT
                || dir == DIR_DOWN || dir == DIR_UP) {
                MAP_AT(i * 2 + 1, j * 2 + 1) = TILE_FLOOR;
            }
        }
    }
    /* top & bottom border */
    for (int i = 0; i < map_width; i++) {
        MAP_AT(i, map_height - 1) = MAP_AT(i, 0) = TILE_WALL;
    }
    /* left $ right border */
    for (int j = 0; j < map_height; j++) {
        MAP_AT(map_width -1, j) = MAP_AT(0, j) = TILE_WALL;
    }

    const int entry_y = map_width / 2;
    const int exit_y  = map_width / 2;

    /* add entry and exit: */
    for (int i = 0; i < map_width - 1; i++) {
        MAP_AT(i, entry_y) = TILE_FLOOR;
        
        if (MAP_AT(i + 1, entry_y) == TILE_FLOOR)
            break;
        if (MAP_AT(i, entry_y + 1) == TILE_FLOOR)
            break;
        if (MAP_AT(i, entry_y - 1) == TILE_FLOOR)
            break;
    }
    for (int i = map_width - 1; i > 0; i--) {
        MAP_AT(i, exit_y) = TILE_FLOOR;
        
        if (MAP_AT(i + 1, exit_y) == TILE_FLOOR)
            break;
        if (MAP_AT(i, exit_y + 1) == TILE_FLOOR)
            break;
        if (MAP_AT(i, exit_y - 1) == TILE_FLOOR)
            break;
    }
#undef MAP_AT
}

