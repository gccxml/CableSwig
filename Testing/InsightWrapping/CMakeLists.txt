PROJECT(WrapITK)

SET(LIBRARY_OUTPUT_PATH ${WrapITK_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries.")
SET(EXECUTABLE_OUTPUT_PATH ${WrapITK_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")

# find and use ITK
FIND_PACKAGE(ITK)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build  without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)

# find python and tcl
INCLUDE (${CMAKE_ROOT}/Modules/FindTCL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindPythonLibs.cmake)

IF(CSWIG)
  GET_FILENAME_COMPONENT(CSWIG_PATH ${CSWIG} PATH)
ENDIF(CSWIG)

IF(GCCXML)
  GET_FILENAME_COMPONENT(GCCXML_PATH ${GCCXML} PATH)
ENDIF(GCCXML)

IF(CABLE_INDEX)
  GET_FILENAME_COMPONENT(CABLE_INDEX_PATH ${CABLE_INDEX} PATH)
ENDIF(CABLE_INDEX)

# find cswig, gccxml, and cableidx programs
FIND_PROGRAM(CSWIG
  NAMES 
  cswig
  PATHS
  ${CSWIG_PATH} ${GCCXML_PATH} ${CABLE_INDEX_PATH}
  ${CYGWIN_INSTALL_PATH}/bin
  /bin
  /usr/bin 
  /usr/local/bin
)

FIND_PROGRAM(GCCXML
  NAMES 
  gccxml
  PATHS
  ${CSWIG_PATH} ${GCCXML_PATH} ${CABLE_INDEX_PATH}
  ${CYGWIN_INSTALL_PATH}/bin
  /bin
  /usr/bin 
  /usr/local/bin
)

FIND_PROGRAM(CABLE_INDEX
  NAMES 
  cableidx
  PATHS
  ${CSWIG_PATH} ${GCCXML_PATH} ${CABLE_INDEX_PATH}
  ${CYGWIN_INSTALL_PATH}/bin
  /bin
  /usr/bin 
  /usr/local/bin
)

SET(IGNORE_WARNINGS -w362 -w389 -w503 -w508 -w509)
# define macros for wrapping commands
MACRO(GCCXML_CREATE_XML_FILE Source Bin Input Output Library)
   ADD_CUSTOM_COMMAND(
   SOURCE ${Source}/${Input}
   COMMAND ${GCCXML}
   ARGS -fxml-start=_cable_ -DCABLE_CONFIGURATION ${Source}/${Input} -fxml=${Bin}/${Output} ${SWIG_INC}
   TARGET ${Library}
   OUTPUTS ${Bin}/${Output} )
ENDMACRO(GCCXML_CREATE_XML_FILE)


MACRO(CINDEX_CREATE_IDX_FILE Bin Input Output Library)
   ADD_CUSTOM_COMMAND(
   SOURCE ${Bin}/${Input}
   COMMAND ${CABLE_INDEX}
   ARGS ${Bin}/${Input} ${Bin}/${Output}
   TARGET ${Library}
   OUTPUTS ${Bin}/${Output} )
ENDMACRO(CINDEX_CREATE_IDX_FILE)

MACRO(CSWIG_CREATE_TCL_CXX_FILE Bin MasterIdx InputIdx InputXml OutputTclCxx Library LibraryIndexFiles)
   SET(CINDEX)
   FOREACH(MIDX ${MasterIdx})
     SET(CINDEX ${CINDEX} -Cindex "${MIDX}")
   ENDFOREACH(MIDX)
   ADD_CUSTOM_COMMAND(
     SOURCE ${Bin}/${InputIdx}
     COMMAND ${CSWIG}
     ARGS -c ${CINDEX} ${IGNORE_WARNINGS} -o ${Bin}/${OutputTclCxx} -tcl -c++ ${Bin}/${InputXml}
     TARGET ${Library}
     OUTPUTS ${Bin}/${OutputTclCxx}
     DEPENDS ${LibraryIndexFiles}
     )
ENDMACRO(CSWIG_CREATE_TCL_CXX_FILE)

MACRO(CSWIG_CREATE_PYTHON_CXX_FILE Bin MasterIdx InputIdx InputXml OutputTclCxx Library LibraryIndexFiles)
   SET(CINDEX)
   FOREACH(MIDX ${MasterIdx})
     SET(CINDEX ${CINDEX} -Cindex "${MIDX}")
   ENDFOREACH(MIDX)
   ADD_CUSTOM_COMMAND(
     SOURCE ${Bin}/${InputIdx}
     COMMAND ${CSWIG} 
     ARGS -c ${CINDEX} ${IGNORE_WARNINGS} -o ${Bin}/${OutputTclCxx} -python -c++ ${Bin}/${InputXml}
     TARGET ${Library}
     OUTPUTS ${Bin}/${OutputTclCxx}
     DEPENDS ${LibraryIndexFiles}
     )
   GET_FILENAME_COMPONENT(OutputPythonFilePath ${Bin}/${OutputTclCxx} PATH)
   GET_FILENAME_COMPONENT(OutputPythonFileName ${Bin}/${OutputTclCxx} NAME_WE)
   STRING(REGEX REPLACE "_" "" LibName ${Library})
   STRING(REGEX REPLACE "wrap_" "${LibName}_" OutputPythonFileName ${OutputPythonFileName})
   STRING(REGEX REPLACE "Python" "" OutputPythonFileName ${OutputPythonFileName})
   SET(OutputPythonFile ${OutputPythonFilePath}/${OutputPythonFileName}.py)
   STRING(REGEX REPLACE ".*_" "" A ${OutputPythonFileName})
   IF("${OutputPythonFileName}" MATCHES "${A}.*_${A}")
     ADD_CUSTOM_COMMAND(
       SOURCE ${Library}
       COMMAND ${CMAKE_COMMAND} 
       ARGS -E copy_if_different  ${OutputPythonFilePath}/${LibName}.py "${EXECUTABLE_OUTPUT_PATH}/${LibName}.py"
       DEPENDS "${EXECUTABLE_OUTPUT_PATH}/${LibName}.py"
       TARGET ${Library}
       )
   ELSE("${OutputPythonFileName}" MATCHES "${A}.*_${A}")
     ADD_CUSTOM_COMMAND(
       SOURCE ${Library}
       COMMAND ${CMAKE_COMMAND} 
       ARGS -E copy_if_different ${OutputPythonFile} "${EXECUTABLE_OUTPUT_PATH}/${OutputPythonFileName}.py"
       DEPENDS "${EXECUTABLE_OUTPUT_PATH}/${OutputPythonFileName}.py"
       TARGET ${Library}
       )
   ENDIF("${OutputPythonFileName}" MATCHES "${A}.*_${A}")

ENDMACRO(CSWIG_CREATE_PYTHON_CXX_FILE)

# make sure required stuff is set
IF(NOT CSWIG)
   MESSAGE(SEND_ERROR "CSWIG Must be specified")
ENDIF(NOT CSWIG)
IF(NOT CABLE_INDEX)
   MESSAGE(SEND_ERROR "CABLE_INDEX Must be specified")
ENDIF(NOT CABLE_INDEX)
IF(NOT GCCXML)
   MESSAGE(SEND_ERROR "GCCXML Must be specified")
ENDIF(NOT GCCXML)



INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
FOREACH(INCDIR ${ITK_INCLUDE_DIRS})
   SET(SWIG_INC ${SWIG_INC} -I${INCDIR})
   IF( "${INCDIR}" MATCHES "Utilities/vxl$")
     IF(NOT ITK_TOP)
       SET(ITK_TOP ${INCDIR}/../.. )
       SET(SWIG_INC ${SWIG_INC} -I${ITK_TOP}/Wrapping/Tcl)
       SET(SWIG_INC ${SWIG_INC} -I${ITK_TOP}/Wrapping/Tcl/Common)
       SET(SWIG_INC ${SWIG_INC} -I${ITK_TOP}/Wrapping/Tcl/Numerics)
       SET(SWIG_INC ${SWIG_INC} -I${ITK_TOP}/Wrapping/Tcl/BasicFilters)
       SET(SWIG_INC ${SWIG_INC} -I${ITK_TOP}/Wrapping/Tcl/IO)
       SET(SWIG_INC ${SWIG_INC} -I${ITK_TOP}/Wrapping/Tcl/Algorithms)
     ENDIF(NOT ITK_TOP)
     SET(SWIG_INC ${SWIG_INC} -I${INCDIR}/vnl -I${INCDIR}/vcl/doc)
   ENDIF( "${INCDIR}" MATCHES "Utilities/vxl$")
ENDFOREACH(INCDIR)
ADD_DEFINITIONS(-DSWIG_GLOBAL)
SUBDIRS(SwigRuntime VXLNumerics Common Numerics BasicFilters IO Algorithms)
