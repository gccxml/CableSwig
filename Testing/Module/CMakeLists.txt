PROJECT(TestModule)
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
INCLUDE_DIRECTORIES(../../..)

SET_SOURCE_FILES_PROPERTIES(fooTcl.cxx barTcl.cxx moduleTcl.cxx GENERATED )
ADD_LIBRARY(TestModule SHARED moduleTcl.cxx fooTcl.cxx barTcl.cxx )

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
        ${TestModule_SOURCE_DIR}
        ${TestModule_BINARY_DIR}
        foo.h
        fooTcl.cxx
        TestModule)

CSWIG_WRAP_FILE(
        ${TestModule_SOURCE_DIR}
        ${TestModule_BINARY_DIR}
        bar.h
        barTcl.cxx
        TestModule)

CSWIG_WRAP_FILE(
        ${TestModule_SOURCE_DIR}
        ${TestModule_BINARY_DIR}
        module.h
        moduleTcl.cxx
        TestModule)

TARGET_LINK_LIBRARIES(TestModule ${TCL_LIBRARY})
