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

# find cswig, gccxml, and cableidx programs
FIND_PROGRAM(CSWIG
  NAMES 
  cswig
  PATH
  ${CYGWIN_INSTALL_PATH}/bin
  /bin
  /usr/bin 
  /usr/local/bin
)

FIND_PROGRAM(GCCXML
  NAMES 
  gccxml
  PATH
  ${CYGWIN_INSTALL_PATH}/bin
  /bin
  /usr/bin 
  /usr/local/bin
)

FIND_PROGRAM(CABLE_INDEX
  NAMES 
  cableidx
  PATH
  ${CSWIG}
  ${CYGWIN_INSTALL_PATH}/bin
  /bin
  /usr/bin 
  /usr/local/bin
)


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
   ADD_CUSTOM_COMMAND(
   SOURCE ${Bin}/${InputIdx}
   COMMAND ${CSWIG}
   ARGS -c -cindex ${Bin}/${MasterIdx} ${IGNORE_WARNINGS} -o ${Bin}/${OutputTclCxx} -tcl -c++ ${Bin}/${InputXml}
   TARGET ${Library}
   OUTPUTS ${Bin}/${OutputTclCxx}
   DEPENDS ${LibraryIndexFiles}
 )
ENDMACRO(CSWIG_CREATE_TCL_CXX_FILE)

MACRO(CSWIG_CREATE_PYTHON_CXX_FILE Bin MasterIdx InputIdx InputXml OutputTclCxx Library LibraryIndexFiles)
   ADD_CUSTOM_COMMAND(
   SOURCE ${Bin}/${InputIdx}
   COMMAND ${CSWIG}
   ARGS -c -cindex ${Bin}/${MasterIdx} ${IGNORE_WARNINGS} -o ${Bin}/${OutputTclCxx} -python -c++ ${Bin}/${InputXml}
   TARGET ${Library}
   OUTPUTS ${Bin}/${OutputTclCxx}
   DEPENDS ${LibraryIndexFiles}
 )
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
     ENDIF(NOT ITK_TOP)
     SET(SWIG_INC ${SWIG_INC} -I${INCDIR}/vnl -I${INCDIR}/vcl/doc)
   ENDIF( "${INCDIR}" MATCHES "Utilities/vxl$")
ENDFOREACH(INCDIR)
ADD_DEFINITIONS(-DSWIG_GLOBAL)
SUBDIRS(SwigRuntime Common)
