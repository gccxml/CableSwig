/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctCalls.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctCalls_h
#define _ctCalls_h

#include "ctTclCxxObject.h"
#include "ctTypeInfo.h"
#include "ctWrapperFacility.h"

namespace _cable_tcl_
{

namespace
{
/**
 * Every type invovled in a wrapper should have a specialization of this
 * class with the following members:
 *   static CvQualifiedType type;
 *   typedef T NoCv;  // Type T without any top-level cv-qualifiers.
 * And if the type can be passed as an argument to a function:
 *   typedef ArgumentAs...<T> ArgumentFor;
 */
template <class T>
struct CvType;

/**
 * Every enumeration type that is returned from a method should have a
 * specialization of this class the following member:
 *   static void From(const T& result, const WrapperFacility* wrapperFacility);
 */
template <typename T>
struct ReturnEnum;
} // anonymous namespace

/**
 * When a method returns an object, the wrapper function calls this to
 * prepare the return.  This creates the instance, sets the Tcl object
 * result, and creates the Tcl command in case of nested command
 * calls.
 */
template <typename T>
struct Return
{
  typedef typename CvType<T>::NoCv NoCvT;
  static void From(const T& result, const WrapperFacility* wrapperFacility)
    {
    Tcl_Interp* interp = wrapperFacility->GetInterpreter();
    CxxObject* cxxObject =
      wrapperFacility->GetCxxObjectFor(Anything(new NoCvT(result)),
                                       CvType<T>::type.GetType());
    Tcl_SetObjResult(interp, Tcl_NewCxxObjectObj(cxxObject));
    }
  static void FromConstructor(T* result, const WrapperFacility* wrapperFacility)
    {
    Tcl_Interp* interp = wrapperFacility->GetInterpreter();
    CxxObject* cxxObject =
      wrapperFacility->GetCxxObjectFor(Anything(result),
                                       CvType<T>::type.GetType());
    Tcl_SetObjResult(interp, Tcl_NewCxxObjectObj(cxxObject));
    }
};


/**
 * A specialization for a method with no return type.
 */
template <>
struct Return<void>
{ static _cable_tcl_EXPORT void From(const WrapperFacility*); };


/*@{
 * A specialization for returning an object that can be treated as a
 * predefined Tcl object type.
 */
template <>
struct Return<bool>
{ static _cable_tcl_EXPORT void From(bool result, const WrapperFacility*); };

template <>
struct  Return<short>
{ static _cable_tcl_EXPORT void From(short result, const WrapperFacility*); };

template <>
struct  Return<unsigned short>
{ static _cable_tcl_EXPORT void From(unsigned short result, const WrapperFacility*); };

template <>
struct  Return<int>
{ static _cable_tcl_EXPORT void From(int result, const WrapperFacility*); };

template <>
struct  Return<unsigned int>
{ static _cable_tcl_EXPORT void From(unsigned int result, const WrapperFacility*); };

template <>
struct  Return<long>
{ static _cable_tcl_EXPORT void From(long result, const WrapperFacility*); };

template <>
struct Return<unsigned long>
{ static _cable_tcl_EXPORT void From(unsigned long result, const WrapperFacility*); };

template <>
struct  Return<float>
{ static _cable_tcl_EXPORT void From(float result, const WrapperFacility*); };

template <>
struct  Return<double>
{ static _cable_tcl_EXPORT void From(double result, const WrapperFacility*); };
//@}
  
  
/**
 * When a method returns a pointer type, the wrapper function calls this to
 * prepare the return.  This creates the Pointer Tcl object for the
 * Tcl object result.
 */
template <typename T>
struct ReturnPointerTo
{
  static void From(T* result, const WrapperFacility* wrapperFacility)
    {
    Tcl_Interp* interp = wrapperFacility->GetInterpreter();
    CxxObject* cxxObject = 
      wrapperFacility->GetCxxObjectFor(Anything(result),
                                       CvType<T*>::type.GetType());
    Tcl_SetObjResult(interp, Tcl_NewCxxObjectObj(cxxObject));
    }
};


/**
 * A specialization of ReturnPointerTo for char* to convert to a Tcl String
 * object.
 */
template <>
struct ReturnPointerTo<char>
{ static _cable_tcl_EXPORT void From(char* result, const WrapperFacility*); };


/**
 * A specialization of ReturnPointerTo for const char* to convert to a
 * Tcl String object.
 */
template <>
struct ReturnPointerTo<const char>
{ static _cable_tcl_EXPORT void From(const char* result, const WrapperFacility*); };


/**
 * When a method returns a reference type, the wrapper function calls this to
 * prepare the return.  This creates the Reference Tcl object for the
 * Tcl object result.
 */
template <typename T>
struct ReturnReferenceTo
{
  static void From(T& result, const WrapperFacility* wrapperFacility)
    {
    Tcl_Interp* interp = wrapperFacility->GetInterpreter();
    CxxObject* cxxObject =
      wrapperFacility->GetCxxObjectFor(Anything(&result),
                                       CvType<T&>::type.GetType());
    Tcl_SetObjResult(interp, Tcl_NewCxxObjectObj(cxxObject));
    }
};


/*@{
 * A specialization for returning a reference to a const object that
 * can be treated as a predefined Tcl object type.
 */
template <>
struct ReturnReferenceTo<const bool>
{ static _cable_tcl_EXPORT void From(const bool& result, const WrapperFacility*); };

template <>
struct  ReturnReferenceTo<const short>
{ static _cable_tcl_EXPORT void From(const short& result, const WrapperFacility*); };

template <>
struct  ReturnReferenceTo<const unsigned short>
{ static _cable_tcl_EXPORT void From(const unsigned short& result, const WrapperFacility*); };

template <>
struct  ReturnReferenceTo<const int>
{ static _cable_tcl_EXPORT void From(const int& result, const WrapperFacility*); };

template <>
struct  ReturnReferenceTo<const unsigned int>
{ static _cable_tcl_EXPORT void From(const unsigned int& result, const WrapperFacility*); };

template <>
struct  ReturnReferenceTo<const long>
{ static _cable_tcl_EXPORT void From(const long& result, const WrapperFacility*); };

template <>
struct ReturnReferenceTo<const unsigned long>
{ static _cable_tcl_EXPORT void From(const unsigned long& result, const WrapperFacility*); };

template <>
struct  ReturnReferenceTo<const float>
{ static _cable_tcl_EXPORT void From(const float& result, const WrapperFacility*); };

template <>
struct  ReturnReferenceTo<const double>
{ static _cable_tcl_EXPORT void From(const double& result, const WrapperFacility*); };
//@}


/**
 * Base class for ArgumentAsInstanceOf to implement non-templated
 * portion of the functor.
 */
class _cable_tcl_EXPORT ArgumentAsInstanceBase
{
protected:
  ArgumentAsInstanceBase(const WrapperFacility* wrapperFacility,
                         const Type* type):
    m_WrapperFacility(wrapperFacility),
    m_ConversionFunction(0),
    m_To(type) {}

