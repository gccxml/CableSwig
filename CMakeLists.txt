CMAKE_MINIMUM_REQUIRED(VERSION 1.4)
PROJECT(GCC)
IF(APPLE)
  IF(CMAKE_COMPILER_IS_GNUCC)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-cpp-precomp")
  ENDIF(CMAKE_COMPILER_IS_GNUCC)
ENDIF(APPLE)
SUBDIRS(libiberty gen gcc)

INCLUDE(${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/CheckVariableExists.cmake)

CHECK_FUNCTION_EXISTS(__argz_count HAVE___ARGZ_COUNT)         # gcc
CHECK_FUNCTION_EXISTS(__argz_next HAVE___ARGZ_NEXT)           # gcc
CHECK_FUNCTION_EXISTS(__argz_stringify HAVE___ARGZ_STRINGIFY) # gcc
CHECK_FUNCTION_EXISTS(alloca HAVE_ALLOCA)                     # libiberty
CHECK_FUNCTION_EXISTS(asprintf HAVE_ASPRINTF)                 # libiberty
CHECK_FUNCTION_EXISTS(atexit HAVE_ATEXIT)                     # libiberty
CHECK_FUNCTION_EXISTS(atoll HAVE_ATOLL)                       # gcc
CHECK_FUNCTION_EXISTS(atoq HAVE_ATOQ)                         # gcc
CHECK_FUNCTION_EXISTS(basename HAVE_BASENAME)                 # libiberty
CHECK_FUNCTION_EXISTS(bcmp HAVE_BCMP)                         # libiberty
CHECK_FUNCTION_EXISTS(bcopy HAVE_BCOPY)                       # libiberty
CHECK_FUNCTION_EXISTS(bsearch HAVE_BSEARCH)                   # libiberty
CHECK_FUNCTION_EXISTS(bzero HAVE_BZERO)                       # libiberty
CHECK_FUNCTION_EXISTS(calloc HAVE_CALLOC)                     # libiberty
CHECK_FUNCTION_EXISTS(clock HAVE_CLOCK)                       # libiberty
CHECK_FUNCTION_EXISTS(dcgettext HAVE_DCGETTEXT)               # gcc
CHECK_FUNCTION_EXISTS(fputc_unlocked HAVE_FPUTC_UNLOCKED)     # gcc
CHECK_FUNCTION_EXISTS(fputs_unlocked HAVE_FPUTS_UNLOCKED)     # gcc
CHECK_FUNCTION_EXISTS(getcwd HAVE_GETCWD)                     # libiberty
CHECK_FUNCTION_EXISTS(getpagesize HAVE_GETPAGESIZE)           # libiberty
CHECK_FUNCTION_EXISTS(getrlimit HAVE_GETRLIMIT)               # gcc
CHECK_FUNCTION_EXISTS(getrusage HAVE_GETRUSAGE)               # libiberty
CHECK_FUNCTION_EXISTS(gettimeofday HAVE_GETTIMEOFDAY)         # libiberty
CHECK_FUNCTION_EXISTS(iconv HAVE_ICONV)                       # gcc
CHECK_FUNCTION_EXISTS(index HAVE_INDEX)                       # libiberty
CHECK_FUNCTION_EXISTS(insque HAVE_INSQUE)                     # libiberty
CHECK_FUNCTION_EXISTS(isascii HAVE_ISASCII)                   # gcc
CHECK_FUNCTION_EXISTS(kill HAVE_KILL)                         # gcc
CHECK_FUNCTION_EXISTS(lstat HAVE_LSTAT)                       # gcc
CHECK_FUNCTION_EXISTS(memchr HAVE_MEMCHR)                     # libiberty
CHECK_FUNCTION_EXISTS(memcmp HAVE_MEMCMP)                     # libiberty
CHECK_FUNCTION_EXISTS(memcpy HAVE_MEMCPY)                     # libiberty
CHECK_FUNCTION_EXISTS(memmove HAVE_MEMMOVE)                   # libiberty
CHECK_FUNCTION_EXISTS(memset HAVE_MEMSET)                     # libiberty
CHECK_FUNCTION_EXISTS(mkstemps HAVE_MKSTEMPS)                 # libiberty
CHECK_FUNCTION_EXISTS(mmap HAVE_MMAP)                         # libiberty
CHECK_FUNCTION_EXISTS(munmap HAVE_MUNMAP)                     # gcc
CHECK_FUNCTION_EXISTS(nl_langinfo HAVE_NL_LANGINFO)           # gcc
CHECK_FUNCTION_EXISTS(on_exit HAVE_ON_EXIT)                   # libiberty
CHECK_FUNCTION_EXISTS(popen HAVE_POPEN)                       # gcc
CHECK_FUNCTION_EXISTS(psignal HAVE_PSIGNAL)                   # libiberty
CHECK_FUNCTION_EXISTS(putc_unlocked HAVE_PUTC_UNLOCKED)       # gcc
CHECK_FUNCTION_EXISTS(putenv HAVE_PUTENV)                     # libiberty
CHECK_FUNCTION_EXISTS(random HAVE_RANDOM)                     # libiberty
CHECK_FUNCTION_EXISTS(rename HAVE_RENAME)                     # libiberty
CHECK_FUNCTION_EXISTS(rindex HAVE_RINDEX)                     # libiberty
CHECK_FUNCTION_EXISTS(sbrk HAVE_SBRK)                         # libiberty
CHECK_FUNCTION_EXISTS(setenv HAVE_SETENV)                     # libiberty
CHECK_FUNCTION_EXISTS(setlocale HAVE_SETLOCALE)               # gcc
CHECK_FUNCTION_EXISTS(setrlimit HAVE_SETRLIMIT)               # gcc
CHECK_FUNCTION_EXISTS(sigsetmask HAVE_SIGSETMASK)             # libiberty
CHECK_FUNCTION_EXISTS(stpcpy HAVE_STPCPY)                     # gcc
CHECK_FUNCTION_EXISTS(strcasecmp HAVE_STRCASECMP)             # libiberty
CHECK_FUNCTION_EXISTS(strchr HAVE_STRCHR)                     # libiberty
CHECK_FUNCTION_EXISTS(strdup HAVE_STRDUP)                     # libiberty
CHECK_FUNCTION_EXISTS(strerror HAVE_STRERROR)                 # libiberty
CHECK_FUNCTION_EXISTS(strncasecmp HAVE_STRNCASECMP)           # libiberty
CHECK_FUNCTION_EXISTS(strrchr HAVE_STRRCHR)                   # libiberty
CHECK_FUNCTION_EXISTS(strsignal HAVE_STRSIGNAL)               # libiberty
CHECK_FUNCTION_EXISTS(strstr HAVE_STRSTR)                     # libiberty
CHECK_FUNCTION_EXISTS(strtod HAVE_STRTOD)                     # libiberty
CHECK_FUNCTION_EXISTS(strtol HAVE_STRTOL)                     # libiberty
CHECK_FUNCTION_EXISTS(strtoul HAVE_STRTOUL)                   # libiberty
CHECK_FUNCTION_EXISTS(sysconf HAVE_SYSCONF)                   # libiberty
CHECK_FUNCTION_EXISTS(times HAVE_TIMES)                       # libiberty
CHECK_FUNCTION_EXISTS(tmpnam HAVE_TMPNAM)                     # libiberty
CHECK_FUNCTION_EXISTS(vasprintf HAVE_VASPRINTF)               # libiberty
CHECK_FUNCTION_EXISTS(vfprintf HAVE_VFPRINTF)                 # libiberty
CHECK_FUNCTION_EXISTS(vprintf HAVE_VPRINTF)                   # libiberty
CHECK_FUNCTION_EXISTS(vsprintf HAVE_VSPRINTF)                 # libiberty
CHECK_FUNCTION_EXISTS(waitpid HAVE_WAITPID)                   # libiberty
CHECK_INCLUDE_FILE(alloca.h HAVE_ALLOCA_H)                    # libiberty
CHECK_INCLUDE_FILE(argz.h HAVE_ARGZ_H)                        # gcc
CHECK_INCLUDE_FILE(direct.h HAVE_DIRECT_H)                    # gcc
CHECK_INCLUDE_FILE(fcntl.h HAVE_FCNTL_H)                      # libiberty
CHECK_INCLUDE_FILE(inttypes.h HAVE_INTTYPES_H)                # gcc
CHECK_INCLUDE_FILE(langinfo.h HAVE_LANGINFO_H)                # gcc
CHECK_INCLUDE_FILE(limits.h HAVE_LIMITS_H)                    # libiberty
CHECK_INCLUDE_FILE(locale.h HAVE_LOCALE_H)                    # gcc
CHECK_INCLUDE_FILE(malloc.h HAVE_MALLOC_H)                    # gcc
CHECK_INCLUDE_FILE(nl_types.h HAVE_NL_TYPES_H)                # gcc
CHECK_INCLUDE_FILE(stab.h HAVE_STAB_H)                        # gcc
CHECK_INCLUDE_FILE(stdbool.h HAVE_STDBOOL_H)                  # gcc
CHECK_INCLUDE_FILE(stddef.h HAVE_STDDEF_H)                    # gcc
CHECK_INCLUDE_FILE(stdlib.h HAVE_STDLIB_H)                    # libiberty
CHECK_INCLUDE_FILE(string.h HAVE_STRING_H)                    # libiberty
CHECK_INCLUDE_FILE(strings.h HAVE_STRINGS_H)                  # libiberty
CHECK_INCLUDE_FILE(sys/file.h HAVE_SYS_FILE_H)                # libiberty
CHECK_INCLUDE_FILE(sys/mman.h HAVE_SYS_MMAN_H)                # libiberty
CHECK_INCLUDE_FILE(sys/param.h HAVE_SYS_PARAM_H)              # libiberty
CHECK_INCLUDE_FILE(sys/resource.h HAVE_SYS_RESOURCE_H)        # libiberty
CHECK_INCLUDE_FILE(sys/stat.h HAVE_SYS_STAT_H)                # libiberty
CHECK_INCLUDE_FILE(sys/time.h HAVE_SYS_TIME_H)                # libiberty
CHECK_INCLUDE_FILE(sys/times.h HAVE_SYS_TIMES_H)              # gcc
CHECK_INCLUDE_FILE(sys/wait.h HAVE_SYS_WAIT_H)                # libiberty
CHECK_INCLUDE_FILE(time.h HAVE_TIME_H)                        # libiberty
CHECK_INCLUDE_FILE(unistd.h HAVE_UNISTD_H)                    # libiberty
CHECK_INCLUDE_FILE(vfork.h HAVE_VFORK_H)                      # libiberty
CHECK_VARIABLE_EXISTS(sys_errlist HAVE_SYS_ERRLIST)           # libiberty
CHECK_VARIABLE_EXISTS(sys_nerr HAVE_SYS_NERR)                 # libiberty
CHECK_VARIABLE_EXISTS(sys_siglist HAVE_SYS_SIGLIST)           # libiberty

IF(ENUM_BITFIELDS_ARE_UNSIGNED MATCHES "^ENUM_BITFIELDS_ARE_UNSIGNED$")
  MESSAGE(STATUS "Checking whether enum bitfields are unsigned")
  TRY_RUN(ENUM_BITFIELDS_ARE_UNSIGNED HAVE_ENUM_BITFIELDS_ARE_UNSIGNED
	  ${GCC_BINARY_DIR}
	  ${GCC_SOURCE_DIR}/EnumBitfieldsAreUnsigned.c
	  OUTPUT_VARIABLE OUTPUT)
  IF(HAVE_ENUM_BITFIELDS_ARE_UNSIGNED)
    IF(ENUM_BITFIELDS_ARE_UNSIGNED)
      MESSAGE(STATUS "Checking whether enum bitfields are unsigned - yes")
    ELSE(ENUM_BITFIELDS_ARE_UNSIGNED)
      MESSAGE(STATUS "Checking whether enum bitfields are unsigned - no")
    ENDIF(ENUM_BITFIELDS_ARE_UNSIGNED)
  ELSE(HAVE_ENUM_BITFIELDS_ARE_UNSIGNED)
    MESSAGE(STATUS "Checking whether enum bitfields are unsigned - failed")
    WRITE_FILE(${GCC_BINARY_DIR}/CMakeError.log 
      "Determining signedness of bit fields failed with the following output:\n${OUTPUT}\n"
      APPEND)
  ENDIF(HAVE_ENUM_BITFIELDS_ARE_UNSIGNED)
ENDIF(ENUM_BITFIELDS_ARE_UNSIGNED MATCHES "^ENUM_BITFIELDS_ARE_UNSIGNED$")

IF(TIME_WITH_SYS_TIME MATCHES "^TIME_WITH_SYS_TIME$")
  MESSAGE(STATUS "Checking include of time.h and sys/time.h together")
  TRY_COMPILE(TIME_WITH_SYS_TIME
             ${GCC_BINARY_DIR}
             ${GCC_SOURCE_DIR}/CheckTimeWithSysTime.c
             OUTPUT_VARIABLE OUTPUT)
  IF(TIME_WITH_SYS_TIME)
    MESSAGE(STATUS "Checking include of time.h and sys/time.h together - passed")
    SET(TIME_WITH_SYS_TIME 1 CACHE INTERNAL "time.h and sys/time.h work together")
  ELSE(TIME_WITH_SYS_TIME)
    MESSAGE(STATUS "Checking include of time.h and sys/time.h together - passed")
    SET(TIME_WITH_SYS_TIME "" CACHE INTERNAL "time.h and sys/time.h work together")
    WRITE_FILE(${GCC_BINARY_DIR}/CMakeError.log 
      "Checking include of time.h and sys/time.h together "
      "failed with the following output:\n"
      "${OUTPUT}\n" APPEND)
  ENDIF(TIME_WITH_SYS_TIME)
ENDIF(TIME_WITH_SYS_TIME MATCHES "^TIME_WITH_SYS_TIME$")

IF(STRING_WITH_STRINGS MATCHES "^STRING_WITH_STRINGS$")
  MESSAGE(STATUS "Checking include of string.h and strings.h together")
  TRY_COMPILE(STRING_WITH_STRINGS
             ${GCC_BINARY_DIR}
             ${GCC_SOURCE_DIR}/CheckStringWithStrings.c
             OUTPUT_VARIABLE OUTPUT)
  IF(STRING_WITH_STRINGS)
    MESSAGE(STATUS "Checking include of string.h and strings.h together - passed")
    SET(STRING_WITH_STRINGS 1 CACHE INTERNAL "string.h and strings.h work together")
  ELSE(STRING_WITH_STRINGS)
    MESSAGE(STATUS "Checking include of string.h and strings.h together - failed")
    SET(STRING_WITH_STRINGS "" CACHE INTERNAL "string.h and strings.h work together")
    WRITE_FILE(${GCC_BINARY_DIR}/CMakeError.log 
      "Checking include of string.h and strings.h together "
      "failed with the following output:\n"
      "${OUTPUT}\n" APPEND)
  ENDIF(STRING_WITH_STRINGS)
ENDIF(STRING_WITH_STRINGS MATCHES "^STRING_WITH_STRINGS$")

IF(HAVE_DECL_ENVIRON MATCHES "^HAVE_DECL_ENVIRON$")
  MESSAGE(STATUS "Checking whether environ is declared")
  TRY_COMPILE(HAVE_DECL_ENVIRON 
	  ${GCC_BINARY_DIR}
	  ${GCC_SOURCE_DIR}/CheckEnviron.c
	  OUTPUT_VARIABLE OUTPUT)
  IF(HAVE_DECL_ENVIRON)
    MESSAGE(STATUS "Checking whether environ is declared - yes")
    SET(HAVE_DECL_ENVIRON 1 CACHE INTERNAL "environ is declared")
  ELSE(HAVE_DECL_ENVIRON)
    MESSAGE(STATUS "Checking whether environ is declared - no")
    SET(HAVE_DECL_ENVIRON "" CACHE INTERNAL "environ is declared")
    WRITE_FILE(${GCC_BINARY_DIR}/CMakeError.log 
      "Checking if environ is declared "
      "failed with the following output:\n"
      "${OUTPUT}\n" APPEND)
  ENDIF(HAVE_DECL_ENVIRON)
ENDIF(HAVE_DECL_ENVIRON MATCHES "^HAVE_DECL_ENVIRON$")

CONFIGURE_FILE(${GCC_SOURCE_DIR}/gcc/auto-host.h.in
               ${GCC_BINARY_DIR}/gcc/auto-host.h)
CONFIGURE_FILE(${GCC_SOURCE_DIR}/gcc/configargs.h.in
               ${GCC_BINARY_DIR}/gcc/configargs.h)
CONFIGURE_FILE(${GCC_SOURCE_DIR}/gcc/libintl.h.in
               ${GCC_BINARY_DIR}/gcc/libintl.h)
CONFIGURE_FILE(${GCC_SOURCE_DIR}/gcc/gencheck.h.in
               ${GCC_BINARY_DIR}/gcc/gencheck.h)
CONFIGURE_FILE(${GCC_SOURCE_DIR}/gcc/options.h.in
               ${GCC_BINARY_DIR}/gcc/options.h)
CONFIGURE_FILE(${GCC_SOURCE_DIR}/gcc/specs.h.in
               ${GCC_BINARY_DIR}/gcc/specs.h)
CONFIGURE_FILE(${GCC_SOURCE_DIR}/gcc/config.h.in
               ${GCC_BINARY_DIR}/gcc/config.h)
CONFIGURE_FILE(${GCC_SOURCE_DIR}/gcc/hconfig.h.in
               ${GCC_BINARY_DIR}/gcc/hconfig.h)
CONFIGURE_FILE(${GCC_SOURCE_DIR}/gcc/tm.h.in
               ${GCC_BINARY_DIR}/gcc/tm.h)
CONFIGURE_FILE(${GCC_SOURCE_DIR}/gcc/tm_p.h.in
               ${GCC_BINARY_DIR}/gcc/tm_p.h)
CONFIGURE_FILE(${GCC_SOURCE_DIR}/libiberty/config.h.in
               ${GCC_BINARY_DIR}/libiberty/config.h)

UTILITY_SOURCE(GCC_gengenrtl_EXE gengenrtl gen/gen1)
MARK_AS_ADVANCED(GCC_gengenrtl_EXE)

FOREACH(g
    gencodes
    genflags
    gencheck
    genattr
    genconfig
    genpeep
    genopinit
    genoutput
    genemit
    genattrtab
  )
  UTILITY_SOURCE(GCC_${g}_EXE ${g} gen/gen2)
  MARK_AS_ADVANCED(GCC_${g}_EXE)
ENDFOREACH(g)

FOREACH(g
    genrecog
    genextract
  )
  UTILITY_SOURCE(GCC_${g}_EXE ${g} gen/gen3)
  MARK_AS_ADVANCED(GCC_${g}_EXE)
ENDFOREACH(g)
