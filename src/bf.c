#include "bf.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Environment *create_env()
{
  struct Environment *env = malloc(sizeof(struct Environment));
  env->stack = stack_create();
  env->nfuncs = 0;
  return env;
}

void destroy_env(struct Environment *env)
{
  stack_destroy(env->stack);
  free(env);
  env = NULL;
}

int bf_read(char buf[], int max)
{
  int c, i = 0;
  int nlbracket = 0;
  int nrbracket = 0;

  while ((c = getchar()) != EOF && i < max-1)
    {
      if (c == '[')
        nlbracket++;
      if (c == ']')
        nrbracket++;
      buf[i++] = c;
    }
  buf[i] = '\0';

  // Brackets can't be unbalanced.
  if (nlbracket != nrbracket)
      return -1;
  return 0;
}

int bf_run(char program[], struct Environment *env)
{
#define MEM_CURRENT mem[memp]
  unsigned char mem[MEM_SIZE];
  memset(mem, 0, MEM_SIZE);
  unsigned int ip = 0;
  unsigned int memp = 0;

  int c;
  while ((c = program[ip]) != '\0')
    {
      switch (c)
        {
        case '>':
          if (memp > MEM_SIZE - 1)
              return -EFAULT;
          memp++;
          break;
        case '<':
          if (memp < 0)
              return -EFAULT;
          memp--;
          break;
        case '+':
          MEM_CURRENT++;
          break;
        case '-':
          MEM_CURRENT--;
          break;
        case '.':
          putchar(MEM_CURRENT);
          break;
        case ',':
          printf("$ ");
          int c = getchar();
          // We don't want EOF to change value in memory cell.
          if (c != EOF)
              MEM_CURRENT = c;
          printf("\n");
          break;
        case '[':
          if (MEM_CURRENT == 0)
            {
              int depth = 1;
              while (depth != 0)
                {
                  ip++;
                  if (program[ip] == '[')
                      depth++;
                  if (program[ip] == ']')
                      depth--;
                }
            }
          break;
        case ']':
          if (MEM_CURRENT != 0)
            {
              int depth = 1;
              while (depth != 0)
                {
                  ip--;
                  if (program[ip] == ']')
                      depth++;
                  if (program[ip] == '[')
                      depth--;
                }
            }
          break;
        case '#':
          if (stack_push(env->stack, MEM_CURRENT) < 0)
              return -ENOMEM;
          break;
        case '$':
          if (stack_empty(env->stack))
              return -EFAULT;
          MEM_CURRENT = stack_pop(env->stack);
          break;
        case '@':
          if (MEM_CURRENT > env->nfuncs-1)
            return -EFAULT;
          bf_run(program, env);
          break;
        }
      ip++;
    }
  return 0;
}
