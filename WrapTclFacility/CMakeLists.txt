INCLUDE(${CMAKE_ROOT}/Modules/FindTCL.cmake)
INCLUDE(${CABLE_SOURCE_DIR}/CxxTypes/CMakeLists_Sources.txt)

INCLUDE_DIRECTORIES(
${TCL_INCLUDE_PATH}
${CABLE_SOURCE_DIR}/WrapTclFacility
)

LINK_DIRECTORIES(
${CABLE_BINARY_DIR}/WrapTclFacility
)

LINK_LIBRARIES(
${TCL_LIBRARY}
)

ADD_DEFINITIONS(-DCxxTypes_EXPORTS)

SOURCE_FILES(WrapTclFacility_SRCS
wrapArgument
wrapCalls
wrapClassWrapper
wrapConstructor
wrapCxxObject
wrapFunction
wrapFunctionBase
wrapFunctionSelector
wrapFunctionWrapper
wrapInit
wrapMethod
wrapStaticMethod
wrapTclCxxObject
wrapTypeInfo
wrapUtils
wrapWrapperFacility
)

IF (WIN32)
  SOURCE_FILES(WrapTclFacility_SRCS wrapWin32OutputWindow)
ENDIF (WIN32)

ADD_LIBRARY(WrapTclFacility SHARED CxxTypes_SRCS WrapTclFacility_SRCS)  

INCLUDE(${CABLE_SOURCE_DIR}/WrapTclFacility/wrapConfigureTclPackages.cmake)

IF (NOT CABLE_NO_INSTALL)
  INSTALL_FILES(/share/Cable/include/WrapTclFacility ^wrap.*\\.h$)
  INSTALL_FILES(/share/Cable/include/WrapTclFacility ^wrap.*\\.txx$)
  INSTALL_TARGETS(/share/Cable/lib WrapTclFacility)
  IF(UNIX)
    SET (CABLE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
    CONFIGURE_FILE(${CABLE_SOURCE_DIR}/WrapTclFacility/pkgIndex.tcl.unix
                   ${CABLE_BINARY_DIR}/WrapTclFacility/Install/Hide/pkgIndex.tcl)
  ELSE(UNIX)
    CONFIGURE_FILE(${CABLE_SOURCE_DIR}/WrapTclFacility/pkgIndex.tcl.win32
                   ${CABLE_BINARY_DIR}/WrapTclFacility/Install/Hide/pkgIndex.tcl)
  ENDIF(UNIX)
  INSTALL_FILES(/share/Cable/lib "" Install/Hide/pkgIndex.tcl)
ENDIF (NOT CABLE_NO_INSTALL)
