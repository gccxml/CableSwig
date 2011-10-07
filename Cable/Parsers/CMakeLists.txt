INCLUDE_DIRECTORIES(${CABLE_SOURCE_DIR}/CxxTypes)
ADD_DEFINITIONS(-D_cxx_STATIC)

ADD_LIBRARY(CableParsers STATIC
  cableArrayType.cxx
  cableClass.cxx
  cableClassType.cxx
  cableConstructor.cxx
  cableContext.cxx
  cableConverter.cxx
  cableDestructor.cxx
  cableEnumeration.cxx
  cableEnumerationType.cxx
  cableField.cxx
  cableFunction.cxx
  cableFunctionType.cxx 
  cableFundamentalType.cxx
  cableIndent.cxx
  cableMethod.cxx
  cableMethodType.cxx
  cableNamed.cxx
  cableNamespace.cxx
  cableNamespaceAlias.cxx
  cableObject.cxx
  cableOffsetType.cxx
  cableOperatorFunction.cxx
  cableOperatorMethod.cxx
  cablePointerType.cxx
  cableReferenceType.cxx
  cableSourceObject.cxx
  cableSourceRepresentation.cxx
  cableStruct.cxx
  cableType.cxx
  cableTypedef.cxx
  cableUnion.cxx
  cableVariable.cxx
  cableXMLParser.cxx
  cableXMLSourceElement.cxx
  cableXMLSourceParser.cxx
)

TARGET_LINK_LIBRARIES(CableParsers CxxTypes CableExpat)

IF(NOT CABLE_NO_INSTALL_FACILITIES)
  INSTALL(TARGETS CableParsers
    ARCHIVE DESTINATION ${CableSwig_INSTALL_ROOT}lib
    COMPONENT Development)

  INSTALL(FILES
    cableSmartPointer.h
    cableUtils.h

    cableArrayType.h
    cableClass.h
    cableClassType.h
    cableConstructor.h
    cableContext.h
    cableConverter.h
    cableDestructor.h
    cableEnumeration.h
    cableEnumerationType.h
    cableField.h
    cableFunction.h
    cableFunctionType.h 
    cableFundamentalType.h
    cableIndent.h
    cableMethod.h
    cableMethodType.h
    cableNamed.h
    cableNamespace.h
    cableNamespaceAlias.h
    cableObject.h
    cableOffsetType.h
    cableOperatorFunction.h
    cableOperatorMethod.h
    cablePointerType.h
    cableReferenceType.h
    cableSourceObject.h
    cableSourceRepresentation.h
    cableStruct.h
    cableType.h
    cableTypedef.h
    cableUnion.h
    cableVariable.h
    cableXMLParser.h
    cableXMLSourceElement.h
    cableXMLSourceParser.h
    DESTINATION ${CableSwig_INSTALL_ROOT}include/Cable
    COMPONENT Development)
ENDIF(NOT CABLE_NO_INSTALL_FACILITIES)
