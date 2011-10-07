INCLUDE_DIRECTORIES(${CABLE_SOURCE_DIR}/CxxTypes
                    ${CABLE_SOURCE_DIR}/Parsers)
ADD_DEFINITIONS(-D_cxx_STATIC)

ADD_LIBRARY(CableGenerators STATIC
  cableCvTypeWriter.cxx
  cableGenerator.cxx
  cableTclGenerator.cxx
  cableSystemTools.cxx
)

TARGET_LINK_LIBRARIES(CableGenerators CableParsers CxxTypes CableExpat)

IF(NOT CABLE_NO_INSTALL_FACILITIES)
  INSTALL(TARGETS CableGenerators
    ARCHIVE DESTINATION ${CableSwig_INSTALL_ROOT}lib
    COMPONENT Development)

  INSTALL(FILES
    cableCvTypeWriter.h
    cableGenerator.h
    cableTclGenerator.h
    cableSystemTools.h
    DESTINATION ${CableSwig_INSTALL_ROOT}include/Cable
    COMPONENT Development)
ENDIF(NOT CABLE_NO_INSTALL_FACILITIES)
