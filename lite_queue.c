#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "common.h"
#include "lite_queue.h"

void lite_queue_init(lite_queue_t* q)
{
  q->q = malloc(sizeof(vertex_t) * MAX_LITE_QUEUE_SIZE);
  q->rear = 0;
  q->front = 0;
}
void lite_enqueue(lite_queue_t* q, vertex_t v)
{
  q->q[q->front] = v;
  q->front++;
}
vertex_t lite_dequeue(lite_queue_t* q)
{
  return q->q[(q->rear)++];
}

bool lite_queue_is_empty(lite_queue_t* q)
{
  return (q->rear == q->front);
}

void lite_queue_test(void)
{
  vertex_t v;
  lite_queue_t q;
  lite_queue_init(&q);
  lite_enqueue(&q, (vertex_t){0, 0});
  v = lite_dequeue(&q);
  printf("%d %d\n", v.x, v.y);

  lite_enqueue(&q, (vertex_t){1, 0});
  lite_enqueue(&q, (vertex_t){2, 0});
  lite_enqueue(&q, (vertex_t){3, 0});


  v = lite_dequeue(&q);
  printf("%d %d\n", v.x, v.y);
  v = lite_dequeue(&q);
  printf("%d %d\n", v.x, v.y);
  v = lite_dequeue(&q);
  printf("%d %d\n", v.x, v.y);
  
}


