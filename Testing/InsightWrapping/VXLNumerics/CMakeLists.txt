# create the VXLNumerics libraries

SET(WRAP_SOURCES
  wrap_VXLNumerics
  wrap_vnl_matrix
  wrap_vnl_vector
  wrap_vnl_c_vector
  wrap_vnl_diag_matrix
  wrap_vnl_file_matrix
  wrap_vnl_file_vector
  wrap_vnl_fortran_copy
  wrap_vnl_matrix_fixed
  wrap_vnl_matrix_fixed_ref
  wrap_vnl_matrix_ref
  wrap_vnl_vector_ref
)


# loop over cable config files creating two lists:
# WRAP_TCL_SOURCES: list of generated files
FOREACH(Source ${WRAP_SOURCES})
   SET(WRAP_TCL_SOURCES ${WRAP_TCL_SOURCES} ${Source}Tcl.cxx)
   SET(WRAP_PYTHON_SOURCES ${WRAP_PYTHON_SOURCES} ${Source}Python.cxx)
   SET(ALL_IDX_FILES ${ALL_IDX_FILES} ${WrapITK_BINARY_DIR}/VXLNumerics/${Source}.idx )
   SET(INDEX_FILE_CONTENT "${INDEX_FILE_CONTENT}${WrapITK_BINARY_DIR}/VXLNumerics/${Source}.idx\n")
ENDFOREACH(Source)

# set the generated sources as generated
SET_SOURCE_FILES_PROPERTIES(${WRAP_TCL_SOURCES} ${WRAP_PYTHON_SOURCES} GENERATED )
# add the tcl and python libraries
ADD_LIBRARY(VXLNumericsTcl SHARED ${WRAP_TCL_SOURCES})
ADD_LIBRARY(_VXLNumericsTcl MODULE ${WRAP_PYTHON_SOURCES})
SET_TARGET_PROPERTIES( _VXLNumericsTcl PROPERTIES PREFIX "")
TARGET_LINK_LIBRARIES(VXLNumericsTcl itkvnl itkvnl_algo SwigRuntimeTcl ${TCL_LIBRARY})
TARGET_LINK_LIBRARIES(_VXLNumericsTcl itkvnl itkvnl_algo  SwigRuntimePython ${PYTHON_LIBRARY})

CONFIGURE_FILE( 
${WrapITK_SOURCE_DIR}/Master.mdx.in
${WrapITK_BINARY_DIR}/VXLNumerics/VXLNumericsTcl.mdx IMMEDIATE
)

SET(MASTER_INDEX_FILES "${WrapITK_BINARY_DIR}/VXLNumerics/VXLNumericsTcl.mdx")
FOREACH(Source ${WRAP_SOURCES})
# tcl
  GCCXML_CREATE_XML_FILE(${ITK_TOP}/Wrapping/Tcl/Numerics 
                         ${WrapITK_BINARY_DIR}/VXLNumerics ${Source}.cxx ${Source}.xml VXLNumericsTcl)
  CINDEX_CREATE_IDX_FILE(${WrapITK_BINARY_DIR}/VXLNumerics ${Source}.xml ${Source}.idx VXLNumericsTcl)
  CSWIG_CREATE_TCL_CXX_FILE(${WrapITK_BINARY_DIR}/VXLNumerics
                            "${MASTER_INDEX_FILES}" ${Source}.idx
                            ${Source}.xml ${Source}Tcl.cxx VXLNumericsTcl
                            "${ALL_IDX_FILES}")
# python 
  GCCXML_CREATE_XML_FILE(${ITK_TOP}/Wrapping/Tcl/Numerics 
                         ${WrapITK_BINARY_DIR}/VXLNumerics ${Source}.cxx ${Source}.xml _VXLNumericsTcl)
  CINDEX_CREATE_IDX_FILE(${WrapITK_BINARY_DIR}/VXLNumerics ${Source}.xml ${Source}.idx _VXLNumericsTcl)
  CSWIG_CREATE_PYTHON_CXX_FILE(${WrapITK_BINARY_DIR}/VXLNumerics 
                            "${MASTER_INDEX_FILES}" ${Source}.idx
                            ${Source}.xml ${Source}Python.cxx _VXLNumericsTcl
                            "${ALL_IDX_FILES}")
ENDFOREACH(Source)

