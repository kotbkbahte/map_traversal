#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lite_queue.h"
#include "map.h"
#include "alg.h"
#include "common.h"

extern int map_size;
extern int test_flag;

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

int _v3_arr[] = 
{
   1,  0,
   1,  1,
   0,  1,
  -1,  1,
   
  -1,  0,
  -1, -1,
   0, -1,
   1, -1,
};

extern trav_t trav;
extern int path_lenght;

void trav_init(trav_t* trav) 
{
  trav->g = malloc(sizeof(trav_tile_t) * (2 * path_lenght + 1) * (2 * path_lenght + 1) );
  
  lite_queue_init(&trav->q); 
  
  memset(trav->g, 0, sizeof(trav_tile_t) *  (2 * path_lenght + 1) * (2 * path_lenght + 1));
}

void trav_reinit(trav_t* trav, int l)
{
  free(trav->g);
  path_lenght = l;
  
  lite_queue_clear(&trav->q);
  trav->g = malloc(sizeof(trav_tile_t) * (2 * path_lenght + 1) * (2 * path_lenght + 1) );
  memset(trav->g, 0, sizeof(trav_tile_t) *  (2 * path_lenght + 1) * (2 * path_lenght + 1));
}

void trav_clear(trav_t* trav)
{
  lite_queue_clear(&trav->q);
  memset(trav->g, 0, sizeof(trav_tile_t) *  (2 * path_lenght + 1) * (2 * path_lenght + 1));
}

void trav_close(trav_t* trav)
{
  lite_queue_close(&trav->q);
  free(trav->g);
}

void bfs_V3(map_t* map, trav_t* trav, int start_x, int start_y, int path_lenght)
{
  lite_queue_clear(&trav->q);
  lite_enqueue(&trav->q, (vertex_t){0, 0});
  int vertex_count = 1, vertex_count_added = 0, vertex_count_processed = 0, range = 0;
  vertex_t vertex;
  
    
  while( (!lite_queue_is_empty(&trav->q)) && (range != path_lenght + 1) )
  {
    vertex = lite_dequeue(&trav->q);
    
    if( vertex_count == vertex_count_processed )
    {
      vertex_count           = vertex_count_added;
      vertex_count_added     = 0;
      vertex_count_processed = 0;
      range++;
    }
    
    vertex_count_processed++;
        
       
    if(trav->g[(path_lenght + vertex.y) * (2 * path_lenght + 1) + (path_lenght + vertex.x) ].processed) 
    {
      continue;
    }
    
    if(map->tiles[(start_y + vertex.y) * map_size + (start_x + vertex.x)].landscape == PLAIN) // if can visit this tile
    {      
      trav->g[(path_lenght + vertex.y) * (2 * path_lenght + 1) + (path_lenght + vertex.x) ].processed = true;
      trav->g[(path_lenght + vertex.y) * (2 * path_lenght + 1) + (path_lenght + vertex.x) ].range = range;
      
      for(int i = 0; i < 8; i++)
      {
        if(!is_on_trav(trav, path_lenght + vertex.x + _v3_arr[2 * i + 0], path_lenght + vertex.y + _v3_arr[2 * i + 1]))
          continue;
        if(!is_on_map(map, start_x + vertex.x + _v3_arr[2 * i + 0], start_y + vertex.y + _v3_arr[2 * i + 1]))
          continue;
        if(trav->g[(path_lenght + vertex.y + _v3_arr[2 * i + 1]) * (2 * path_lenght + 1) + 
          (path_lenght + vertex.x + _v3_arr[2 * i + 0]) ].processed)
          continue;
        lite_enqueue(&trav->q, (vertex_t){vertex.x + _v3_arr[2 * i + 0], vertex.y + _v3_arr[2 * i + 1]});  
        vertex_count_added += 1;
      }
    }
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

bool is_on_trav(trav_t* trav, int x, int y)
{
    return (0 <= x) && (x < 2 * path_lenght + 1) && (0 <= y) && (y < 2 * path_lenght + 1);
}

void trav_print(trav_t* trav)
{
    for (size_t i = 0; i < 2 * path_lenght + 1; i++) 
    {
        for (size_t j = 0; j < 2 * path_lenght + 1; j++) 
        {
            printf("%d ", trav->g[(2 * path_lenght - i) * (2 * path_lenght + 1) + j].range);
        }
        printf("\n");
    }
}
