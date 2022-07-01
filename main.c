#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include "map.h"
#include "alg.h"

int running = true;
int map_size;
int mountains_density;
int path_lenght;
int fast_flag = false;
bool default_settings = false;
map_t map;
trav_t trav;

void set_default(void)
{
    map_size = 12;
    mountains_density = 4;
    path_lenght = 2;
    map.x = 6;
    map.y = 4;
}

void set_custom(void)
{
    printf("Map size: ");
        scanf("%d", &map_size);
    printf("Mountains density: ");
        scanf("%d", &mountains_density);
    printf("Path lenght: ");
        scanf("%d", &path_lenght);
    printf("Start position: ");
        scanf("%d %d", &map.x, &map.y);
}

static inline int parse_args(int argc, char **argv)
{
    int c;
    while ((c = getopt(argc, argv, "fd")) != -1)
        switch (c)
            {
            case 'd':
                default_settings = true;
                break;
            case 'f':
                fast_flag = true;
                break;
            case '?':
                fprintf(stderr, "Usage:\n-d\t set's default settings. Overwise custom settings\n");
                return 1;
            default:
                break;
            }

    if(default_settings)
    {
        fprintf(stderr, "Set default.\n");
        set_default();
    }
    else
    {
        fprintf(stderr, "Set custom.\n");
        set_custom();
    }


    return 0;
}

void process_input()
{
    int tx = map.x, ty = map.y;

    char c, ct;
    printf("Input char: ");
        scanf("%c", &c);

    // to clear stdin
    while ( (ct = getchar()) != '\n' && ct != EOF ) { }

    switch(c)
    {
        case 'w': map.y++; break;
        case 's': map.y--; break;
        case 'a': map.x--; break;
        case 'd': map.x++; break;
        case 'q': map.x--; map.y++; break;
        case 'e': map.x++; map.y++; break;
        case 'c': map.x++; map.y--; break;
        case 'z': map.x--; map.y--; break;
        case 'r': running = false; break;
    }

    if( (!is_on_map(map.x, map.y)) ||
        (map.tiles[map.y * map_size + map.x].landscape == MOUNTAIN) )
    {
        map.x = tx; map.y = ty;
    }
}

int main(int argc, char **argv)
{
    parse_args(argc, argv);
    printf("Controls: 'wasd' and 'qezc'\nTo leave press: 'q'");

    map_init();
    trav_init();

    bfs_V2(&map, &trav, map.x, map.y, path_lenght);
    map_print_trav(&map, &trav);


    while (running) {

        process_input();


        map.str_init = false;
        map.trav_land_init = false;
        trav_clear();
        bfs_V2(&map, &trav, map.x, map.y, path_lenght);
        map_print_trav(&map, &trav);
        // printf("Input char: ");
    }

    return 0;
}
