INCLUDE_DIRECTORIES(${CABLE_SOURCE_DIR}/CxxTypes
                    ${CABLE_SOURCE_DIR}/Parsers)
ADD_DEFINITIONS(-D_cxx_STATIC)

ADD_LIBRARY(CableGenerators STATIC
  cableCvTypeWriter.cxx
  cableGenerator.cxx
  cableTclGenerator.cxx
)

TARGET_LINK_LIBRARIES(CableGenerators CableParsers CxxTypes CableExpat)
