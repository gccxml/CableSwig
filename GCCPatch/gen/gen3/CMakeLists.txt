FOREACH(g
    genrecog
    genextract
  )
  ADD_EXECUTABLE(${g} ${GCC_SOURCE_DIR}/gcc/${g}.c ${GEN_SRCS})
  TARGET_LINK_LIBRARIES(${g} iberty)
  SET_SOURCE_FILES_PROPERTIES(${GCC_SOURCE_DIR}/gcc/${g}.c OBJECT_DEPENDS
                              ${GCC_BINARY_DIR}/gcc/insn-config.h)

  SET(OUTPUT_FILE  ${GCC_BINARY_DIR}/gcc/insn-config.h)
  IF(WIN32)
    IF(NOT CYGWIN)
      STRING(REGEX REPLACE "/" "\\\\" OUTPUT_FILE ${OUTPUT_FILE}) 
    ENDIF(NOT CYGWIN)
  ENDIF(WIN32)
  ADD_CUSTOM_COMMAND(TARGET genextract
                     SOURCE ${GCC_SOURCE_DIR}/gcc/config/i386/i386.md
                     COMMAND ${GCC_genconfig_EXE}
                     ARGS ${GCC_SOURCE_DIR}/gcc/config/i386/i386.md
                          > ${OUTPUT_FILE}
                     OUTPUTS ${GCC_BINARY_DIR}/gcc/insn-config.h
                     DEPENDS ${GCC_genconfig_EXE})
ENDFOREACH(g)
