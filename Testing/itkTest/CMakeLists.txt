PROJECT(WrapITK)
FIND_PACKAGE(ITK)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build  without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

# add the correct include directories for swig command
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

# create a list of cable config files for wrapping
SET(WRAP_SOURCES
  wrap_ITKBase
  wrap_ITKCommon
  wrap_ITKFilterBase
  wrap_ITKInterpolators
  wrap_ITKRegions
  wrap_ITKTransforms
  wrap_itkArray
  wrap_itkContinuousIndex
  wrap_itkDenseFiniteDifferenceImageFilter
  wrap_itkDifferenceImageFilter
  wrap_itkEventObject
  wrap_itkFiniteDifferenceImageFilter
  wrap_itkFixedArray
  wrap_itkImage
  wrap_itkIndex
  wrap_itkPoint
  wrap_itkSize
  wrap_itkVector
)

SET(INDEX_FILE_CONTENT "")
WRITE_FILE(${WrapITK_BINARY_DIR}/ITKCommonTCL.mdx.tmp "")

# loop over cable config files creating two lists:
# WRAP_TCL_SOURCES: list of generated files
FOREACH(Source ${WRAP_SOURCES})
   SET(WRAP_TCL_SOURCES ${WRAP_TCL_SOURCES} ${Source}Tcl.cxx)
   SET(WRAP_PYTHON_SOURCES ${WRAP_PYTHON_SOURCES} ${Source}Python.cxx)
   SET(ALL_IDX_FILES ${ALL_IDX_FILES} ${WrapITK_BINARY_DIR}/${Source}.idx )
   SET(INDEX_FILE_CONTENT "${INDEX_FILE_CONTENT}${WrapITK_BINARY_DIR}/${Source}.idx\n")
ENDFOREACH(Source)

CONFIGURE_FILE(
${WrapITK_SOURCE_DIR}/ITKCommonTCL.mdx.in
${WrapITK_BINARY_DIR}/ITKCommonTCL.mdx
)

# set the generated sources as generated
SET_SOURCE_FILES_PROPERTIES(${WRAP_TCL_SOURCES} ${WRAP_PYTHON_SOURCES} GENERATED )

# add the library
ADD_LIBRARY(ITKCommonTcl SHARED ${WRAP_TCL_SOURCES})
ADD_LIBRARY(_ITKCommonTcl MODULE ${WRAP_PYTHON_SOURCES})
SET_TARGET_PROPERTIES( _ITKCommonTcl PROPERTIES PREFIX "")
SET(IGNORE_WARNINGS -w362 -w389 -w503 -w508 -w509)

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

MACRO(CSWIG_CREATE_TCL_CXX_FILE Bin InputIdx InputXml OutputTclCxx Library LibraryIndexFiles)
   ADD_CUSTOM_COMMAND(
   SOURCE ${Bin}/${InputIdx}
   COMMAND ${CSWIG}
   ARGS -c -cindex ${Bin}/${InputIdx} ${IGNORE_WARNINGS} -o ${Bin}/${OutputTclCxx} -tcl -c++ ${Bin}/${InputXml}
   TARGET ${Library}
   OUTPUTS ${Bin}/${OutputTclCxx}
   DEPENDS ${LibraryIndexFiles}
 )
ENDMACRO(CSWIG_CREATE_TCL_CXX_FILE)


MACRO(CSWIG_CREATE_PYTHON_CXX_FILE Bin InputIdx InputXml OutputTclCxx Library LibraryIndexFiles)
   ADD_CUSTOM_COMMAND(
   SOURCE ${Bin}/${InputIdx}
   COMMAND ${CSWIG}
   ARGS -c -cindex ${Bin}/${InputIdx} ${IGNORE_WARNINGS} -o ${Bin}/${OutputTclCxx} -python -c++ ${Bin}/${InputXml}
   TARGET ${Library}
   OUTPUTS ${Bin}/${OutputTclCxx}
   DEPENDS ${LibraryIndexFiles}
 )
ENDMACRO(CSWIG_CREATE_PYTHON_CXX_FILE)


# wrap each of the files
FOREACH(Source ${WRAP_SOURCES})
  GCCXML_CREATE_XML_FILE(${ITK_TOP}/Wrapping/Tcl/Common 
                         ${WrapITK_BINARY_DIR} ${Source}.cxx ${Source}.xml ITKCommonTcl)
  CINDEX_CREATE_IDX_FILE(${WrapITK_BINARY_DIR} ${Source}.xml ${Source}.idx ITKCommonTcl)
  CSWIG_CREATE_TCL_CXX_FILE(${WrapITK_BINARY_DIR} 
                            ITKCommonTCL.mdx
                            ${Source}.xml ${Source}Tcl.cxx ITKCommonTcl
                            "${ALL_IDX_FILES}")
  CSWIG_CREATE_PYTHON_CXX_FILE(${WrapITK_BINARY_DIR} 
                            ITKCommonTCL.mdx
                            ${Source}.xml ${Source}Python.cxx _ITKCommonTcl
                            "${ALL_IDX_FILES}")
ENDFOREACH(Source)

# link to ITKCommon
TARGET_LINK_LIBRARIES(ITKCommonTcl ${TCL_LIBRARY} ITKCommon)
TARGET_LINK_LIBRARIES(_ITKCommonTcl ${PYTHON_LIBRARY} ITKCommon)
