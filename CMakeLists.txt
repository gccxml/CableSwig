PROJECT(CableSwig)

SET(CableSwig_VERSION_MAJOR 0)
SET(CableSwig_VERSION_MINOR 1)
SET(CableSwig_VERSION_PATCH 0)

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

SUBDIRS(SWIG Cable GCC GCC_XML Executables)

#-----------------------------------------------------------------------------
# Create the CableSwigConfig.cmake files for external projects to use.

# Settings for the build tree.
UTILITY_SOURCE(CableSwig_cableidx_EXE cableidx Executables cableidx.cxx)
UTILITY_SOURCE(CableSwig_cswig_EXE cswig Executables CableSwig.cxx)
UTILITY_SOURCE(CableSwig_gccxml_EXE gccxml GCC_XML/GXFront gxFront.cxx)
MARK_AS_ADVANCED(CableSwig_cableidx_EXE CableSwig_cswig_EXE CableSwig_gccxml_EXE)
SET(CableSwig_cableidx_EXE_CONFIG ${CableSwig_cableidx_EXE})
SET(CableSwig_cswig_EXE_CONFIG ${CableSwig_cswig_EXE})
SET(CableSwig_gccxml_EXE_CONFIG ${CableSwig_gccxml_EXE})

CONFIGURE_FILE(${CableSwig_SOURCE_DIR}/CableSwigConfig.cmake.in
               ${CableSwig_BINARY_DIR}/CableSwigConfig.cmake @ONLY IMMEDIATE)

# Settings for the install tree.
SET(CableSwig_cableidx_EXE_CONFIG ${CMAKE_INSTALL_PREFIX}${CableSwig_INSTALL_ROOT}/bin/cableidx)
SET(CableSwig_cswig_EXE_CONFIG ${CMAKE_INSTALL_PREFIX}${CableSwig_INSTALL_ROOT}/bin/cswig)
SET(CableSwig_gccxml_EXE_CONFIG ${CMAKE_INSTALL_PREFIX}${GCCXML_INSTALL_ROOT}/bin/gccxml)

CONFIGURE_FILE(${CableSwig_SOURCE_DIR}/CableSwigConfig.cmake.in
               ${CableSwig_BINARY_DIR}/InstallOnly/CableSwigConfig.cmake
               @ONLY IMMEDIATE)

INSTALL_FILES(${CableSwig_INSTALL_ROOT}/lib/CableSwig FILES
              ${CableSwig_BINARY_DIR}/InstallOnly/CableSwigConfig.cmake)
