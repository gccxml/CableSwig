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
#  wrap_ITKUtils
  wrap_itkArray
  wrap_itkContinuousIndex
  wrap_itkDenseFiniteDifferenceImageFilter
  wrap_itkDifferenceImageFilter
  wrap_itkEventObject
  wrap_itkFiniteDifferenceImageFilter
  wrap_itkFixedArray
  wrap_itkImage
#  wrap_itkImageToImageFilter  duplicates wrap_ITKFilterBaseTcl.cxx
  wrap_itkIndex
  wrap_itkPoint
  wrap_itkSize
  wrap_itkVector
)

# loop over cable config files creating two lists:
# WRAP_TCL_SOURCES: list of generated files
FOREACH(Source ${WRAP_SOURCES})
   SET(WRAP_TCL_SOURCES ${WRAP_TCL_SOURCES} ${Source}Tcl.cxx)
   SET(WRAP_PYTHON_SOURCES ${WRAP_PYTHON_SOURCES} ${Source}Python.cxx)
ENDFOREACH(Source)
# set the generated sources as generated
SET_SOURCE_FILES_PROPERTIES(${WRAP_TCL_SOURCES} ${WRAP_PYTHON_SOURCES} GENERATED )

# add the library
ADD_LIBRARY(ITKCommonTcl SHARED ${WRAP_TCL_SOURCES})
ADD_LIBRARY(_ITKCommonTcl MODULE ${WRAP_PYTHON_SOURCES})
SET_TARGET_PROPERTIES( _ITKCommonTcl PROPERTIES PREFIX "")
SET(IGNORE_WARNINGS -w362 -w389 -w503 -w508 -w509)

MACRO(CSWIG_WRAP_TCL_FILE Source Bin Input Output Library)
   ADD_CUSTOM_COMMAND(
   SOURCE ${Source}/${Input}
   COMMAND ${CSWIG}
   ARGS ${IGNORE_WARNINGS} -o ${Bin}/${Output} ${SWIG_INC}
        -tcl -c++ ${Source}/${Input}
  TARGET ${Library}
  OUTPUTS ${Bin}/${Output} )
ENDMACRO(CSWIG_WRAP_TCL_FILE)

MACRO(CSWIG_WRAP_PYTHON_FILE Source Bin Input Output Library)
   ADD_CUSTOM_COMMAND(
   SOURCE ${Source}/${Input}
   COMMAND ${CSWIG}
   ARGS ${IGNORE_WARNINGS} -o ${Bin}/${Output} ${SWIG_INC}
        -python -c++ ${Source}/${Input}
  TARGET ${Library}
  OUTPUTS ${Bin}/${Output} )
ENDMACRO(CSWIG_WRAP_PYTHON_FILE)


# wrap each of the files
FOREACH(Source ${WRAP_SOURCES})
  CSWIG_WRAP_TCL_FILE(
        ${ITK_TOP}/Wrapping/Tcl/Common
        ${WrapITK_BINARY_DIR}
        ${Source}.cxx
        ${Source}Tcl.cxx
        ITKCommonTcl)

  CSWIG_WRAP_PYTHON_FILE(
        ${ITK_TOP}/Wrapping/Tcl/Common
        ${WrapITK_BINARY_DIR}
        ${Source}.cxx
        ${Source}Python.cxx
        _ITKCommonTcl)
ENDFOREACH(Source)

# link to ITKCommon
TARGET_LINK_LIBRARIES(ITKCommonTcl ${TCL_LIBRARY} ITKCommon)
TARGET_LINK_LIBRARIES(_ITKCommonTcl ${PYTHON_LIBRARY} ITKCommon)
