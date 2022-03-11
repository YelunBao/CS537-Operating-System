#include "types.h"
#include "user.h"
#include "pstat.h"

int
main(int argc, char *argv[])
{
  struct pstat pstat;
  getpinfo(&pstat);
  uint a = 100;
  
  srand(a);
  int pidA = getpid();
  
  settickets(pidA, 20);

  exit();
}
