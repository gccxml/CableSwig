INCLUDE_DIRECTORIES(${GCC_BINARY_DIR}/gcc)
INCLUDE_DIRECTORIES(${GCC_SOURCE_DIR}/gcc)
INCLUDE_DIRECTORIES(${GCC_SOURCE_DIR}/gcc/config)
INCLUDE_DIRECTORIES(${GCC_SOURCE_DIR}/include)

ADD_DEFINITIONS(-DIN_GCC -DHAVE_CONFIG_H
                "-DLOCALEDIR=\\\"${CMAKE_INSTALL_PREFIX}/share/locale\\\"")

ADD_LIBRARY(cpp
  cpplib.c cpplex.c cppmacro.c cppexp.c cppfiles.c
  cpphash.c cpperror.c cppinit.c cppdefault.c
  mkdeps.c prefix.c version.c mbchar.c
)

ADD_EXECUTABLE(cpp0 cppmain.c intl.c ${GCC_SOURCE_DIR}/libiberty/obstack.c)
TARGET_LINK_LIBRARIES(cpp0 cpp iberty)

SET(GCC_backend_GENSRCS
  ${GCC_BINARY_DIR}/gcc/genrtl.c
  ${GCC_BINARY_DIR}/gcc/insn-attrtab.c
  ${GCC_BINARY_DIR}/gcc/insn-emit.c
  ${GCC_BINARY_DIR}/gcc/insn-extract.c
  ${GCC_BINARY_DIR}/gcc/insn-opinit.c
  ${GCC_BINARY_DIR}/gcc/insn-output.c
  ${GCC_BINARY_DIR}/gcc/insn-peep.c
  ${GCC_BINARY_DIR}/gcc/insn-recog.c
)

SET(GCC_backend_GENHEADERS
  ${GCC_BINARY_DIR}/gcc/insn-attr.h
  ${GCC_BINARY_DIR}/gcc/insn-codes.h
  ${GCC_BINARY_DIR}/gcc/insn-flags.h
  ${GCC_BINARY_DIR}/gcc/tree-check.h
)

SET(GCC_backend_SRCS
  ${GCC_SOURCE_DIR}/libiberty/splay-tree.c
  ${GCC_backend_GENSRCS}
  alias.c
  bb-reorder.c
  bitmap.c
  builtins.c
  caller-save.c
  calls.c
  combine.c
  config/i386/i386.c
  conflict.c
  convert.c
  cse.c
  dbxout.c
  dce.c
  dependence.c
  diagnostic.c
  doloop.c
  dominance.c
  dwarf2asm.c
  dwarf2out.c
  dwarfout.c
  emit-rtl.c
  except.c
  explow.c
  expmed.c
  expr.c
  final.c
  flow.c
  fold-const.c
  function.c
  gcse.c
  ggc-common.c
  ggc-page.c
  global.c
  graph.c
  haifa-sched.c
  hash.c
  ifcvt.c
  integrate.c
  intl.c
  jump.c
  lcm.c
  lists.c
  local-alloc.c
  loop.c
  mbchar.c
  optabs.c
  params.c
  predict.c
  print-rtl.c
  print-tree.c
  profile.c
  real.c
  recog.c
  reg-stack.c
  regclass.c
  regmove.c
  regrename.c
  reload.c
  reload1.c
  reorg.c
  resource.c
  rtl.c
  rtlanal.c
  sbitmap.c
  sched-deps.c
  sched-ebb.c
  sched-rgn.c
  sched-vis.c
  sdbout.c
  sibcall.c
  simplify-rtx.c
  ssa.c
  stmt.c
  stor-layout.c
  stringpool.c
  timevar.c
  tree.c
  unroll.c
  varasm.c
  varray.c
  version.c
  xcoffout.c
)

SET_SOURCE_FILES_PROPERTIES(${GCC_backend_GENSRCS} GENERATED)
SET_SOURCE_FILES_PROPERTIES(${GCC_backend_SRCS}
                            OBJECT_DEPENDS "${GCC_backend_GENHEADERS}")

ADD_LIBRARY(backend ${GCC_backend_SRCS})



SET(OUTPUT_FILE  ${GCC_BINARY_DIR}/gcc/tree-check.h)
IF(WIN32)
  STRING(REGEX REPLACE "/" "\\\\" OUTPUT_FILE ${OUTPUT_FILE}) 
ENDIF(WIN32)
ADD_CUSTOM_COMMAND(TARGET backend
                   SOURCE ${GCC_SOURCE_DIR}/gcc/config/i386/i386.md
                   COMMAND ${GCC_gencheck_EXE}
                   ARGS > ${OUTPUT_FILE}
                   OUTPUTS ${GCC_BINARY_DIR}/gcc/tree-check.h
                   DEPENDS ${GCC_gencheck_EXE})


FOREACH(gen attr codes flags  )
  SET(OUTPUT_FILE  ${GCC_BINARY_DIR}/gcc/insn-${gen}.h)
  IF(WIN32)
    STRING(REGEX REPLACE "/" "\\\\" OUTPUT_FILE ${OUTPUT_FILE}) 
  ENDIF(WIN32)
  ADD_CUSTOM_COMMAND(TARGET backend
    SOURCE ${GCC_SOURCE_DIR}/gcc/config/i386/i386.md
    COMMAND ${GCC_gen${gen}_EXE}
    ARGS ${GCC_SOURCE_DIR}/gcc/config/i386/i386.md > ${OUTPUT_FILE}
    OUTPUTS ${GCC_BINARY_DIR}/gcc/insn-${gen}.h
    DEPENDS ${GCC_gen${gen}_EXE})
ENDFOREACH(gen)

FOREACH(gen peep opinit recog extract output emit attrtab )
  SET(OUTPUT_FILE  ${GCC_BINARY_DIR}/gcc/insn-${gen}.c)
  IF(WIN32)
    STRING(REGEX REPLACE "/" "\\\\" OUTPUT_FILE ${OUTPUT_FILE}) 
  ENDIF(WIN32)
  ADD_CUSTOM_COMMAND(TARGET backend
    SOURCE ${GCC_SOURCE_DIR}/gcc/config/i386/i386.md
    COMMAND ${GCC_gen${gen}_EXE}
    ARGS ${GCC_SOURCE_DIR}/gcc/config/i386/i386.md > ${OUTPUT_FILE}
    OUTPUTS ${GCC_BINARY_DIR}/gcc/insn-${gen}.c
    DEPENDS ${GCC_gen${gen}_EXE})
ENDFOREACH(gen)

SET(OUTPUT_FILE  ${GCC_BINARY_DIR}/gcc/genrtl.c)
IF(WIN32)
  STRING(REGEX REPLACE "/" "\\\\" OUTPUT_FILE ${OUTPUT_FILE}) 
ENDIF(WIN32)

ADD_CUSTOM_COMMAND(TARGET backend
                   SOURCE ${GCC_SOURCE_DIR}/gcc/rtl.def
                   COMMAND ${GCC_gengenrtl_EXE}
                   DEPENDS ${GCC_BINARY_DIR}/gcc/genrtl.h
                   ARGS > ${OUTPUT_FILE}
                   OUTPUTS ${GCC_BINARY_DIR}/gcc/genrtl.c
                   DEPENDS ${GCC_gengenrtl_EXE})

SUBDIRS(cp)
