#ifndef bar_h
#define bar_h

class bar
{
public:
  int BarDoNothing(int i) 
    {
      return i;
    }
};


#ifdef CABLE_CONFIGURATION
namespace _cable_
{
  const char* const group="bar";
  namespace wrappers
  {
    typedef ::bar bar;
  }
}

void cable_instantiate()
{
  using namespace _cable_::wrappers;
  sizeof(::bar);
}
#endif
#endif
