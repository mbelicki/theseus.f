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

    render_maze(grid, grid_size.x, grid_size.y, map_data, width, height);
}

//  /** @return {Array.<Cell>} */
//  function mazeChunkBuilder() {
//    var gridWidth  = (((C.CHUNK_WIDTH) / 2) | 0);
//    var gridHeight = (((C.CHUNK_HEIGHT) / 2) | 0);
//  
//    var grid = new Array(gridWidth);
//    for (var i = 0; i < gridWidth; i++) {
//      grid[i] = new Array(gridHeight);
//      for (var j = 0; j < gridWidth; j++) {
//        grid[i][j] = 0;
//      }
//    }
//  
//    var directions = [DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT];
//    var activeCells = new Array();
//    var _x = ((Math.random() * gridWidth)  | 0)
//    var _y = ((Math.random() * gridHeight) | 0)
//    activeCells.push({x: _x, y: _y});
//  
//    while (activeCells.length > 0) {
//      var inactivate = true;
//      var cell = activeCells[activeCells.length - 1];
//      _x = cell.x; _y = cell.y; 
//      directions = shuffle(directions);
//  
//      for (var key in directions) {
//        var dir = directions[key];
//        var nx = (_x + dir.vector.x) | 0;
//        var ny = (_y + dir.vector.y) | 0;
//  
//        if (   nx >= 0 && nx < gridWidth
//            && ny >= 0 && ny < gridHeight
//            && grid[nx][ny] == 0) {
//          grid[_x][_y] |= dir.flag;
//          grid[nx][ny] |= opposite(dir.flag);
//          activeCells.push({x: nx, y: ny});
//          inactivate = false;
//          break;
//        }
//      }
//  
//      if (inactivate) activeCells.pop();
//    }
//  
//    return renderMaze(grid, gridWidth, gridHeight);
//  }

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
    
    for (int i = 0; i < grid_width; i++) {
        for (int j = 0; j < grid_height; j++) {
            Dir dir = grid[i + grid_width * j];

            MAP_AT(i * 2, j * 2) = TILE_WALL;
            if ((dir & DIR_DOWN) == 0)
                MAP_AT(i * 2 + 1, j * 2) = TILE_WALL;
            
            if ((dir & DIR_LEFT) == 0)
                MAP_AT(i * 2, j * 2 + 1) = TILE_WALL;
            
            if (   dir == DIR_LEFT || dir == DIR_RIGHT
                || dir == DIR_DOWN || dir == DIR_UP) {
                MAP_AT(i * 2 + 1, j * 2 + 1) = TILE_FLOOR;
            }
        }
    }
#undef MAP_AT

}

//  /** @return {Array.<Cell>} */
//  function renderMaze(grid, gridWidth, gridHeight) {
//    
//    /* turn grid into tiles: */
//    for (var i = 0; i < gridWidth; i++) {
//      for (var j = 0; j < gridHeight; j++) {
//        var dir = grid[i][j];
//        data[i * 2][j * 2].height = 0;
//        if ((dir & DIR_DOWN.flag) == 0)
//          data[i * 2 + 1][j * 2].height = 0;
//        if ((dir & DIR_LEFT.flag) == 0)
//          data[i * 2][j * 2 + 1].height = 0;
//        if (   dir === DIR_LEFT.flag || dir === DIR_RIGHT.flag
//            || dir === DIR_DOWN.flag || dir === DIR_UP.flag) {
//          data[i * 2 + 1][j * 2 + 1].feature = 1;
//        }
//      }
//    }
//    /* add entry and exit: */
//    for (var i = 0; i < C.CHUNK_WIDTH - 1; i++) {
//      data[i][C.CHUNK_HALF_HEIGHT].height = 1;
//      if (data[i + 1][C.CHUNK_HALF_HEIGHT].height > 0)
//        break;
//      if (data[i][C.CHUNK_HALF_HEIGHT + 1].height > 0)
//        break;
//      if (data[i][C.CHUNK_HALF_HEIGHT - 1].height > 0)
//        break;
//    }
//    for (var i = C.CHUNK_WIDTH - 1; i > 0; i--) {
//      data[i][C.CHUNK_HALF_HEIGHT].height = 1;
//      if (data[i - 1][C.CHUNK_HALF_HEIGHT].height > 0)
//        break;
//      if (data[i][C.CHUNK_HALF_HEIGHT + 1].height > 0)
//        break;
//      if (data[i][C.CHUNK_HALF_HEIGHT - 1].height > 0)
//        break;
//    }
//  
//    return data;
//  }
