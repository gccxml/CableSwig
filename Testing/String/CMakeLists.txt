PROJECT(wrapString)
INCLUDE (${CMAKE_ROOT}/Modules/FindTCL.cmake)
FIND_PROGRAM(CSWIG
  NAMES 
  cswig
  PATH
  ${CYGWIN_INSTALL_PATH}/bin
  /bin
  /usr/bin 
  /usr/local/bin
)
SET_SOURCE_FILES_PROPERTIES(stringConfigTcl.cxx GENERATED )
ADD_LIBRARY(WrapString SHARED stringConfigTcl.cxx)
ADD_CUSTOM_COMMAND(
  SOURCE ${wrapString_SOURCE_DIR}/stringConfig.cxx
  COMMAND ${CSWIG}
  ARGS -o ${wrapString_BINARY_DIR}/stringConfigTcl.cxx
       -module wrapString -tcl -c++ ${wrapString_SOURCE_DIR}/stringConfig.cxx
  TARGET WrapString
  OUTPUTS ${wrapString_BINARY_DIR}/stringConfigTcl.cxx )
TARGET_LINK_LIBRARIES(WrapString ${TCL_LIBRARY})