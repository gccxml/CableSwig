PROJECT(wrapTemplate)
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
SET_SOURCE_FILES_PROPERTIES(simpleTemplateTcl.cxx GENERATED )
ADD_LIBRARY(WrapTemplate SHARED simpleTemplateTcl.cxx)
ADD_CUSTOM_COMMAND(
  SOURCE ${wrapTemplate_SOURCE_DIR}/simpleTemplate.h
  COMMAND ${CSWIG}
  ARGS -o ${wrapTemplate_BINARY_DIR}/simpleTemplateTcl.cxx
       -module wrapTemplate -tcl -c++ ${wrapTemplate_SOURCE_DIR}/simpleTemplate.h
  TARGET WrapTemplate
  OUTPUTS ${wrapTemplate_BINARY_DIR}/simpleTemplateTcl.cxx )
TARGET_LINK_LIBRARIES(WrapTemplate ${TCL_LIBRARY})
