#ifndef __STACK_H
#define __STACK_H

struct Stack;
typedef struct Stack Stack;

Stack *stack_create();
void stack_destroy(Stack *stack);

int stack_push(Stack *stack, int value);
int stack_pop(Stack *stack);
int stack_empty(Stack *stack);
#endif
