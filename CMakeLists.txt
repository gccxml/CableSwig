PROJECT(CableSwig)

IF(NOT LIBRARY_OUTPUT_PATH)
  SET(LIBRARY_OUTPUT_PATH ${CableSwig_BINARY_DIR}/bin CACHE INTERNAL 
    "Single output directory for building all libraries.  Can not be changed.")
ENDIF(NOT LIBRARY_OUTPUT_PATH)

IF(NOT EXECUTABLE_OUTPUT_PATH)
  SET(EXECUTABLE_OUTPUT_PATH ${CableSwig_BINARY_DIR}/bin CACHE INTERNAL
    "Single output directory for building all executables.  Can not be changed.")
ENDIF(NOT EXECUTABLE_OUTPUT_PATH)

SET(SWIG_TOP ${CableSwig_SOURCE_DIR}/SWIG)
SET(CABLE_SOURCE ${CableSwig_SOURCE_DIR}/Cable)
SET(CABLE_BIN ${CableSwig_BINARY_DIR}/Cable)

SET(BUILD_SHARED_LIBS OFF)
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

# Tell GCC-XML that we are building gccxml_cc1plus.
SET(GCCXML_NATIVE_CC1PLUS 1)

# Tell CABLE to not install CxxTypes headers.
SET(CABLE_NO_INSTALL_FACILITIES 1)

# Tell GCC-XML where to install.
SET(GCCXML_INSTALL_ROOT ${CableSwig_INSTALL_ROOT}/lib/CableSwig)

SUBDIRS(SWIG Cable GCCPatch GCC_XML Executables)
