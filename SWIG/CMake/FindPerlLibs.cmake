#
# This module finds if PERL is installed and determines where the include files
# and libraries are. It also determines what the name of the library is. This
# code sets the following variables:
#
#  PERL_INCLUDE_PATH       = path to where object.h can be found
#  PERL_EXECUTABLE         = full path to the perl binary
#

SET(PERL_POSSIBLE_INCLUDE_PATHS
  c:/Perl/lib/CORE
  /Perl/lib/CORE
  /usr/lib/perl/5.8.0/CORE
  /usr/lib/perl/5.8.2/CORE
  )

SET(PERL_POSSIBLE_LIB_PATHS
  c:/Perl/lib/CORE
  /Perl/lib/CORE
  /Perl/lib
  /usr/lib
  )

FIND_PATH(PERL_INCLUDE_PATH perl.h
  ${PERL_POSSIBLE_INCLUDE_PATHS})


FIND_PROGRAM(PERL_EXECUTABLE
  NAMES perl
  PATHS
  c:/Perl/bin
  /Perl/bin
  /usr/bin
  /usr/local/bin
  )


FIND_LIBRARY(PERL_LIBRARY
  NAMES perl perl5.8.0 perl58
  PATHS ${PERL_POSSIBLE_LIB_PATHS}
  )

MARK_AS_ADVANCED(
  PERL_INCLUDE_PATH
  PERL_EXECUTABLE
  PERL_LIBRARY
  )
