/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctCalls.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "ctCalls.h"
#include "ctException.h"

#include <string.h>

namespace _cable_tcl_
{

void Return<void>::From(const WrapperFacility* wrapperFacility)
{
  Tcl_ResetResult(wrapperFacility->GetInterpreter());
}

void Return<bool>::From(bool result, const WrapperFacility* wrapperFacility)
{
  int boolValue = result;
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewBooleanObj(boolValue));
}

void Return<short>::From(short result, const WrapperFacility* wrapperFacility)
{
  int intValue = result;
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewIntObj(intValue));
}

void Return<unsigned short>::From(unsigned short result, const WrapperFacility* wrapperFacility)
{
  int intValue = result;
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewIntObj(intValue));
}

void Return<int>::From(int result, const WrapperFacility* wrapperFacility)
{
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewIntObj(result));
}

void Return<unsigned int>::From(unsigned int result, const WrapperFacility* wrapperFacility)
{
  int intValue = result;
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewIntObj(intValue));
}

void Return<long>::From(long result, const WrapperFacility* wrapperFacility)
{
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewLongObj(result));
}

void Return<unsigned long>::From(unsigned long result, const WrapperFacility* wrapperFacility)
{
  long longValue = result;
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewLongObj(longValue));
}

void Return<float>::From(float result, const WrapperFacility* wrapperFacility)
{
  double doubleValue = result;
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewDoubleObj(doubleValue));
}  

void Return<double>::From(double result, const WrapperFacility* wrapperFacility)
{
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewDoubleObj(result));
}
  
void ReturnPointerTo<char>::From(char* result, const WrapperFacility* wrapperFacility)
{
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewStringObj(result, -1));
}

void ReturnPointerTo<const char>::From(const char* result,
                                       const WrapperFacility* wrapperFacility)
{
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(),
                   Tcl_NewStringObj(const_cast<char*>(result), -1));
}


void ReturnReferenceTo<const bool>::From(const bool& result, const WrapperFacility* wrapperFacility)
{
  int boolValue = result;
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewBooleanObj(boolValue));
}

void ReturnReferenceTo<const short>::From(const short& result, const WrapperFacility* wrapperFacility)
{
  int intValue = result;
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewIntObj(intValue));
}

void ReturnReferenceTo<const unsigned short>::From(const unsigned short& result, const WrapperFacility* wrapperFacility)
{
  int intValue = result;
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewIntObj(intValue));
}

void ReturnReferenceTo<const int>::From(const int& result, const WrapperFacility* wrapperFacility)
{
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewIntObj(result));
}

void ReturnReferenceTo<const unsigned int>::From(const unsigned int& result, const WrapperFacility* wrapperFacility)
{
  int intValue = result;
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewIntObj(intValue));
}

void ReturnReferenceTo<const long>::From(const long& result, const WrapperFacility* wrapperFacility)
{
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewLongObj(result));
}

void ReturnReferenceTo<const unsigned long>::From(const unsigned long& result, const WrapperFacility* wrapperFacility)
{
  long longValue = result;
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewLongObj(longValue));
}

void ReturnReferenceTo<const float>::From(const float& result, const WrapperFacility* wrapperFacility)
{
  double doubleValue = result;
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewDoubleObj(doubleValue));
}  

void ReturnReferenceTo<const double>::From(const double& result, const WrapperFacility* wrapperFacility)
{
  Tcl_SetObjResult(wrapperFacility->GetInterpreter(), Tcl_NewDoubleObj(result));
}


/**
 * Try to find a conversion to type m_To from the given type.  Returns
 * whether an identity is allowed.  If false is returned,
 * m_ConversionFunction will point to the conversion function if it
 * was found.
 */
bool
ArgumentAsInstanceBase
::FindConversionFunction(const CvQualifiedType& from)
{
  // If the "from" and "to" types are the same, we can use an identity.
  if(Type::Equal(m_To, from.GetType()))
    {
    return true;
    }
  else
    {
    // We don't have a trivial conversion.  Try to lookup the
    // conversion function.
    m_ConversionFunction = m_WrapperFacility->GetConversion(from, m_To);
    }  

  if(!m_ConversionFunction)
    {
    // No conversion function was found, but one is needed.
    throw _cable_tcl_UnknownConversionException(from.GetName(), m_To->Name());
    }
  
  return false;
}


/**
 * Try to find a conversion to type m_To from the given type.  Returns
 * whether an identity is allowed.  If false is returned,
 * m_ConversionFunction will point to the conversion function if it
 * was found.
 */
