INCLUDE_DIRECTORIES (${CABLE_SOURCE_DIR}/CxxTypes
                     ${CABLE_SOURCE_DIR}/Parsers
                     ${CABLE_SOURCE_DIR}/Generators)

LINK_DIRECTORIES (${CABLE_BINARY_DIR}/CxxTypes
                  ${CABLE_BINARY_DIR}/Expat
                  ${CABLE_BINARY_DIR}/Parsers
                  ${CABLE_BINARY_DIR}/Generators)

ADD_DEFINITIONS(-D_cxx_STATIC)

ADD_EXECUTABLE(cable cable.cxx cableSystemTools.cxx)

TARGET_LINK_LIBRARIES(cable CableGenerators CableParsers CxxTypes CableExpat)

IF (NOT CABLE_NO_INSTALL)
  INSTALL_TARGETS(/bin cable)
ENDIF (NOT CABLE_NO_INSTALL)
