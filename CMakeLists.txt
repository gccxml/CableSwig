PROJECT(CableSwig)

SET(LIBRARY_OUTPUT_PATH ${CableSwig_BINARY_DIR}/bin CACHE PATH 
                        "Single output directory for building all libraries.")
SET(EXECUTABLE_OUTPUT_PATH ${CableSwig_BINARY_DIR}/bin CACHE PATH 
                           "Single output directory for building all executables.")

SET(SWIG_TOP ${CableSwig_SOURCE_DIR}/SWIG)
SET(CABLE_SOURCE ${CableSwig_SOURCE_DIR}/Cable)
SET(CABLE_BIN ${CableSwig_BINARY_DIR}/Cable)


INCLUDE_DIRECTORIES(
        ${SWIG_TOP}/Source/Modules1.1
	${SWIG_TOP}/Source/CParse
	${SWIG_TOP}/Source/Preprocessor
	${SWIG_TOP}/Source/Swig
	${SWIG_TOP}/Source/DOH
	${SWIG_TOP}/Source/Include
        ${SWIG_TOP}/Source/DOH/Include
        ${CableSwig_BINARY_DIR}/SWIG
        ${CABLE_BIN}
        ${CABLE_SOURCE}/CxxTypes
        ${CABLE_SOURCE}/Executables
        ${CABLE_SOURCE}/Expat
        ${CABLE_SOURCE}/Generators
        ${CABLE_SOURCE}/Parsers
        ${CABLE_SOURCE}
)


LINK_DIRECTORIES( ${CableSwig_BINARY_DIR}/bin  )
SUBDIRS(SWIG Cable GCCPatch GCC_XML Executables)
