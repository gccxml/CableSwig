MACRO(CSWIG_WRAP_FILE Source Bin Input Output Library)
   ADD_CUSTOM_COMMAND(
   SOURCE ${Source}/${Input}
   COMMAND ${CSWIG}
   ARGS -o ${Bin}/${Output}
          -tcl -c++ ${Source}/${Input}
  TARGET ${Library}
  OUTPUTS ${Bin}/${Output} )
ENDMACRO(CSWIG_WRAP_FILE)
INCLUDE (${CMAKE_ROOT}/Modules/FindTCL.cmake)
# Find Python
INCLUDE (${CMAKE_ROOT}/Modules/FindPythonLibs.cmake)

FIND_PROGRAM(CSWIG
  NAMES 
  cswig
  PATH
  ${CYGWIN_INSTALL_PATH}/bin
  /bin
  /usr/bin 
  /usr/local/bin
)
IF(NOT CSWIG)
  MESSAGE(SEND_ERROR "CSWIG Must be specified")
ENDIF(NOT CSWIG)

SUBDIRS(SimpleTemplate String Module itkTest)
