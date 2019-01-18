#include <stdio.h>

void dump(int buf[])
{
  int i;
  for (i=0; i < 10; i++)
    {
      printf("%d ",buf[i]);
    }
  printf("\n");
}

void read_program(char buf[], int max, FILE *f)
{
  int c;
  int i = 0;
  while ((c = getc(f)) != '\0' && i < max-1)
    {
      buf[i++] = c;
    }
  buf[i] = '\0';
}

int main(int argc, char *argv[])
{
  int mem_tape[4096] = {0};
  char program[4096];
  int in_ptr = 0;
  int mem_ptr = 0;
  int c;
  FILE *f = fopen(argv[1], "r");
  read_program(program, 4096, f);

  while ((c = program[in_ptr]) != '\0')
    {
      switch (c)
        {
        case '>':
          mem_ptr++;
          break;
        case '<':
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
          mem_tape[mem_ptr] = getchar();
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
        }
      //dump(mem_tape);
      in_ptr++;
    }
  return 0;
}
