FOREACH(g
    gencodes
    genflags
    genattr
    genconfig
    genpeep
    genopinit
    genoutput
    genemit
    genattrtab
  )
  ADD_EXECUTABLE(${g} ${GCC_SOURCE_DIR}/gcc/${g}.c ${GEN_SRCS})
  TARGET_LINK_LIBRARIES(${g} iberty)
  SET_SOURCE_FILES_PROPERTIES(${GCC_SOURCE_DIR}/gcc/${g}.c
    OBJECT_DEPENDS ${GCC_BINARY_DIR}/gcc/genrtl.h)
  
  SET(OUTPUT_FILE  ${GCC_BINARY_DIR}/gcc/genrtl.h)
  IF(WIN32)
    IF(NOT CYGWIN)
      STRING(REGEX REPLACE "/" "\\\\" OUTPUT_FILE ${OUTPUT_FILE}) 
    ENDIF(NOT CYGWIN)
  ENDIF(WIN32)
  ADD_CUSTOM_COMMAND(TARGET ${g}
                     SOURCE ${GCC_SOURCE_DIR}/gen/rtl.def.dummy
                     COMMAND ${GCC_gengenrtl_EXE}
                     ARGS -h > ${OUTPUT_FILE}
                     OUTPUTS ${GCC_BINARY_DIR}/gcc/genrtl.h
                     DEPENDS ${GCC_SOURCE_DIR}/gcc/rtl.def
                             ${GCC_gengenrtl_EXE})
ENDFOREACH(g)

ADD_EXECUTABLE(gencheck ${GCC_SOURCE_DIR}/gcc/gencheck.c)
