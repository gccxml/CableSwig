# create the ITKIOTcl libraries
SET(WRAP_SOURCES
    wrap_ITKIO
    wrap_IOBase
    wrap_itkImageFileReader
    wrap_itkImageFileWriter
#    wrap_itkTkImageViewer2D
)

# loop over cable config files creating two lists:
# WRAP_TCL_SOURCES: list of generated files
FOREACH(Source ${WRAP_SOURCES})
   SET(WRAP_TCL_SOURCES ${WRAP_TCL_SOURCES} ${Source}Tcl.cxx)
   SET(WRAP_PYTHON_SOURCES ${WRAP_PYTHON_SOURCES} ${Source}Python.cxx)
   SET(ALL_IDX_FILES ${ALL_IDX_FILES} ${WrapITK_BINARY_DIR}/IO/${Source}.idx )
   SET(INDEX_FILE_CONTENT "${INDEX_FILE_CONTENT}${WrapITK_BINARY_DIR}/IO/${Source}.idx\n")
ENDFOREACH(Source)

# set the generated sources as generated
SET_SOURCE_FILES_PROPERTIES(${WRAP_TCL_SOURCES} ${WRAP_PYTHON_SOURCES} GENERATED )

ADD_LIBRARY(ITKIOTcl SHARED ${WRAP_TCL_SOURCES})
ADD_LIBRARY(_ITKIOTcl MODULE ${WRAP_PYTHON_SOURCES})
SET_TARGET_PROPERTIES( _ITKIOTcl PROPERTIES PREFIX "")

TARGET_LINK_LIBRARIES(ITKIOTcl ITKIO VXLNumericsTcl ITKNumericsTcl ITKCommonTcl SwigRuntimeTcl ${TCL_LIBRARY})
TARGET_LINK_LIBRARIES(_ITKIOTcl ITKIO SwigRuntimePython ${PYTHON_LIBRARY})

CONFIGURE_FILE(
${WrapITK_SOURCE_DIR}/Master.mdx.in
${WrapITK_BINARY_DIR}/IO/ITKIO.mdx IMMEDIATE
)

SET(MASTER_INDEX_FILES "${WrapITK_BINARY_DIR}/VXLNumerics/VXLNumericsTcl.mdx"
                       "${WrapITK_BINARY_DIR}/Numerics/ITKNumericsTcl.mdx"
                       "${WrapITK_BINARY_DIR}/Common/ITKCommonTcl.mdx"
                       "${WrapITK_BINARY_DIR}/BasicFilters/ITKBasicFilters.mdx"
                       "${WrapITK_BINARY_DIR}/IO/ITKIO.mdx"
)

FOREACH(Source ${WRAP_SOURCES})
# tcl
  GCCXML_CREATE_XML_FILE(${ITK_TOP}/Wrapping/Tcl/IO 
                         ${WrapITK_BINARY_DIR}/IO ${Source}.cxx ${Source}.xml ITKIOTcl)
  CINDEX_CREATE_IDX_FILE(${WrapITK_BINARY_DIR}/IO ${Source}.xml ${Source}.idx ITKIOTcl)
  CSWIG_CREATE_TCL_CXX_FILE(${WrapITK_BINARY_DIR}/IO
                            "${MASTER_INDEX_FILES}" ${Source}.idx
                            ${Source}.xml ${Source}Tcl.cxx ITKIOTcl
                            "${ALL_IDX_FILES}")
# python
  GCCXML_CREATE_XML_FILE(${ITK_TOP}/Wrapping/Tcl/IO 
                         ${WrapITK_BINARY_DIR}/IO ${Source}.cxx ${Source}.xml _ITKIOTcl)
  CINDEX_CREATE_IDX_FILE(${WrapITK_BINARY_DIR}/IO ${Source}.xml ${Source}.idx _ITKIOTcl)
  CSWIG_CREATE_PYTHON_CXX_FILE(${WrapITK_BINARY_DIR}/IO 
                            "${MASTER_INDEX_FILES}" ${Source}.idx
                            ${Source}.xml ${Source}Python.cxx _ITKIOTcl
                            "${ALL_IDX_FILES}")
ENDFOREACH(Source)
