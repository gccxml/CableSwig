#ifndef B_H
#define B_H

//#include "namespace2.h"

namespace Foo
{
  class Bar //: public Car
  {
  public:
    int DoNothing(int i) 
      {
        return i;
      }
  };
}

namespace Foo
{
  class Bill //: public Car
  {
  public:
    Foo::Bar DoStuff(int i) 
      {
        Foo::Bar b;
        return b;
      }
  };
}
#endif
