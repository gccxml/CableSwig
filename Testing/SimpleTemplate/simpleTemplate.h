#ifndef simpleTemplate_h
#define simpleTemplate_h

template <class T>
class bar
{
  T barval;
public:
  void BarSetT(T v)
    {
      barval = v;
    }
  T BarGetT()
    {
      return barval;
    }
};


template <class T>
class foo : public bar<T>
{
  T val;
public:
  foo(T t) 
    {
      val = t;
    }
  foo()
    {
      val = (T)0;
    }
  void SetT(T v)
    {
      val = v;
    }
  T GetT()
    {
      return val;
    }
};


#ifdef CABLE_CONFIGURATION
namespace _cable_
{
  const char* const group="WrapTemplate";
  const char* const package="WrapTemplate";
  const char* const groups[]={"WrapTemplate"};
  namespace wrappers
  {
    typedef bar<int> intbar;
    typedef foo<int> intfoo;
  }
}

void cable_instantiate()
{
  using namespace _cable_::wrappers;
  sizeof(intfoo);
}
#endif
#endif