  bool FindConversionFunction(const CvQualifiedType& from);
  
  ///! The WrapperFacility for which this is handling an argument.
  const WrapperFacility* m_WrapperFacility;
  
  ///! The target type to which a conversion is being performed.
  const Type* m_To;
  
  ///! A pointer to the conversion function if it was found.
  ConversionFunction m_ConversionFunction;
};


/**
 * Base class for ArgumentAsPointerTo and ArgumentAsPointerToFunction
 * to implement non-templated portion of the functors.
 */
class _cable_tcl_EXPORT ArgumentAsPointerBase
{
protected:
  ArgumentAsPointerBase(const WrapperFacility* wrapperFacility,
                        const Type* type):
    m_WrapperFacility(wrapperFacility),
    m_ConversionFunction(0),
    m_To(PointerType::SafeDownCast(type)) {}

  bool FindConversionFunction(const CvQualifiedType& from);
  
  ///! The Wrapper for which this is handling an argument.
  const WrapperFacility* m_WrapperFacility;
  
  ///! The target type to which a conversion is being performed.
  const PointerType* m_To;
  
  ///! A pointer to the conversion function if it was found.
  ConversionFunction m_ConversionFunction;
};


/**
 * Base class for ArgumentAsPointerTo_array to implement non-templated
 * portion of the functor.
 */
class _cable_tcl_EXPORT ArgumentAsPointerBase_array
{
protected:
  ArgumentAsPointerBase_array(const WrapperFacility* wrapperFacility,
                              const Type* type,
                              const CvQualifiedType& elementType):
    m_WrapperFacility(wrapperFacility),
    m_ConversionFunction(0),
    m_To(PointerType::SafeDownCast(type)),
    m_ElementType(elementType),
    m_NeedArray(false) {}

  bool FindConversionFunction(const CvQualifiedType& from);
  
  ///! The WrapperFacility for which this is handling an argument.
  const WrapperFacility* m_WrapperFacility;
  
  ///! The target type to which a conversion is being performed.
  const PointerType* m_To;
  
  ///! A pointer to the conversion function if it was found.
  ConversionFunction m_ConversionFunction;
  
  ///! If a temporary array is needed, this is its element type.
  CvQualifiedType m_ElementType;
  
  ///! Flag for whether a temporary array must be allocated.
  bool m_NeedArray;
};


/**
 * Base class for ArgumentAsReferenceTo to implement non-templated
 * portion of the functor.
 */
class _cable_tcl_EXPORT ArgumentAsReferenceBase
{
protected:
  ArgumentAsReferenceBase(const WrapperFacility* wrapperFacility,
                          const Type* type):
    m_WrapperFacility(wrapperFacility),
    m_ConversionFunction(0),
    m_To(ReferenceType::SafeDownCast(type)) {}

