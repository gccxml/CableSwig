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


MACRO(CSWIG_WRAP_FILE Source Bin Input Output Library)
   ADD_CUSTOM_COMMAND(
   SOURCE ${Source}/${Input}
   COMMAND ${CSWIG}
   ARGS -o ${Bin}/${Output}
          -tcl -c++ ${Source}/${Input}
  TARGET ${Library}
  OUTPUTS ${Bin}/${Output} )
ENDMACRO(CSWIG_WRAP_FILE)

CSWIG_WRAP_FILE(
        ${wrapTemplate_SOURCE_DIR}
        ${wrapTemplate_BINARY_DIR}
        simpleTemplate.h
        simpleTemplateTcl.cxx
        WrapTemplate)

TARGET_LINK_LIBRARIES(WrapTemplate ${TCL_LIBRARY})
