#include "bf.h"

int main(int argc, char *argv[])
{
  char prog[1000];
  struct Environment *env = create_env();
  bf_read(prog, 999);
  bf_run(prog, env);
  destroy_env(env);
  return 0;
}
