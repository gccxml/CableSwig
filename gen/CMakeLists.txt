INCLUDE_DIRECTORIES(${GCC_BINARY_DIR}/gcc)
INCLUDE_DIRECTORIES(${GCC_SOURCE_DIR}/gcc)
INCLUDE_DIRECTORIES(${GCC_SOURCE_DIR}/gcc/config)
INCLUDE_DIRECTORIES(${GCC_SOURCE_DIR}/include)

INCLUDE_REGULAR_EXPRESSION("^[^i].*|^i[^n].*|^in[^s].*|^ins[^n].*|^insn[^-].*")

ADD_DEFINITIONS(-DIN_GCC -DHAVE_CONFIG_H -DGENERATOR_FILE)

SET(GEN_SRCS
  ${GCC_SOURCE_DIR}/gcc/rtl.c
  ${GCC_SOURCE_DIR}/gcc/bitmap.c
  ${GCC_SOURCE_DIR}/gcc/ggc-none.c
  ${GCC_SOURCE_DIR}/gcc/gensupport.c
  ${GCC_SOURCE_DIR}/libiberty/hashtab.c
  ${GCC_SOURCE_DIR}/libiberty/safe-ctype.c
  ${GCC_SOURCE_DIR}/libiberty/obstack.c
  ${GCC_SOURCE_DIR}/gcc/print-rtl.c
  ${GCC_SOURCE_DIR}/gcc/errors.c
)

SUBDIRS(gen1 gen2 gen3)
