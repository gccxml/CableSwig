namespace Foo
{
  class Bar
  {
  public:
    int DoNothing(int i) 
      {
        return i;
      }
  };
}
#ifdef CABLE_CONFIGURATION
#include "namespace2.h"
namespace _cable_
{
  const char* const group="NameSpace";
  namespace wrappers
  {
    typedef Foo::Bar Bar;
    typedef Foo::Car Car;
  }
}

void cable_instantiate()
{
  using namespace _cable_::wrappers;
  sizeof(Bar);
}
#endif
