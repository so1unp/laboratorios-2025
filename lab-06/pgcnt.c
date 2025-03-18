#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  printf(1, "%d\n", pgcnt());

  b = sbrk(4096);
  printf(1, "%d\n", pgcnt());

  b = malloc(1);
  printf(1, "%d\n", pgcnt());

  exit();
}