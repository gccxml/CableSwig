#include <alloca.h>

int main()
{
  int* i = alloca(10);
  i[0] = 0;
  return i[0];
}

