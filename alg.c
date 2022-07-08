#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "alg.h"
#include "lite_queue.h"
#include "common.h"

extern int map_size;


#define _tile(x, y) map->tiles[map_size * (y) + (x)]
#define _t(x, y) map->tiles[map_size * (y + start_y) + (x + start_x)]

// #define _trav(x, y) trav->g[path_lenght * (y + 2) + (x + 2)]
#define _trav(x, y) trav->g[(2 * path_lenght + 1) * (y + 2) + (x + 2)]


#define PROCESS_TILE(x, y) \
    if(_t(x, y).landscape == PLAIN) _t(x, y).processed = 2;

#define V2_PROCESS_TILE(x, y) \
    if(_t(x, y).landscape == PLAIN) _trav(x, y).range = 2;

#define _PROCESS_TILE_5(x, y) \
    PROCESS_TILE(_v2_arr5[x + 1][y + 1][0], _v2_arr5[x + 1][y + 1][1])  \
    PROCESS_TILE(_v2_arr5[x + 1][y + 1][4], _v2_arr5[x + 1][y + 1][5])  \
    PROCESS_TILE(_v2_arr5[x + 1][y + 1][2], _v2_arr5[x + 1][y + 1][3])  \
    PROCESS_TILE(_v2_arr5[x + 1][y + 1][6], _v2_arr5[x + 1][y + 1][7])  \
    PROCESS_TILE(_v2_arr5[x + 1][y + 1][8], _v2_arr5[x + 1][y + 1][9])

#define _PROCESS_TILE_3(x, y) \
    PROCESS_TILE(_v2_arr3[x + 1][y + 1][0], _v2_arr3[x + 1][y + 1][1])  \
    PROCESS_TILE(_v2_arr3[x + 1][y + 1][2], _v2_arr3[x + 1][y + 1][3])  \
    PROCESS_TILE(_v2_arr3[x + 1][y + 1][4], _v2_arr3[x + 1][y + 1][5])

#define _AFTER_PROCESS_TILE_3(x, y) \
    V2_PROCESS_TILE(_v2_arr3[x + 1][y + 1][0], _v2_arr3[x + 1][y + 1][1])   \
    V2_PROCESS_TILE(_v2_arr3[x + 1][y + 1][2], _v2_arr3[x + 1][y + 1][3])   \
    V2_PROCESS_TILE(_v2_arr3[x + 1][y + 1][4], _v2_arr3[x + 1][y + 1][5])

#define _AFTER_PROCESS_TILE_5(x, y) \
    V2_PROCESS_TILE(_v2_arr5[x + 1][y + 1][0], _v2_arr5[x + 1][y + 1][1])   \
    V2_PROCESS_TILE(_v2_arr5[x + 1][y + 1][4], _v2_arr5[x + 1][y + 1][5])   \
    V2_PROCESS_TILE(_v2_arr5[x + 1][y + 1][2], _v2_arr5[x + 1][y + 1][3])   \
    V2_PROCESS_TILE(_v2_arr5[x + 1][y + 1][6], _v2_arr5[x + 1][y + 1][7])   \
    V2_PROCESS_TILE(_v2_arr5[x + 1][y + 1][8], _v2_arr5[x + 1][y + 1][9])


#define PROCESS_TILE_5(x, y)                \
    if(_t(x, y).landscape == PLAIN)         \
    {                                       \
        _t(x, y).processed = true;          \
        _PROCESS_TILE_5(x, y)               \
    }

#define PROCESS_TILE_3(x, y)                \
    if(_t(x, y).landscape == PLAIN)         \
    {                                       \
        _t(x, y).processed = true;          \
        _PROCESS_TILE_3(x, y)               \
    }

#define V2_PROCESS_TILE_3(x, y)             \
    if(_t(x, y).landscape == PLAIN)         \
    {                                       \
        _trav(x, y).range = 1;              \
        _AFTER_PROCESS_TILE_3(x, y)         \
    }

#define V2_PROCESS_TILE_5(x, y)             \
    if(_t(x, y).landscape == PLAIN)         \
    {                                       \
        _trav(x, y).range = 1;              \
        _AFTER_PROCESS_TILE_5(x, y)         \
    }

