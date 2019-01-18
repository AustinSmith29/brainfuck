#include <stdio.h>

void read_program(int buf[], int max)
{
  int c;
  int i = 0;
  while ((c = getchar()) != '\0' && i < max-1)
    {
      buf[i++] = c;
    }
  buf[i] = '\0';
}

int main(int argc, char *argv[])
{
  int mem_tape[4096] = {0};
  int program[4096];
  int in_ptr = 0;
  int mem_ptr = 0;
  int c;
  read_program(program, 4096);

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
          mem_tape[mem_ptr] = program[in_ptr+1];
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
                  printf("%d\n", depth);
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
      in_ptr++;
    }
  return 0;
}
