#ifndef LITE_QUEUE_H_INCLUDED
#define LITE_QUEUE_H_INCLUDED


#include <stdbool.h>

typedef struct vertex_t vertex_t;

/* fixed length queue */

typedef struct lite_queue_t
{
  vertex_t* q;
  int rear;  // or size_t
  int front; // or site_t

} lite_queue_t;

void lite_queue_init(lite_queue_t* q);
void lite_queue_close(lite_queue_t* q);
void lite_queue_clear(lite_queue_t* q);
void lite_enqueue(lite_queue_t* q, vertex_t v);
vertex_t lite_dequeue(lite_queue_t* q);
bool lite_queue_is_empty(lite_queue_t* q);

void lite_queue_test(void);
void lite_queue_print(lite_queue_t* q); 


#endif /* end of include guard: LITE_QUEUE_H_INCLUDED */