  bool FindConversionFunction(const CvQualifiedType& from);
  bool FindDirectConversionFunction(const CvQualifiedType& from);
  
  ///! The WrapperFacility for which this is handling an argument.
  const WrapperFacility* m_WrapperFacility;
  
  ///! The target type to which a conversion is being performed.
  const ReferenceType* m_To;
  
  ///! A pointer to the conversion function if it was found.
  ConversionFunction m_ConversionFunction;
};


/**
 * Base class for ArgumentAsReferenceTo_const to implement
 * non-templated portion of the functor.
 */
class _cable_tcl_EXPORT ArgumentAsReferenceBase_const:
  public ArgumentAsReferenceBase
{
protected:
  ArgumentAsReferenceBase_const(const WrapperFacility* wrapperFacility,
                                const Type* type, const Type* tempType):
    ArgumentAsReferenceBase(wrapperFacility, type),
    m_TempType(tempType),
    m_NeedTemporary(false) {}

  bool FindConversionFunction(const CvQualifiedType& from);

  ///! The type of a temporary if it is allocated.
  const Type* m_TempType;
  
  ///! Flag for whether a temporary is needed to do the conversion.
  bool m_NeedTemporary;
};


/**
 * Functor to convert an Argument to an object of T.
 */
template <typename T>
class ArgumentAsInstanceOf: public ArgumentAsInstanceBase
{
public:
  ArgumentAsInstanceOf(const WrapperFacility* wrapperFacility):
    ArgumentAsInstanceBase(wrapperFacility, CvType<T>::type.GetType()) {}
  T operator()(const Argument&);
};


/**
 * Functor to convert an Argument to a pointer to T.
 */
template <typename T>
class ArgumentAsPointerTo: public ArgumentAsPointerBase
{
public:
  ArgumentAsPointerTo(const WrapperFacility* wrapperFacility):
    ArgumentAsPointerBase(wrapperFacility, CvType<T*>::type.GetType()) {}
  T* operator()(const Argument&);
};


/**
 * Functor to convert an Argument to a pointer to T.  This version can
 * optionally point to an array of T.
 */
template <typename T>
class ArgumentAsPointerTo_array: public ArgumentAsPointerBase_array
{
public:
  ArgumentAsPointerTo_array(const WrapperFacility* wrapperFacility):
    ArgumentAsPointerBase_array(wrapperFacility,
                                CvType<T*>::type.GetType(),
                                CvType<T>::type),
    m_Array(0) {}  
  ~ArgumentAsPointerTo_array() { if(m_Array) { delete [] m_Array; } }
  T* operator()(const Argument&);
private:  
  typedef typename CvType<T>::NoCv NoCvT;
  typedef typename CvType<T>::ArgumentFor ElementFor;
  
  ///! If the pointer is to an array, this holds it.
  NoCvT* m_Array;
};



/**
 * Functor to convert an Argument to a pointer to function of type T.
 */
template <typename T>
class ArgumentAsPointerToFunction: public ArgumentAsPointerBase
{
public:
  ArgumentAsPointerToFunction(const WrapperFacility* wrapperFacility):
    ArgumentAsPointerBase(wrapperFacility, CvType<T*>::type.GetType()) {}
  T* operator()(const Argument&);
};


/**
 * Functor to convert an Argument to a reference to T.
 */
template <typename T>
class ArgumentAsReferenceTo: public ArgumentAsReferenceBase
{
public:
  ArgumentAsReferenceTo(const WrapperFacility* wrapperFacility):
    ArgumentAsReferenceBase(wrapperFacility, CvType<T&>::type.GetType()) {}
  T& operator()(const Argument&);
};


/**
 * Functor to convert an Argument to a reference to const T.
 * This version is separate from ArgumentAsReferenceTo since a temporary
 * may be constructed.
 */
template <typename T>
class ArgumentAsReferenceTo_const: public ArgumentAsReferenceBase_const
{
public:
  ArgumentAsReferenceTo_const(const WrapperFacility* wrapperFacility):
    ArgumentAsReferenceBase_const(wrapperFacility,
                                  CvType<const T&>::type.GetType(),
                                  CvType<T>::type.GetType()),
    m_Temporary(0) {}
  ~ArgumentAsReferenceTo_const()
    { if(m_Temporary) { delete const_cast<T*>(m_Temporary); } }  
  const T& operator()(const Argument&);
private:
  ///! If a temporary of type const T is needed, this will point to it.
  const T* m_Temporary;
};
 
} // namespace _cable_tcl_

#ifndef _cable_tcl_MANUAL_INSTANTIATION
#include "ctCalls.txx"
#endif
 
#endif
