PROJECT(wrapString)

SET_SOURCE_FILES_PROPERTIES(stringConfigTcl.cxx GENERATED )

ADD_LIBRARY(WrapString SHARED stringConfigTcl.cxx)

ADD_CUSTOM_COMMAND(
  SOURCE ${wrapString_SOURCE_DIR}/stringConfig.cxx
  COMMAND ${CSWIG}
  ARGS -o ${wrapString_BINARY_DIR}/stringConfigTcl.cxx -dump_tree
       -module wrapString -tcl -c++ ${wrapString_SOURCE_DIR}/stringConfig.cxx
  TARGET WrapString
  OUTPUTS ${wrapString_BINARY_DIR}/stringConfigTcl.cxx )

TARGET_LINK_LIBRARIES(WrapString ${TCL_LIBRARY})