#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#define MAX_PATH_LENGTH 9
#define MIN_PATH_LENGTH 1
#define DEFAULT_MAP_SIZE 24
#define DEFAULT_MOUNTAINS_DENSITY 999
#define DEFAULT_PATH_LENGTH 4
#define MAX_LITE_QUEUE_SIZE 1024

typedef struct vertex_t
{
  int x, y;
} vertex_t;

#endif /* end of include guard: COMMON_H_INCLUDED */
