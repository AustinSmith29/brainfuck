#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "stack.h"

#define MEM_SIZE 30000
#define MAX_FUNCS 100

void setup_env(Stack *stack, char *functions[]);
void read_program(char buff[], int max, FILE *f);
/* TODO: put stack and functions inside an environment struct */
int execute_program(char program[], Stack *stack, char *functions[]);

int main(int argc, char *argv[])
{
  if (argc > 2)
    {
      perror("No program detected. Usage bf [file]\n");
      return 1;
    }

  Stack *stack = stack_create();
  char *functions[MAX_FUNCS];
  setup_env(stack, functions);
  unsigned int mem_tape[MEM_SIZE] = {0};
  char program[4096];
  int in_ptr = 0;
  int mem_ptr = 0;

  FILE *f = fopen(argv[1], "r");
  if (!f)
    {
      perror("File not found.\n");
      return 1;
    }
  read_program(program, 4096, f);
  fclose(f);
  if (execute_program(program, stack, functions) < 0)
    {
      perror("Program failure!\n");
    }
  stack_destroy(stack);
  return 0;
}

void setup_env(Stack *stack, char *functions[])
{
  /*TODO: Load all programs in func directory into functions. */
  // For right now lets just do a basic function to test.
  functions[0] = "$+#";
}

void read_program(char buf[], int max, FILE *f)
{
  int c;
  int i = 0;
  int nlbracket = 0;
  int nrbracket = 0;

  while ((c = getc(f)) != '\0' && i < max-1)
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
    {
      perror("Error: Unbalanced brackets.\n");
      fclose(f);
      exit(1);
    }
}

int execute_program(char program[], Stack *stack, char *functions[])
{
  unsigned int mem_tape[MEM_SIZE] = {0};
  int in_ptr = 0;
  int mem_ptr = 0;

  int c;
  while ((c = program[in_ptr]) != '\0')
    {
      switch (c)
        {
        case '>':
          if (mem_ptr > MEM_SIZE-1)
            {
              perror("Error: mem_ptr out of bounds.\n");
              return -1;
            }
          mem_ptr++;
          break;
        case '<':
          if (mem_ptr < 0)
            {
              perror("Error: mem_ptr out of bounds.\n");
              return -1;
            }
          mem_ptr--;
          break;
        case '+':
          mem_tape[mem_ptr]++;
          break;
        case '-':
          mem_tape[mem_ptr]--;
          break;
        case '.':
          putchar(mem_tape[mem_ptr]);
          break;
        case ',':
          printf("$ ");
          int c = getchar();
          // We don't want EOF to change value in mem cell.
          if (c != EOF)
            mem_tape[mem_ptr] = c;
          printf("\n");
          break;
        case '[':
          if (mem_tape[mem_ptr] == 0)
            {
              int depth = 1;
              while (depth != 0)
                {
                  in_ptr++;
                  if (program[in_ptr] == '[')
                    depth++;
                  if (program[in_ptr] == ']')
                    depth--;
                }
            }
          break;
        case ']':
          if (mem_tape[mem_ptr] != 0)
            {
              int depth = 1;
              while (depth != 0)
                {
                  in_ptr--;
                  if (program[in_ptr] == ']')
                    depth++;
                  if (program[in_ptr] == '[')
                    depth--;
                }
            }
          break;
        case '#':
          stack_push(stack, mem_tape[mem_ptr]);
          break;
        case '$':
          mem_tape[mem_ptr] = stack_pop(stack);
          break;
        case '@':
          execute_program(functions[mem_tape[mem_ptr]], stack, functions);
          break;
        }
      in_ptr++;
    }
  return 1;
}
