PROJECT(CableSwig)

FIND_LIBRARY(SWIG_TOP  swig.spec.in ${CableSwig_SOURCE_DIR}/SWIG-1.3.17 ${HOME}/SWIG-1.3.17
                       DOC "Path to the swig build/source tree" )

FIND_PATH(CABLE_BIN NAMES cableVersion.h  
             $ENV{HOME}/Cable-gcc/Generators
             $ENV{HOME}/Cable-build/Generators
             DOC "Full path to the cable bin")

FIND_PATH(CABLE_SOURCE CABLEConfig.cmake.in ${HOME}/Cable
          DOC "Path to root of cable source tree")

IF(NOT CABLE_SOURCE)
   MESSAGE(SEND_ERROR "CABLE_SOURCE must be set")
ENDIF(NOT CABLE_SOURCE)
IF(NOT SWIG_TOP)
   MESSAGE(SEND_ERROR "SWIG_TOP must be set")
ENDIF(NOT SWIG_TOP)
IF(NOT CABLE_BIN)
   MESSAGE(SEND_ERROR "CABLE_BIN must be set")
ENDIF(NOT CABLE_BIN)

INCLUDE_DIRECTORIES(
        ${SWIG_TOP}/Source/Modules1.1
	${SWIG_TOP}/Source/CParse
	${SWIG_TOP}/Source/Preprocessor
	${SWIG_TOP}/Source/Swig
	${SWIG_TOP}/Source/DOH
	${SWIG_TOP}/Source/Include
        ${SWIG_TOP}/Source/DOH/Include
        ${CABLE_BIN}
        ${CABLE_SOURCE}/CxxTypes
        ${CABLE_SOURCE}/Executables
        ${CABLE_SOURCE}/Expat
        ${CABLE_SOURCE}/Generators
        ${CABLE_SOURCE}/Parsers
        ${CABLE_SOURCE}
)


LINK_DIRECTORIES( 
        ${SWIG_TOP}/Source/Modules1.1
	${SWIG_TOP}/Source/CParse
	${SWIG_TOP}/Source/Preprocessor
	${SWIG_TOP}/Source/Swig
	${SWIG_TOP}/Source/DOH
        ${CABLE_BIN}/CxxTypes
        ${CABLE_BIN}/Executables
        ${CABLE_BIN}/Expat
        ${CABLE_BIN}/Generators
        ${CABLE_BIN}/Parsers
        )
LINK_LIBRARIES(
        modules11 
        cparse
        cpp
        swig
        doh
        CableGenerators
        CableParsers
        CableExpat 
        CxxTypes 
        )
ADD_EXECUTABLE( cswig CableSwig.cxx cableSwigMain.cxx)
