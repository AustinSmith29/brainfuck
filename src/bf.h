#ifndef __BF_H
#define __BF_H
#include "stack.h"

#define MEM_SIZE  30000
#define MAX_FUNCS 100

struct Environment
{
  char *functions[MAX_FUNCS];
  Stack *stack;
  int nfuncs;
};

struct Environment *create_env();
void destroy_env(struct Environment *env);

int bf_read(char buf[], int max);
int bf_run(char program[], struct Environment *env);
#endif
