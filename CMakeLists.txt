PROJECT(CableSwig)

FIND_PATH(CABLE_BIN NAMES cableVersion.h  
             $ENV{HOME}/Cable-gcc/Generators
             $ENV{HOME}/Cable-build/Generators
             DOC "Full path to the cable bin")

FIND_PATH(CABLE_SOURCE CABLEConfig.cmake.in ${HOME}/Cable
          DOC "Path to root of cable source tree")

SET(SWIG_TOP ${CableSwig_SOURCE_DIR}/SWIG-1.3.17)

SET(LIBRARY_OUTPUT_PATH ${CableSwig_BINARY_DIR}/bin CACHE PATH 
                        "Single output directory for building all libraries.")
SET(EXECUTABLE_OUTPUT_PATH ${CableSwig_BINARY_DIR}/bin CACHE PATH 
                           "Single output directory for building all executables.")

INCLUDE_DIRECTORIES(
        ${SWIG_TOP}/Source/Modules1.1
	${SWIG_TOP}/Source/CParse
	${SWIG_TOP}/Source/Preprocessor
	${SWIG_TOP}/Source/Swig
	${SWIG_TOP}/Source/DOH
	${SWIG_TOP}/Source/Include
        ${SWIG_TOP}/Source/DOH/Include
        ${CableSwig_BINARY_DIR}/SWIG-1.3.17
        ${CABLE_BIN}
        ${CABLE_SOURCE}/CxxTypes
        ${CABLE_SOURCE}/Executables
        ${CABLE_SOURCE}/Expat
        ${CABLE_SOURCE}/Generators
        ${CABLE_SOURCE}/Parsers
        ${CABLE_SOURCE}
)


LINK_DIRECTORIES( 
        ${CableSwig_BINARY_DIR}/bin
        ${CABLE_BIN}/CxxTypes
        ${CABLE_BIN}/Executables
        ${CABLE_BIN}/Expat
        ${CABLE_BIN}/Generators
        ${CABLE_BIN}/Parsers
        )
SUBDIRS(SWIG-1.3.17 Executables)
