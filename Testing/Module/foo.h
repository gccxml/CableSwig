#ifndef foo_h
#define foo_h

class foo
{
public:
  int DoNothing(int i) 
    {
      return i;
    }
};


#ifdef CABLE_CONFIGURATION
namespace _cable_
{
  const char* const group="foo";
  namespace wrappers
  {
    typedef ::foo foo;
  }
}

void cable_instantiate()
{
  using namespace _cable_::wrappers;
  sizeof(::foo);
}
#endif
#endif
