#ifndef ALG_H
#define ALG_H

typedef struct lite_queue_t lite_queue_t;

typedef struct 
{
  int processed;
  int range;
} trav_tile_t;

typedef struct trav_t 
{
  trav_tile_t* g;   // graph
  lite_queue_t q; // queue
} trav_t;


void trav_init(trav_t* trav);
void trav_reinit(trav_t* trav, int l);
void trav_close(trav_t* trav);

void trav_clear(trav_t* trav);
void trav_print(trav_t* trav);
void bfs_V1(map_t* map, int start_x, int start_y, int path_lenght);
void bfs_V2(map_t* map, trav_t* trav, int start_x, int start_y, int path_lenght);
void bfs_V3(map_t* map, trav_t* trav, int start_x, int start_y, int path_lenght);

bool is_on_trav(trav_t* trav, int x, int y);
void trav_print(trav_t* trav);
#endif /* end of include guard: ALG_H */