bool
ArgumentAsPointerBase
::FindConversionFunction(const CvQualifiedType& from)
{
  // If the "from" type is a pointer and the conversion is a valid
  // cv-qualifier adjustment, we can use an identity.
  if(from.GetType()->IsEitherPointerType()
     && Conversions::IsValidQualificationConversion(
       PointerType::SafeDownCast(from.GetType()), m_To))
    {
    return true;
    }
  else
    {
    // We don't have a trivial conversion.  Try to lookup the
    // conversion function.
    m_ConversionFunction = m_WrapperFacility->GetConversion(from, m_To);
    }

  if(!m_ConversionFunction)
    {
    // No conversion function was found, but one is needed.
    throw _cable_tcl_UnknownConversionException(from.GetName(), m_To->Name());
    }
  
  return false;
}


/**
 * Try to find a conversion to type m_To from the given type.  Returns
 * whether an identity is allowed.  If false is returned,
 * m_ConversionFunction will point to the conversion function if it
 * was found.  If a temporary array is needed, m_NeedArray will be set
 * to true.  The caller must then allocate the array and perform the
 * element-by-element conversions to fill it.
 */
bool
ArgumentAsPointerBase_array
::FindConversionFunction(const CvQualifiedType& from)
{
  // If the "from" type is a pointer and the conversion is a valid
  // cv-qualifier adjustment, we can use an identity.
  if(from.GetType()->IsEitherPointerType()
     && Conversions::IsValidQualificationConversion(PointerType::SafeDownCast(from.GetType()),
                                                    m_To))
    {
    return true;
    }
  else if(from.IsArrayType()
          && (ArrayType::SafeDownCast(from.GetType())->GetElementType() == m_ElementType))
    {
    // We must convert to an array of the given elements.
    m_NeedArray = true;
    return false;
    }
  else
    {
    // We don't have a trivial conversion.  Try to lookup the
    // conversion function.
    m_ConversionFunction = m_WrapperFacility->GetConversion(from, m_To);
    }

  if(!m_ConversionFunction)
    {
    // No conversion function was found, but one is needed.
    throw _cable_tcl_UnknownConversionException(from.GetName(), m_To->Name());
    }
  
  return false;
}


/**
 * Try to find a conversion to type m_To from the given type.  Returns
 * whether an identity is allowed.  If false is returned,
 * m_ConversionFunction will point to the conversion function if it
 * was found.
 */
bool
ArgumentAsReferenceBase
::FindConversionFunction(const CvQualifiedType& from)
{
  bool identity = this->FindDirectConversionFunction(from);

  if(!identity && !m_ConversionFunction)
    {
    // No conversion function was found, but one is needed.
    throw _cable_tcl_UnknownConversionException(from.GetName(), m_To->Name());
    }
    
  return identity;
}


/**
 * Internal method for ArgumentAsReferenceBase::FindConversionFunction
 * and ArgumentAsReferenceBase_const::FindConversionFunction.  It does
 * the real work for finding the conversion function, but will not
 * throw an exception if a conversion function can't be found.  This
 * is used so that
 * ArgumentAsReferenceBase_const::FindConversionFunction can see about
 * a conversion function that produces a temporary.
 */
bool
ArgumentAsReferenceBase
::FindDirectConversionFunction(const CvQualifiedType& in_from)
{
  CvQualifiedType from = in_from;
  
  // If the "from" type is a ReferenceType, dereference it.
  if(from.GetType()->IsReferenceType())
    {
    from = ReferenceType::SafeDownCast(from.GetType())->GetReferencedType();
    }
    
  // If the "to" type is a reference to the "from" type, we can use
  // an identity.
  if(Conversions::ReferenceCanBindAsIdentity(from, m_To))
    {
    return true;
    }
  else
    {
    // We don't have a trivial conversion.  Try to lookup the
    // conversion function.
    m_ConversionFunction = m_WrapperFacility->GetConversion(from, m_To);
    return false;
    }  
}


/**
 * Try to find a conversion to type m_To from the given type.  Returns
 * whether an identity is allowed.  If false is returned,
 * m_ConversionFunction will point to the conversion function if it
 * was found.  If a temporary is needed, m_NeedTemporary will be set
 * to true.  The caller is then responsible for allocating the
 * temporary and initializing it with the argument's value.
 */
bool
ArgumentAsReferenceBase_const
::FindConversionFunction(const CvQualifiedType& from)
{
  bool identity = this->FindDirectConversionFunction(from);
  if(identity) { return true; }
  
  if(!m_ConversionFunction)
    {
    // There is no direct conversion function.  Try to lookup one
    // that uses a temporary.
    m_ConversionFunction = m_WrapperFacility->GetConversion(from, m_TempType);
    m_NeedTemporary = true;
    }

  if(!m_ConversionFunction)
    {
    // No conversion function was found, but one is needed.
    throw _cable_tcl_UnknownConversionException(from.GetName(), m_To->Name());
    }
    
  return false;
}


} // namespace _cable_tcl_
