#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#ifdef _WIN32
#define CLEAR "cls"
#else //In any other OS
#define CLEAR "clear"
#endif

#include "lite_queue.h"
#include "map.h"
#include "alg.h"

int running = true;
int map_size;
int mountains_density;
int path_lenght;
int fast_flag = false;
int test_flag = false;
bool default_settings = false;
map_t map;
trav_t trav;

void set_default(void)
{
    map_size = 24;
    mountains_density = 4;
    path_lenght = 4;
    map.x = map_size / 2;
    map.y = map_size / 2;
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
    while ((c = getopt(argc, argv, "fdt")) != -1)
        switch (c)
            {
            case 'd':
                default_settings = true;
                break;
            case 'f':
                fast_flag = true;
                break;
            case 't':
              test_flag = true;
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
      case 't': test_flag = true; break;
      case 'w': map.y++; break;
      case 's': map.y--; break;
      case 'a': map.x--; break;
      case 'd': map.x++; break;
      case 'q': map.x--; map.y++; break;
      case 'e': map.x++; map.y++; break;
      case 'c': map.x++; map.y--; break;
      case 'z': map.x--; map.y--; break;
      case '+': if(path_lenght == 9) {printf("Path length must be <= 9.\n"); break;} path_lenght++; break;
      case '-': if(path_lenght == 1) {printf("Path length must be >= 1.\n"); break;} path_lenght--; break;
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
    
    if(test_flag)
    {
      map_init();
      trav_reinit(12);
      lite_queue_test();
/*      trav_print();*/
      bfs_V3(&map, &trav, map.x, map.y, path_lenght);
      map_print_trav(&map, &trav);
/*      trav_print();*/
      return 1;
    } 
    
    printf("Controls: 'wasd' and 'qezc'\nTo leave press: 'q'\n");

    
    map_init();
    trav_init();

    bfs_V3(&map, &trav, map.x, map.y, path_lenght);
    printf("To start press <enter>\n");
    if(getchar());
    system(CLEAR);


    
    while (running) {
        map.str_init = false;
        map.trav_land_init = false;
        
        system(CLEAR);
        trav_reinit(path_lenght);
        bfs_V3(&map, &trav, map.x, map.y, path_lenght);
        map_print_trav(&map, &trav);
        trav_print();


        process_input();
    }

    return 0;
}
