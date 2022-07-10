#include <stdlib.h>
#include <stdio.h>

#include "lite_queue.h"
#include "map.h"
#include "alg.h"

// static inline void map_print(void);

extern map_t map;
/*extern int map_size;*/
extern int mountains_density;
extern int path_lenght;
extern int fast_flag;


char landscape_str[] =
{
    [PLAIN] = ' ',
    [MOUNTAIN] = 'M',
};


void map_init(map_t* map, int size)
{
  map->size = size;
  map->tiles = malloc(sizeof(tile_t) * map->size * map->size);
  map->str   = malloc(sizeof(char) * (map->size * ( 2 * map->size) + 1));
  map->trav_land = malloc(sizeof(char) * (map->size * ( 2 * map->size) + 1));
  map->str_init = false;
  map->trav_land_init = false;
  
  map_gen(map);
}

bool is_on_map(map_t* map, int x, int y)
{
    return (0 <= x) && (x < map->size) && (0 <= y) && (y < map->size);
}

void map_gen(map_t* map)
{
    for (size_t i = 0; i < map->size; i++) {
        for (size_t j = 0; j < map->size; j++) {
            map->tiles[i * map->size + j].processed = false;
            if(rand() % mountains_density)
                map->tiles[i * map->size + j].landscape = PLAIN;
            else
                map->tiles[i * map->size + j].landscape = MOUNTAIN;
        }
    }

    map->tiles[map->y * map->size + map->x].landscape = PLAIN;
    map->tiles[map->y * map->size + map->x].processed = 9;
}

void map_print(map_t* map)
{
    if(!map->str_init)
    {
        for (size_t i = 0; i < map->size; i++) 
        {
            for (size_t j = 0; j < map->size; j++)
            {
                map->str[2 * (map->size - 1 - i) * map->size + 2 * j] = landscape_str[map->tiles[i * map->size + j].landscape];
                map->str[2 * (map->size - 1 - i) * map->size + 2 * j + 1] = ' ';
            }
            map->str[2 * (map->size - 1 - i) * map->size + 2 * (map->size - 1) + 1] = '\n';
        }


        map->str[2 * map->size * map->size] = '\0';
        map->str[2 * (map->size - 1 - map->y) * map->size + 2 * map->x] = 'v';

        map->str_init = true;
    }



    fwrite(map->str, sizeof(char), (map->size * ( 2 * map->size) + 1), stdout);
}

/*void map_print_processed(void)*/
/*{*/
/*    for (int i = map->size-1; i >= 0; i--) {*/
/*        for (size_t j = 0; j < map->size; j++) {*/
/*            printf("%d ", map.tiles[i * map->size + j].processed);*/
/*        }*/
/*        printf("\n");*/
/*    }*/
/*}*/

void map_print_trav(map_t* map, trav_t* trav)
{
    if(!map->trav_land_init)
    {
            // printf("%ld\n",map_size - 1 - i );
        for (size_t i = 0; i < map->size; i++) {
            for (size_t j = 0; j < map->size; j++)
            {
                // printf("%ld\n", 2 * (map_size - 1 - i) * map_size + 2 * j);
                map->trav_land[2 * (map->size - 1 - i) * map->size + 2 * j] = landscape_str[map->tiles[i * map->size + j].landscape];
                map->trav_land[2 * (map->size - 1 - i) * map->size + 2 * j + 1] = ' ';
            }
            map->trav_land[2 * (map->size - 1 - i) * map->size + 2 * (map->size - 1) + 1] = '\n';
        }
        map->trav_land[2 * map->size * map->size] = '\0';



        for (int i = 0; i < 2 * path_lenght + 1; i++) {
            for (int j = 0; j < 2 * path_lenght + 1; j++) 
            {
                if(!is_on_map(map, j + map->x - path_lenght, map->size - 1 - (i + map->y - path_lenght))) continue;
                if(trav->g[(i) * (path_lenght * 2 + 1) + (j)].range != 0)
                {
                    map->trav_land[2 * (map->size - 1 - (i + map->y - path_lenght)) * map->size + 2 * (j + map->x - path_lenght)] =
                        trav->g[(i) * (path_lenght * 2 + 1) + (j)].range + '0';

                }
            }
        }
        map->trav_land[2 * (map->size - 1 - map->y) * map->size + 2 * map->x] = 'v';
        map->trav_land_init = true;
    }
    fwrite(map->trav_land, sizeof(char), (map->size * ( 2 * map->size) + 1), stdout);
}

void map_close(map_t* map)
{
  free(map->tiles);
  free(map->str);
  free(map->trav_land);
}
