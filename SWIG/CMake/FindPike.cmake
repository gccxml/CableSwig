#
# This module finds if PIKE is installed and determines where the include files
# and libraries are. It also determines what the name of the library is. This
# code sets the following variables:
#
#  PIKE_INCLUDE_PATH       = path to where object.h can be found
#  PIKE_EXECUTABLE         = full path to the pike binary
#

SET(PIKE_POSSIBLE_INCLUDE_PATHS
  /usr/include
  /usr/include/pike
  /usr/include/pike/7.4.27
  /usr/local/include/pike/7.4.27
  )

FIND_PATH(PIKE_INCLUDE_PATH program.h
  ${PIKE_POSSIBLE_INCLUDE_PATHS})

FIND_PROGRAM(PIKE_EXECUTABLE
  NAMES pike7.4
  PATHS
  /usr/bin
  /usr/local/bin
  )

MARK_AS_ADVANCED(
  PIKE_EXECUTABLE
  PIKE_INCLUDE_PATH
  )
