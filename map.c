#include <stdlib.h>
#include <stdio.h>

#include "map.h"
#include "alg.h"

// static inline void map_print(void);

extern map_t map;
extern int map_size;
extern int mountains_density;
extern int path_lenght;
extern int fast_flag;

char landscape_str[] =
{
    [PLAIN] = '_',
    [MOUNTAIN] = 'M',
};

char range_str[] =
{
    [0] = '0',
    [1] = '1',
    [2] = '2'
};

void map_init(void)
{
    map.tiles = malloc(sizeof(tile_t) * map_size * map_size);
    map.str   = malloc(sizeof(char) * (map_size * ( 2 * map_size) + 1));
    map.trav_land = malloc(sizeof(char) * (map_size * ( 2 * map_size) + 1));
    map.str_init = false;
    map.trav_land_init = false;

    map_gen();

    map_print();
}

void map_gen(void)
{

    for (size_t i = 0; i < map_size; i++) {
        for (size_t j = 0; j < map_size; j++) {
            map.tiles[i * map_size + j].processed = false;
            if(rand() % 4)
                map.tiles[i * map_size + j].landscape = PLAIN;
            else
                map.tiles[i * map_size + j].landscape = MOUNTAIN;
        }
    }

    map.tiles[map.y * map_size + map.x].landscape = PLAIN;
    map.tiles[map.y * map_size + map.x].processed = 9;
}

// static inline void map_print(void)
// {
//     for (size_t i = 0; i < map_size; i++) {
//         for (size_t j = 0; j < map_size; j++) {
//             printf("%c ", landscape_str[map.tiles[i * map_size + j].landscape] );
//         }
//         printf("\n");
//     }
// }

void map_print(void)
{
    if(!map.str_init)
    {
        for (size_t i = 0; i < map_size; i++) {
            // printf("%ld\n",map_size - 1 - i );
            for (size_t j = 0; j < map_size; j++)
            {
                map.str[2 * (map_size - 1 - i) * map_size + 2 * j] = landscape_str[map.tiles[i * map_size + j].landscape];
                map.str[2 * (map_size - 1 - i) * map_size + 2 * j + 1] = ' ';
            }
            map.str[2 * (map_size - 1 - i) * map_size + 2 * (map_size - 1) + 1] = '\n';
        }


        map.str[2 * map_size * map_size] = '\0';
        map.str[2 * (map_size - 1 - map.y) * map_size + 2 * map.x] = 'v';

        map.str_init = true;
    }



    fwrite(map.str, sizeof(char), (map_size * ( 2 * map_size) + 1), stdout);
}

void map_print_processed(void)
{
    for (int i = map_size-1; i >= 0; i--) {
        for (size_t j = 0; j < map_size; j++) {
            printf("%d ", map.tiles[i * map_size + j].processed);
        }
        printf("\n");
    }
}

void map_print_trav(map_t* map, trav_t* trav)
{
    if(!map->trav_land_init)
    {
            // printf("%ld\n",map_size - 1 - i );
        for (size_t i = 0; i < map_size; i++) {
            for (size_t j = 0; j < map_size; j++)
            {
                // printf("%ld\n", 2 * (map_size - 1 - i) * map_size + 2 * j);
                map->trav_land[2 * (map_size - 1 - i) * map_size + 2 * j] = landscape_str[map->tiles[i * map_size + j].landscape];
                map->trav_land[2 * (map_size - 1 - i) * map_size + 2 * j + 1] = ' ';
            }
            map->trav_land[2 * (map_size - 1 - i) * map_size + 2 * (map_size - 1) + 1] = '\n';
        }


        map->trav_land[2 * map_size * map_size] = '\0';
        map->trav_land_init = true;

        for (size_t i = 0; i < 2 * path_lenght + 1; i++) {
            for (size_t j = 0; j < 2 * path_lenght + 1; j++) {
                if(trav->g[(i) * (path_lenght * 2 + 1) + (j)].range != 0)
                {
                    map->trav_land[2 * (map_size - 1 - (i + map->y - path_lenght)) * map_size + 2 * (j + map->x - path_lenght)] =
                        range_str[ trav->g[(i) * (path_lenght * 2 + 1) + (j)].range ];

                }
            }
        }
        map->trav_land[2 * (map_size - 1 - map->y) * map_size + 2 * map->x] = 'v';
        map->trav_land_init = true;

    }
    fwrite(map->trav_land, sizeof(char), (map_size * ( 2 * map_size) + 1), stdout);
}
