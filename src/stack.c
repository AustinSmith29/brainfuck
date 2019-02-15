#include "stack.h"

#include <stdlib.h>
#include <stdio.h>

struct Stack
{
  int *stack;
  int top;
  int n_items;
  int capacity;
};

Stack *stack_create()
{
  Stack *s = malloc(sizeof(Stack));
  s->stack = malloc(sizeof(int) * 10);
  s->top = 0;
  s->n_items = 0;
  s->capacity = 10;

  return s;
}

void stack_destroy(Stack *stack)
{
  free(stack->stack);
  free(stack);
}

int stack_push(Stack *stack, int value)
{
  if (stack->top == stack->capacity-1)
    {
      const int GROWTH_FACTOR = 2;
      int new_capacity = stack->capacity * GROWTH_FACTOR;
      stack->stack = realloc(stack->stack, sizeof(int) * new_capacity);
      stack->capacity = new_capacity;
    }
  stack->stack[stack->top] = value;
  stack->top++;
  stack->n_items++;
  return 1;
}

int stack_pop(Stack *stack)
{
  int val = stack->stack[stack->top-1];
  stack->top--;
  stack->n_items--;
  return val;
}

int stack_empty(Stack *stack)
{
  if (stack->n_items == 0)
    return 1;
  return 0;
}
