#goes ignored. The solution for me was to add a .i with just the
#following:

#%module swigruntime

#And compile it without -c, while compiling the others with -c. Then
#using -DSWIG_GLOBAL in my CFLAGS for everything. I expected I might
#get some linker clashes from this but it went smoothly. I'll have to
#dig around the automake docs to see if i can set it for just the
#swigruntime.cc.

SET_SOURCE_FILES_PROPERTIES(${WrapITK_BINARY_DIR}/SwigRuntime/swigrunTcl.cxx GENERATED)
SET_SOURCE_FILES_PROPERTIES(${WrapITK_BINARY_DIR}/SwigRuntime/swigrunPython.cxx GENERATED)

ADD_LIBRARY(SwigRuntimeTcl SHARED ${WrapITK_BINARY_DIR}/swigrunTcl.cxx)
ADD_LIBRARY(SwigRuntimePython SHARED ${WrapITK_BINARY_DIR}/swigrunPython.cxx)
TARGET_LINK_LIBRARIES(SwigRuntimeTcl ${TCL_LIBRARY})
TARGET_LINK_LIBRARIES(SwigRuntimePython ${PYTHON_LIBRARY})

ADD_CUSTOM_COMMAND(
   SOURCE ${WrapITK_SOURCE_DIR}/SwigRuntime/swigrun.h
   COMMAND ${GCCXML}
   ARGS -fxml-start=_cable_ -DCABLE_CONFIGURATION ${WrapITK_SOURCE_DIR}/SwigRuntime/swigrun.h
        -fxml=${WrapITK_BINARY_DIR}/swigrun.xml 
   TARGET SwigRuntimeTcl
   OUTPUTS ${WrapITK_BINARY_DIR}/SwigRuntime/swigrun.xml )

ADD_CUSTOM_COMMAND(
   SOURCE ${WrapITK_SOURCE_DIR}/SwigRuntime/swigrun.h
   COMMAND ${CSWIG}
   ARGS -o ${WrapITK_BINARY_DIR}/SwigRuntime/swigrunTcl.cxx -tcl -c++ ${WrapITK_BINARY_DIR}/swigrun.xml
   TARGET SwigRuntimeTcl
   OUTPUTS ${WrapITK_BINARY_DIR}/SwigRuntime/swigrunTcl.cxx
   DEPENDS ${WrapITK_BINARY_DIR}/SwigRuntime/swigrun.xml
 )

ADD_CUSTOM_COMMAND(
   SOURCE ${WrapITK_SOURCE_DIR}/SwigRuntime/swigrun.h
   COMMAND ${CSWIG}
   ARGS -o ${WrapITK_BINARY_DIR}/SwigRuntime/swigrunPython.cxx -python -c++ ${WrapITK_BINARY_DIR}/swigrun.xml
   TARGET SwigRuntimePython
   OUTPUTS ${WrapITK_BINARY_DIR}/SwigRuntime/swigrunPython.cxx
   DEPENDS ${WrapITK_BINARY_DIR}/SwigRuntime/swigrun.xml
 )
