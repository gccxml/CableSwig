PROJECT(SWIG)

# there is a bug with vs 7, if you optimize the
# c file building cswig does not run correctly.
# so add the -Od flag which will turn off
IF(CMAKE_SYSTEM_AND_CXX_COMPILER_INFO_FILE MATCHES "Windows-cl")
  SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Od")
  SET(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -Od")
  IF(CMAKE_MAKE_PROGRAM MATCHES devenv)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Od")
  ENDIF(CMAKE_MAKE_PROGRAM MATCHES devenv)
ENDIF(CMAKE_SYSTEM_AND_CXX_COMPILER_INFO_FILE MATCHES "Windows-cl")


SET(-swig_lib- ${SWIG_SOURCE_DIR}/Lib)
SET(SWIG_VERSION 1.3.17)
SET(SWIG_MAJOR_VERSION 3)
SET(SWIG_MINOR_VERSION 17)
SET(SWIG_SPIN 0)

CONFIGURE_FILE(
${SWIG_SOURCE_DIR}/Source/Include/swigver.h.in
${SWIG_BINARY_DIR}/swigver.h)

CONFIGURE_FILE(
${SWIG_SOURCE_DIR}/Source/Include/swigconfig.h.in
${SWIG_BINARY_DIR}/swigconfig.h)

# find python and tcl
INCLUDE (${CMAKE_ROOT}/Modules/FindTCL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindPythonLibs.cmake)

#INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
#INCLUDE_DIRECTORIES(${TCL_INCLUDE_PATH})


SET(LIBRARY_OUTPUT_PATH ${SWIG_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries.")
SET(EXECUTABLE_OUTPUT_PATH ${SWIG_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")
INCLUDE_DIRECTORIES(
${SWIG_BINARY_DIR}
${SWIG_SOURCE_DIR}/Source/Include
${SWIG_SOURCE_DIR}/Source/DOH/Include
${SWIG_SOURCE_DIR}/Source/Preprocessor
${SWIG_SOURCE_DIR}/Source/Swig
)


SUBDIRS(Source/Modules1.1
Source/CParse
Source/Preprocessor
Source/Swig
Source/DOH
)

# Install support library.
SET(SWIG_LIB_INSTALL
  ${CMAKE_INSTALL_PREFIX}${CableSwig_INSTALL_ROOT}/lib/CableSwig/SWIGLib)
SUBDIRS(Lib)
