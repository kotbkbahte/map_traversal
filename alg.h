#ifndef ALG_H
#define ALG_H

typedef struct {
    int processed;
    int range;
} trav_tile_t;

typedef struct trav_t {
    trav_tile_t* g;
} trav_t;


void trav_init(void);
void trav_reinit(int l);
void trav_clear(void);
void trav_print(void);
void bfs_V1(map_t* map, int start_x, int start_y, int path_lenght);
void bfs_V2(map_t* map, trav_t* trav, int start_x, int start_y, int path_lenght);
void bfs_V3(map_t* map, trav_t* trav, int start_x, int start_y, int path_lenght);

bool is_on_trav(int x, int y);
void trav_print(void);
#endif /* end of include guard: ALG_H */
