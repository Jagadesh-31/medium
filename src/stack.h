#ifndef STACK_H
#define STACK_H

// Stack node structure
typedef struct StackNode {
    int data;               // Data stored in stack node
    struct StackNode* next; // Pointer to next node in stack
} StackNode;

// Stack structure
typedef struct {
    StackNode* top;         // Pointer to top of stack
    int size;               // Current size of stack
} Stack;

// Function declarations
Stack* create_stack();
void free_stack(Stack* stack);
void stack_push(Stack* stack, int data);
int stack_pop(Stack* stack);
int stack_peek(Stack* stack);
int stack_is_empty(Stack* stack);

#endif