FOREACH(g
    genrecog
    genextract
  )
  ADD_EXECUTABLE(${g} ${GCC_SOURCE_DIR}/gcc/${g}.c ${GEN_SRCS})

  SET_SOURCE_FILES_PROPERTIES(${GCC_SOURCE_DIR}/gcc/${g}.c OBJECT_DEPENDS
                              ${GCC_BINARY_DIR}/gcc/insn-config.h)

  ADD_CUSTOM_COMMAND(TARGET genextract
                     SOURCE ${GCC_SOURCE_DIR}/gcc/config/i386/i386.md
                     COMMAND ${GCC_genconfig_EXE}
                     ARGS ${GCC_SOURCE_DIR}/gcc/config/i386/i386.md
                          > ${GCC_BINARY_DIR}/gcc/insn-config.h
                     OUTPUTS ${GCC_BINARY_DIR}/gcc/insn-config.h
                     DEPENDS ${GCC_genconfig_EXE})
ENDFOREACH(g)
