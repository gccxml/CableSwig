#
# This module finds if GUILE is installed and determines where the include files
# and libraries are. It also determines what the name of the library is. This
# code sets the following variables:
#
#  GUILE_INCLUDE_PATH       = path to where object.h can be found
#  GUILE_EXECUTABLE         = full path to the guile binary
#

SET(GUILE_POSSIBLE_INCLUDE_PATHS
  /usr/include
  )

SET(GUILE_POSSIBLE_LIB_PATHS
  /usr/lib
  )

FIND_PATH(GUILE_INCLUDE_PATH guile/gh.h
  ${GUILE_POSSIBLE_INCLUDE_PATHS})

FIND_LIBRARY(GUILE_LIBRARY
  NAMES guile
  PATHS ${GUILE_POSSIBLE_LIB_PATHS}
  )

FIND_PROGRAM(GUILE_EXECUTABLE
  NAMES guile1.4
  PATHS
  /usr/bin
  /usr/local/bin
  )

MARK_AS_ADVANCED(
  GUILE_INCLUDE_PATH
  GUILE_LIBRARY
  GUILE_EXECUTABLE
  )
