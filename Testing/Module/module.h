#ifndef Module_h
#define Module_h


#ifdef CABLE_CONFIGURATION
namespace _cable_
{
  const char* const group="TestModule";
  const char* const package="TestModulePackage";
  const char* const groups[]={"foo", "bar"};
}
#endif
#endif
