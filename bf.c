#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MEM_SIZE 30000

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

int main(int argc, char *argv[])
{
  if (argc > 2)
    {
      perror("No program detected. Usage bf [file]\n");
      return 1;
    }

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

  int c;
  while ((c = program[in_ptr]) != '\0')
    {
      switch (c)
        {
        case '>':
          if (mem_ptr > MEM_SIZE-1)
            {
              perror("Error: mem_ptr out of bounds.\n");
              return 1;
            }
          mem_ptr++;
          break;
        case '<':
          if (mem_ptr < 0)
            {
              perror("Error: mem_ptr out of bounds.\n");
              return 1;
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
      in_ptr++;
    }
  return 0;
}
