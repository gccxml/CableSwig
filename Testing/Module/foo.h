#ifndef foo_h
#define foo_h

#undef CABLE_CONFIGURATION
#include <bar.h>

class foo : public bar
{
public:
  int DoNothing(int i) 
    {
      return i;
    }
};

#define CABLE_CONFIGURATION
#ifdef CABLE_CONFIGURATION
namespace _cable_
{
  const char* const group="foo";
  namespace wrappers
  {
    typedef ::foo foo;
  }
}

#endif
#endif
