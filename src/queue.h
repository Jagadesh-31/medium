#ifndef QUEUE_H
#define QUEUE_H
typedef struct QueueNode {
int val;
struct QueueNode* next;
} QueueNode;
typedef struct {
QueueNode* front;
QueueNode* rear;
int size;
} Queue;
Queue* create_queue();
int enqueue(Queue* queue, int val);
int dequeue(Queue* queue);
int is_queue_empty(Queue* queue);
void free_queue(Queue* queue);
#endif