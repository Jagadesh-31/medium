#include "queue.h"
#include <stdlib.h>

/**
 * Creates a new empty queue
 * Initializes all pointers and size counter
 */
Queue* create_queue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

/**
 * Adds an element to the rear of the queue (enqueue operation)
 * This is the standard FIFO insertion - new elements go to the back
 */
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

/**
 * Removes and returns element from front of queue (dequeue operation)
 * This is the standard FIFO removal - elements leave from the front
 */
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

/**
 * Checks if the queue is empty
 * Useful for loop conditions and validations
 */
int is_queue_empty(Queue* queue) {
    return (queue->front == NULL);
}

/**
 * Frees all memory allocated for the queue
 * Removes all remaining elements and frees the queue structure
 */
void free_queue(Queue* queue) {
    if (queue == NULL) return;
    
    // Remove all elements from the queue
    while (!is_queue_empty(queue)) {
        dequeue(queue);
    }
    
    // Free the queue structure itself
    free(queue);
}