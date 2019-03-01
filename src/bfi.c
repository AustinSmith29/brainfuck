#include "bf.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
  char prog[1000];
  struct Environment *env = create_env();
  if (bf_read(prog, 999) < 0)
  {
     printf("Incorrect program!\n");
     return -1;
  }
  bf_run(prog, env);
  destroy_env(env);
  return 0;
}
