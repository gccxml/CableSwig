INCLUDE(${CMAKE_ROOT}/Modules/FindTCL.cmake)
INCLUDE(${CABLE_SOURCE_DIR}/CxxTypes/CMakeLists_Sources.txt)

INCLUDE_DIRECTORIES(${TCL_INCLUDE_PATH}
                    ${CABLE_SOURCE_DIR}
                    ${CABLE_SOURCE_DIR}/CableTclFacility)

SOURCE_FILES(CableTclFacility_SRCS
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
  SOURCE_FILES(CableTclFacility_SRCS ctWin32OutputWindow.cxx)
ENDIF (WIN32)

ADD_DEFINITIONS(-DCxxTypes_EXPORTS)
ADD_LIBRARY(CableTclFacility SHARED CxxTypes_SRCS CableTclFacility_SRCS)  

TARGET_LINK_LIBRARIES(CableTclFacility ${TCL_LIBRARY})

INCLUDE(${CABLE_SOURCE_DIR}/CableTclFacility/ctConfigureTclPackages.cmake)

IF (NOT CABLE_NO_INSTALL)
  INSTALL_FILES(/share/Cable/include/CableTclFacility ^ct.*\\.h$)
  INSTALL_FILES(/share/Cable/include/CableTclFacility ^ct.*\\.txx$)
  INSTALL_TARGETS(/lib CableTclFacility)
  IF(UNIX)
    SET (CABLE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
    CONFIGURE_FILE(${CABLE_SOURCE_DIR}/CableTclFacility/pkgIndex.tcl.unix
                   ${CABLE_BINARY_DIR}/CableTclFacility/Install/Hide/pkgIndex.tcl)
  ELSE(UNIX)
    CONFIGURE_FILE(${CABLE_SOURCE_DIR}/CableTclFacility/pkgIndex.tcl.win32
                   ${CABLE_BINARY_DIR}/CableTclFacility/Install/Hide/pkgIndex.tcl)
  ENDIF(UNIX)
  INSTALL_FILES(/share/Cable "" Install/Hide/pkgIndex.tcl)
ENDIF (NOT CABLE_NO_INSTALL)
