#include "queue.h"
#include <stdlib.h>


Queue* create_queue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}


int enqueue(Queue* queue, int val) {
    // Create new node for the value
    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
    if (new_node == NULL) return 0;  // Memory allocation failed
    
    new_node->val = val;
    new_node->next = NULL;
    
    // If queue is empty, new node is both front and rear
    if (queue->rear == NULL) {
        queue->front = new_node;
        queue->rear = new_node;
    } 
    // Otherwise, add to rear and update rear pointer
    else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    
    queue->size++;
    return 1;
}

int dequeue(Queue* queue) {
    // Check if queue is empty
    if (queue->front == NULL) {
        return -1;  // Indicate empty queue
    }
    
    // Save the front node and its value
    QueueNode* temp = queue->front;
    int val = temp->val;
    
    // Move front pointer to next node
    queue->front = queue->front->next;
    
    // If queue becomes empty, update rear pointer too
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    
    // Free the old front node and update size
    free(temp);
    queue->size--;
    
    return val;
}


int is_queue_empty(Queue* queue) {
    return (queue->front == NULL);
}

void free_queue(Queue* queue) {
    if (queue == NULL) return;
    
    // Remove all elements from the queue
    while (!is_queue_empty(queue)) {
        dequeue(queue);
    }
    
    // Free the queue structure itself
    free(queue);
}