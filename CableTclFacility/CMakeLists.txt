INCLUDE(${CMAKE_ROOT}/Modules/FindTCL.cmake)
INCLUDE(${CABLE_SOURCE_DIR}/CxxTypes/CMakeLists_Sources.txt)

INCLUDE_DIRECTORIES(${TCL_INCLUDE_PATH}
                    ${CABLE_SOURCE_DIR}
                    ${CABLE_SOURCE_DIR}/CableTclFacility)

SET(CableTclFacility_SRCS
  ctArgument.cxx
  ctCalls.cxx
  ctClassWrapper.cxx
  ctConstructor.cxx
  ctCxxObject.cxx
  ctFunction.cxx
  ctFunctionBase.cxx
  ctFunctionSelector.cxx
  ctFunctionWrapper.cxx
  ctInit.cxx
  ctMethod.cxx
  ctStaticMethod.cxx
  ctTclCxxObject.cxx
  ctTypeInfo.cxx
  ctUtils.cxx
  ctWrapperFacility.cxx
)

IF (WIN32)
  SET(CableTclFacility_SRCS ${CableTclFacility_SRCS} ctWin32OutputWindow.cxx)
ENDIF (WIN32)

ADD_DEFINITIONS(-DCxxTypes_EXPORTS)
ADD_LIBRARY(CableTclFacility SHARED ${CxxTypes_SRCS} ${CableTclFacility_SRCS})

TARGET_LINK_LIBRARIES(CableTclFacility ${TCL_LIBRARY})

IF(UNIX)
  SET(CABLE_LIBNAME_PREFIX "lib")
ELSE(UNIX)
  SET(CABLE_LIBNAME_PREFIX "")
ENDIF(UNIX)

#-----------------------------------------------------------------------------
# Create install targets.
IF(CABLE_CMAKE_HAS_EXPLICIT_INSTALL_FILES)
  SET(CABLE_TCL_PKGINDEX_FOR_INSTALL "${CABLE_BINARY_DIR}/CableTclFacility/Install/Hide/pkgIndex.tcl")
  INSTALL_FILES(/lib/Cable FILES "${CABLE_TCL_PKGINDEX_FOR_INSTALL}")
ENDIF(CABLE_CMAKE_HAS_EXPLICIT_INSTALL_FILES)

INSTALL_FILES(/include/Cable/CableTclFacility "^ct.*\\.h$")
INSTALL_FILES(/include/Cable/CableTclFacility "^ct.*\\.txx$")
INSTALL_TARGETS(/lib/Cable CableTclFacility)

#-----------------------------------------------------------------------------
# Configure the pkgIndex.tcl file for the build tree.
IF(CABLE_MSDEV_CONFIG_DIRS)
  FOREACH (config ${CABLE_MSDEV_CONFIG_DIRS})
    SET(CABLE_TCLFACILITY_DIR "${CABLE_TCL_LIBRARY_DIR}/${config}")
    CONFIGURE_FILE("${CABLE_SOURCE_DIR}/CableTclFacility/pkgIndex.tcl.in"
                   "${CABLE_BINARY_DIR}/CableTclFacility/${config}/pkgIndex.tcl" IMMEDIATE)
  ENDFOREACH (config)
ELSE(CABLE_MSDEV_CONFIG_DIRS)
  SET(CABLE_TCLFACILITY_DIR "${CABLE_TCL_LIBRARY_DIR}")
  CONFIGURE_FILE("${CABLE_SOURCE_DIR}/CableTclFacility/pkgIndex.tcl.in"
                 "${CABLE_BINARY_DIR}/CableTclFacility/pkgIndex.tcl" IMMEDIATE)
ENDIF(CABLE_MSDEV_CONFIG_DIRS)

#-----------------------------------------------------------------------------
# Configure the pkgIndex.tcl file for the install tree.
SET(CABLE_TCLFACILITY_DIR "[file dirname [info script]]")
CONFIGURE_FILE("${CABLE_SOURCE_DIR}/CableTclFacility/pkgIndex.tcl.in"
               "${CABLE_TCL_PKGINDEX_FOR_INSTALL}" IMMEDIATE)
