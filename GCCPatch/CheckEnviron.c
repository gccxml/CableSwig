#include <unistd.h>
int main()
{
#ifndef environ
  char *(*pfn) = (char *(*)) environ;
#endif
  return 0;
}