int _v2_pos[16 * 2] =
{
     2, 0,
     2, 1,
     2, 2,
     1, 2,
     0, 2,
    -1, 2,
    -2, 2,
    -2, 1,
    -2, 0,
    -2,-1,
    -2,-2,
    -1,-2,
     0,-2,
     1,-2,
     2,-2,
     2,-1,
};

int _v2_arr5[3][3][5 * 2] =
{
    [2][2] = {
        2, 0,
        2, 1,
        2, 2,
        1, 2,
        0, 2,
    },
    [0][2] = {
        0, 2,
       -1, 2,
       -2, 2,
       -2, 1,
       -2, 0,
   },
    [0][0] = {
        -2, 0,
        -2,-1,
        -2,-2,
        -1,-2,
         0,-2,
    },
    [2][0] = {
        0,-2,
        1,-2,
        2,-2,
        2,-1,
        2, 0,
    },

};

int _v2_arr3[3][3][3 * 2] =
{
    [2][1] = {
         2,-1,
         2, 0,
         2, 1,
    },
    [1][2] = {
        1, 2,
        0, 2,
       -1, 2,
    },
    [0][1] = {
        -2, 1,
        -2, 0,
        -2,-1,
    },
    [1][0] = {
        -1,-2,
         0,-2,
         1,-2,
    }
};

/*
    +2 # # # # #
    +1 # 2 1 0 #
     0 # 3   7 #
    -1 # 4 5 6 #
    -2 # # # # #
      -2-1 0+1+2
*/ // 16

/*
  1 # # #
  0 # # #
 -1 # # #
   -1 0 1
*/

extern trav_t trav;
extern int path_lenght;

void trav_init(void) 
{
    trav.g = malloc(sizeof(trav_tile_t) * (2 * path_lenght + 1) * (2 * path_lenght + 1) );
    memset(trav.g, 0, sizeof(trav_tile_t) *  (2 * path_lenght + 1) * (2 * path_lenght + 1));
}

void trav_clear(void)
{
    memset(trav.g, 0, sizeof(trav_tile_t) *  (2 * path_lenght + 1) * (2 * path_lenght + 1));
}

void bfs_V3(map_t* map, trav_t* trav, int start_x, int start_y, int path_lenght)
{
  lite_queue_t q;
  lite_queue_init(&q);
  lite_enqueue(&q, (vertex_t){start_x, start_y});
    vertex_t v;
  while( !lite_queue_is_empty(&q) )
  {
    v = lite_dequeue(&q);
    printf("%d %d", v.x, v.y);
  }
}

void bfs_V2(map_t* map, trav_t* trav, int start_x, int start_y, int path_lenght)
{
    if(path_lenght > 2)
    {
        printf("bfs_V2 does not support path_lenght > 2");
        return;
    }

    V2_PROCESS_TILE_5( 1,  1);
    V2_PROCESS_TILE_3( 0,  1);
    V2_PROCESS_TILE_5(-1,  1);

    V2_PROCESS_TILE_3( 1,  0);
    V2_PROCESS_TILE_3(-1,  0);

    V2_PROCESS_TILE_5( 1, -1);
    V2_PROCESS_TILE_3( 0, -1);
    V2_PROCESS_TILE_5(-1, -1);
}

void bfs_V1(map_t* map, int start_x, int start_y, int path_lenght)
{
    if(path_lenght > 2)
    {
        printf("bfs_V1 does not support path_lenght > 2");
        return;
    }
    PROCESS_TILE_5( 1,  1);
    PROCESS_TILE_3( 0,  1);
    PROCESS_TILE_5(-1,  1);

    PROCESS_TILE_3( 1,  0);
    PROCESS_TILE_3(-1,  0);

    PROCESS_TILE_5( 1, -1);
    PROCESS_TILE_3( 0, -1);
    PROCESS_TILE_5(-1, -1);
}


void trav_print(void)
{
    for (size_t i = 0; i < 2 * path_lenght + 1; i++) {
        for (size_t j = 0; j < 2 * path_lenght + 1; j++) {
            printf("%d ", trav.g[(2 * path_lenght - i) * (2 * path_lenght + 1) + j].range);
        }
        printf("\n");
    }
}
