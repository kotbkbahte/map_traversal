#ifndef LITE_QUEUE_H_INCLUDED
#define LITE_QUEUE_H_INCLUDED

#include <stdbool.h>

typedef struct vertex_t vertex_t;

/* fixed length queue */
#define MAX_LITE_QUEUE_SIZE 128

typedef struct lite_queue_t
{
  vertex_t* q;
  int rear;  // size_t 
  int front; // site_t

  
} lite_queue_t;

void lite_queue_init(lite_queue_t* q);
void lite_enqueue(lite_queue_t* q, vertex_t v);
vertex_t lite_dequeue(lite_queue_t* q);
bool lite_queue_is_empty(lite_queue_t* q);

void lite_queue_test(void);


#endif /* end of include guard: LITE_QUEUE_H_INCLUDED */
