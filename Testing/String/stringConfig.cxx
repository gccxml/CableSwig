#include <string>
#ifdef CABLE_CONFIGURATION
namespace _cable_
{
  const char* const group="WrapString";
  namespace wrappers
  {
    typedef ::std::string stdstring;
  }
}

void cable_instantiate()
{
  using namespace _cable_::wrappers;
  sizeof(stdstring);
}
  
#endif
