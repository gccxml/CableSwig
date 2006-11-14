/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    cableCvTypeWriter.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cableCvTypeWriter.h"
#include "cxxTypes.h"
#include "cxxCvQualifiedType.h"

#include <vector>
#include <set>

namespace cable
{

typedef std::vector<cxx::CvQualifiedType> TypeOrderingBase;
class CvTypeWriter::TypeOrdering: public TypeOrderingBase
{
public:
  typedef TypeOrderingBase::iterator iterator;
  typedef TypeOrderingBase::const_iterator const_iterator;
  typedef TypeOrderingBase::value_type value_type;
  static TypeOrdering* New() { return new TypeOrdering; }
  void Delete() { delete this; }
};
  
//----------------------------------------------------------------------------
typedef std::set<cxx::CvQualifiedType> TypesAddedBase;
class CvTypeWriter::TypesAdded: public TypesAddedBase
{
public:
  typedef TypesAddedBase::value_type value_type;
  typedef TypesAddedBase::iterator iterator;
  typedef TypesAddedBase::const_iterator const_iterator;  
  static TypesAdded* New() { return new TypesAdded; }
  void Delete() { delete this; }
};

//----------------------------------------------------------------------------
CvTypeWriter::CvTypeWriter():
  m_TypeOrdering(*TypeOrdering::New()),
  m_TypesAdded(*TypesAdded::New())
{
}

//----------------------------------------------------------------------------
CvTypeWriter::~CvTypeWriter()
{
  m_TypeOrdering.Delete();
  m_TypesAdded.Delete();
}

//----------------------------------------------------------------------------
void CvTypeWriter::WriteClasses(std::ostream& os) const
{
  for(TypeOrdering::const_iterator t = m_TypeOrdering.begin();
      t != m_TypeOrdering.end(); ++t)
    {
    os << "template <> struct CvType< "
       << t->GetName().c_str()
       << " >   { static CvQualifiedType type; typedef "
       << t->GetType()->GenerateDeclaration("NoCv").c_str() << "; ";
    this->WriteArgumentAs(os, *t);
    os << "};\n";
    }
}

//----------------------------------------------------------------------------
void CvTypeWriter::WriteDataDeclarations(std::ostream& os) const
{
  for(TypeOrdering::const_iterator t = m_TypeOrdering.begin();
      t != m_TypeOrdering.end(); ++t)
    {
    os << "CvQualifiedType CvType< " << t->GetName().c_str() << " >::type;\n";
    }
}

//----------------------------------------------------------------------------
void CvTypeWriter::WriteInitializations(std::ostream& os) const
{
  for(TypeOrdering::const_iterator t = m_TypeOrdering.begin();
      t != m_TypeOrdering.end(); ++t)
    {
    this->WriteInitialization(os, *t);
    }
}

//----------------------------------------------------------------------------
void CvTypeWriter::Add(const cxx::CvQualifiedType& cvType)
{
  // If the type has already been added to the list, we are done.
  if(m_TypesAdded.find(cvType) != m_TypesAdded.end())
    { return; }
  
#ifdef _MSC_VER
  // MSVC cannot distinguish among cv-qualified versions of void.
  if((cvType.IsConst() || cvType.IsVolatile()) &&
     cvType.GetType()->IsFundamentalType() &&
     cxx::FundamentalType::SafeDownCast(cvType.GetType())->IsVoid())
    {
    this->Add(cvType.GetType()->GetCvQualifiedType(false, false));
    return;
    } 
#endif
  
  const cxx::Type* type = cvType.GetType();
  
  // Make sure the cv-unqualified version comes first.
  if(cvType.IsConst() || cvType.IsVolatile())
    {
    this->Add(type->GetCvQualifiedType(false, false));
    }
  
  // Recursively add any "inner" types.
  switch (type->GetRepresentationType())
    {
    case cxx::ArrayType_id:
      this->Add(cxx::ArrayType::SafeDownCast(type)->GetElementType());
      break;
    case cxx::ClassType_id:
      // Make sure any superclasses come first.  Only need this for
      // cv-unqualified version.
      if(!cvType.IsConst() && !cvType.IsVolatile())
        {
        const cxx::ClassType* classType = cxx::ClassType::SafeDownCast(type);
        for(cxx::ClassTypes::const_iterator p = classType->ParentsBegin();
            p != classType->ParentsEnd(); ++p)
          {
          this->Add((*p)->GetCvQualifiedType(false, false));
          }
        }
      break;
    case cxx::EnumerationType_id:
      // An EnumerationType has no "inner" types.  Terminate recursion.
      break;
    case cxx::FunctionType_id:
      this->AddFunctionTypes(cxx::FunctionType::SafeDownCast(type));
      break;
    case cxx::FundamentalType_id:
      // A FundamentalType has no "inner" types.  Terminate recursion.
      break;
    case cxx::PointerType_id:
      this->Add(cxx::PointerType::SafeDownCast(type)->GetPointedToType());
      break;
    case cxx::PointerToMemberType_id:
      {
      const cxx::PointerToMemberType* t =
        cxx::PointerToMemberType::SafeDownCast(type);
      this->Add(t->GetPointedToType());
      this->Add(t->GetClassType()->GetCvQualifiedType(false, false));
      }; break;
    case cxx::ReferenceType_id:
      this->Add(cxx::ReferenceType::SafeDownCast(type)->GetReferencedType());
      break;
    case cxx::Undefined_id:
    default: break;
    }
  
  // All "inner" types have been added.  Add this type.
  m_TypesAdded.insert(cvType);
  
  m_TypeOrdering.push_back(cvType);
}

//----------------------------------------------------------------------------
void CvTypeWriter::AddFunctionTypes(const cxx::FunctionType* functionType)
{
  // Add the function's return type.
  this->Add(functionType->GetReturnType());
  
  // Add the function's argument types.
  for(cxx::CvQualifiedTypes::const_iterator arg =
        functionType->GetArgumentTypes().begin();
      arg != functionType->GetArgumentTypes().end(); ++arg)
    {
    this->Add(*arg);
    }
}

//----------------------------------------------------------------------------
void
CvTypeWriter::WriteInitialization(std::ostream& os,
                                  const cxx::CvQualifiedType& cvType) const
{
  const cxx::Type* type = cvType.GetType();
  switch (type->GetRepresentationType())
    {
    case cxx::ArrayType_id:
      {
      const cxx::ArrayType* arrayType = cxx::ArrayType::SafeDownCast(type);
      os << "  CvType< " << cvType.GetName().c_str()
         << " >::type = TypeInfo::GetArrayType(CvType< "
         << arrayType->GetElementType().GetName().c_str()
         << " >::type, " << arrayType->GetLength() << ");\n";
      }; break;
    case cxx::ClassType_id:
      {
      const cxx::ClassType* classType = cxx::ClassType::SafeDownCast(type);
      // Write superclass information on cv-unqualified version.
      if(!cvType.IsConst() && !cvType.IsVolatile()
         && (classType->ParentsBegin() != classType->ParentsEnd()))
        {
        os << "  {\n"
           << "  ClassTypes parents;\n";
        for(cxx::ClassTypes::const_iterator p = classType->ParentsBegin();
            p != classType->ParentsEnd(); ++p)
          {
          os << "  parents.push_back(ClassType::SafeDownCast(CvType< " << (*p)->GetName().c_str()
             << " >::type.GetType()));\n";
          }
        os << "  CvType< " << cvType.GetName().c_str()
           << " >::type = TypeInfo::GetClassType(\""
           << classType->GetName().c_str()
           << "\", " << (cvType.IsConst()? "true":"false")
           << ", " << (cvType.IsVolatile()? "true":"false")
           << ", " << (classType->IsCopyable()? "true":"false")
           << ", parents);\n";
        os << "  }\n";
        }
      else
        {
        os << "  CvType< " << cvType.GetName().c_str()
           << " >::type = TypeInfo::GetClassType(\""
           << classType->GetName().c_str()
           << "\", " << (cvType.IsConst()? "true":"false")
           << ", " << (cvType.IsVolatile()? "true":"false")
           << ", " << (classType->IsCopyable()? "true":"false") << ");\n";
        }
      }; break;
    case cxx::EnumerationType_id:
      os << "  CvType< " << cvType.GetName().c_str()
         << " >::type = TypeInfo::GetEnumerationType(\""
         << cxx::EnumerationType::SafeDownCast(type)->GetName().c_str()
         << "\", " << (cvType.IsConst()? "true":"false")
         << ", " << (cvType.IsVolatile()? "true":"false") << ");\n";
      break;
    case cxx::FundamentalType_id:
      os << "  CvType< " << cvType.GetName().c_str()
         << " >::type = TypeInfo::GetFundamentalType(FundamentalType::";
      switch (cxx::FundamentalType::SafeDownCast(type)->GetId())
        {
        case cxx::FundamentalType::UnsignedChar:
          os << "UnsignedChar"; break;
        case cxx::FundamentalType::UnsignedShortInt:
          os << "UnsignedShortInt"; break;
        case cxx::FundamentalType::UnsignedInt:
          os << "UnsignedInt"; break;
        case cxx::FundamentalType::UnsignedLongInt:
          os << "UnsignedLongInt"; break;
        case cxx::FundamentalType::UnsignedLongLongInt:
          os << "UnsignedLongLongInt"; break;
        case cxx::FundamentalType::SignedChar:
          os << "SignedChar"; break;
        case cxx::FundamentalType::Char:
          os << "Char"; break;
        case cxx::FundamentalType::ShortInt:
          os << "ShortInt"; break;
        case cxx::FundamentalType::Int:
          os << "Int"; break;
        case cxx::FundamentalType::LongInt:
          os << "LongInt"; break;
        case cxx::FundamentalType::LongLongInt:
          os << "LongLongInt"; break;
        case cxx::FundamentalType::WChar_t:
          os << "WChar_t"; break;
        case cxx::FundamentalType::Bool:
          os << "Bool"; break;
        case cxx::FundamentalType::Float:
          os << "Float"; break;
        case cxx::FundamentalType::Double:
          os << "Double"; break;
        case cxx::FundamentalType::LongDouble:
          os << "LongDouble"; break;
        case cxx::FundamentalType::ComplexFloat:
          os << "ComplexFloat"; break;
        case cxx::FundamentalType::ComplexDouble:
          os << "ComplexDouble"; break;
        case cxx::FundamentalType::ComplexLongDouble:
          os << "ComplexLongDouble"; break;
        case cxx::FundamentalType::Void:
          os << "Void"; break;
        case cxx::FundamentalType::NumberOfTypes:
        default:
          os << "ERROR: Incorrect cxx::FundamentalType value..."; break;
        }
      os << ", " << (cvType.IsConst()? "true":"false")
         << ", " << (cvType.IsVolatile()? "true":"false") << ");\n";
      break;
    case cxx::FunctionType_id:
      {
      const cxx::FunctionType* t = cxx::FunctionType::SafeDownCast(type);
      os << "  {\n"
         << "  CvQualifiedTypes argumentTypes;\n";
      for(cxx::CvQualifiedTypes::const_iterator arg = t->GetArgumentTypes().begin();
          arg != t->GetArgumentTypes().end(); ++arg)
        {
        os << "  argumentTypes.push_back(CvType< "
           << arg->GetName().c_str()
           << " >::type);\n";
        }      
      os << "  CvType< " << cvType.GetName().c_str()
         << " >::type = TypeInfo::GetFunctionType(CvType< "
         << t->GetReturnType().GetName().c_str()
         << " >::type, argumentTypes, " << (cvType.IsConst()? "true":"false")
         << ", " << (cvType.IsVolatile()? "true":"false") << ");\n"
         << "  }\n";
      }; break;
    case cxx::PointerType_id:
      os << "  CvType< " << cvType.GetName().c_str()
         << " >::type = TypeInfo::GetPointerType(CvType< "
         << cxx::PointerType::SafeDownCast(type)->GetPointedToType().GetName().c_str()
         << " >::type, " << (cvType.IsConst()? "true":"false")
         << ", " << (cvType.IsVolatile()? "true":"false") << ");\n";      
      break;
    case cxx::PointerToMemberType_id:
      {
      const cxx::PointerToMemberType* t = cxx::PointerToMemberType::SafeDownCast(type);      
      os << "  CvType< " << cvType.GetName().c_str()
         << " >::type = TypeInfo::GetPointerToMemberType(CvType< "
         << t->GetPointedToType().GetName().c_str()
         << " >::type, "
         << "ClassType::SafeDownCast(CvType< " << t->GetClassType()->Name().c_str() << " >::type.GetType())"
         << ", " << (cvType.IsConst()? "true":"false")
         << ", " << (cvType.IsVolatile()? "true":"false") << ");\n";      
      }; break;
    case cxx::ReferenceType_id:
      os << "  CvType< " << cvType.GetName().c_str()
         << " >::type = TypeInfo::GetReferenceType(CvType< "
         << cxx::ReferenceType::SafeDownCast(type)->GetReferencedType().GetName().c_str()
         << " >::type);\n";
      break;
    case cxx::Undefined_id:
    default: break;
    }
}

//----------------------------------------------------------------------------
/**
 * Called internally by WriteClasses().
 *
 * Writes the
 *   typedef ArgumentAs...<T> ArgumentFor;
 * functor type to be used when passing an argument of the given type
 * to a wrapped function.
 */
void CvTypeWriter::WriteArgumentAs(std::ostream& os,
                                   const cxx::CvQualifiedType& cvType) const
{
  const cxx::Type* type = cvType.GetType();
  switch (type->GetRepresentationType())
    {
    case cxx::ArrayType_id:
      {
      os << "typedef ArgumentAsPointerTo_array< "
         << cxx::ArrayType::SafeDownCast(type)->GetElementType().GetName().c_str()
         << " > ArgumentFor; ";
      }; break;
    case cxx::ClassType_id:
    case cxx::EnumerationType_id:
    case cxx::FundamentalType_id:
      {
      os << "typedef ArgumentAsInstanceOf< "
         << cvType.GetName().c_str()
         << " > ArgumentFor; ";
      }; break;
    case cxx::PointerType_id:
      {
      cxx::CvQualifiedType target =
        cxx::PointerType::SafeDownCast(type)->GetPointedToType();
      if(target.IsFunctionType())
        {
        os << "typedef ArgumentAsPointerToFunction< "
           << target.GetName().c_str()
           << " > ArgumentFor; ";
        }
      else if(target.IsPointerType()
              || target.IsEnumerationType()
              || (target.IsFundamentalType()
                  && !cxx::FundamentalType::SafeDownCast(target.GetType())->IsVoid()))
        {
        os << "typedef ArgumentAsPointerTo_array< "
           << target.GetName().c_str()
           << " > ArgumentFor; ";
        }
      else
        {
        os << "typedef ArgumentAsPointerTo< "
           << target.GetName().c_str()
           << " > ArgumentFor; ";
        }
      }; break;
    case cxx::PointerToMemberType_id:
      {
      // TODO: Implement.
      }; break;
    case cxx::ReferenceType_id:
      {
      cxx::CvQualifiedType target =
        cxx::ReferenceType::SafeDownCast(type)->GetReferencedType();
      if(target.IsConst() && !target.IsVolatile()
         && (!target.IsClassType()
             || cxx::ClassType::SafeDownCast(target.GetType())->IsCopyable()))
        {
        os << "typedef ArgumentAsReferenceTo_const< "
           << target.GetType()->Name().c_str()
           << " > ArgumentFor; ";
        }
      else
        {
        os << "typedef ArgumentAsReferenceTo< "
           << target.GetName().c_str()
           << " > ArgumentFor; ";
        }
      }; break;
    case cxx::FunctionType_id:
    case cxx::Undefined_id:
    default: break;
    }
}


} // namespace cable
