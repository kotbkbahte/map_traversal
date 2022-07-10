#ifndef MAP_H
#define MAP_H

#include <stdbool.h>


typedef enum {PLAIN, MOUNTAIN} landscapes;

typedef struct trav_t trav_t;

typedef struct tile_t {
    int landscape;
    int processed;
} tile_t;

typedef struct map_t
{
    int x, y; // character position
    int size; 

    tile_t* tiles;
    
    char* str;
    char* trav_land;
    bool str_init;
    bool trav_land_init;
} map_t;

void map_init(map_t* map, int size);
void map_close(map_t* map);
void map_gen(map_t* map);
void map_print(map_t* map);
void map_print_processed(void);
void map_print_trav(map_t* map, trav_t* trav);
/*bool map_process_tile(map_t* map, int x, int y); */
bool is_on_map(map_t* map, int x, int y);


#endif /* end of include guard: MAP_H */
