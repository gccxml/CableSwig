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

INSTALL_TARGETS(/bin cable)

IF(UNIX)
  IF(NOT CABLE_CMAKE_HAS_EXPLICIT_INSTALL_FILES)
    # Hack for installation with CMake 1.4.
    INSTALL_FILES(/lib/Cable ".cmake" "UseCABLE")
  ENDIF(NOT CABLE_CMAKE_HAS_EXPLICIT_INSTALL_FILES)
ENDIF(UNIX)
