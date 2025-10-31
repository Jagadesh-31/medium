#include "stack.h"
#include <stdlib.h>

// Creates a new empty stack
Stack* create_stack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

// Frees all memory used by stack
void free_stack(Stack* stack) {
    if (stack == NULL) return;
    
    // Pop all elements until stack is empty
    while (!stack_is_empty(stack)) {
        stack_pop(stack);
    }
    free(stack);
}

// Pushes new element onto stack
void stack_push(Stack* stack, int data) {
    StackNode* new_node = (StackNode*)malloc(sizeof(StackNode));
    new_node->data = data;
    new_node->next = stack->top;  // New node points to current top
    stack->top = new_node;        // Update top to new node
    stack->size++;
}

// Removes and returns top element from stack
int stack_pop(Stack* stack) {
    if (stack_is_empty(stack)) {
        return -1;  // Error value for empty stack
    }
    
    StackNode* temp = stack->top;
    int data = temp->data;
    stack->top = temp->next;  // Move top to next node
    free(temp);               // Free old top node
    stack->size--;
    
    return data;
}

// Returns top element without removing it
int stack_peek(Stack* stack) {
    if (stack_is_empty(stack)) {
        return -1;  // Error value for empty stack
    }
    return stack->top->data;
}

// Checks if stack is empty
int stack_is_empty(Stack* stack) {
    return stack->top == NULL;
}