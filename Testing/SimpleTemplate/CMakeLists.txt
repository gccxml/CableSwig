PROJECT(wrapTemplate)

SET_SOURCE_FILES_PROPERTIES(simpleTemplateTcl.cxx GENERATED )

ADD_LIBRARY(WrapTemplate SHARED simpleTemplateTcl.cxx)

CSWIG_WRAP_FILE(
        ${wrapTemplate_SOURCE_DIR}
        ${wrapTemplate_BINARY_DIR}
        simpleTemplate.h
        simpleTemplateTcl.cxx
        WrapTemplate)

TARGET_LINK_LIBRARIES(WrapTemplate ${TCL_LIBRARY})
