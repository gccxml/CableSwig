PROJECT(TestModule)

SET_SOURCE_FILES_PROPERTIES(fooTcl.cxx barTcl.cxx moduleTcl.cxx GENERATED )
ADD_LIBRARY(TestModule SHARED moduleTcl.cxx fooTcl.cxx barTcl.cxx )

CSWIG_WRAP_FILE(
        ${TestModule_SOURCE_DIR}
        ${TestModule_BINARY_DIR}
        foo.h
        fooTcl.cxx
        TestModule)

CSWIG_WRAP_FILE(
        ${TestModule_SOURCE_DIR}
        ${TestModule_BINARY_DIR}
        bar.h
        barTcl.cxx
        TestModule)

CSWIG_WRAP_FILE(
        ${TestModule_SOURCE_DIR}
        ${TestModule_BINARY_DIR}
        module.h
        moduleTcl.cxx
        TestModule)

TARGET_LINK_LIBRARIES(TestModule ${TCL_LIBRARY})
