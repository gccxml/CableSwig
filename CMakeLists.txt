# Project name and version.
PROJECT (CABLE)
SET (CABLE_VERSION_STRING "0.05")

# Limit dependencies in UNIX.
INCLUDE_REGULAR_EXPRESSION ("^(cxx|cable|ct|CxxTypes).*$")

# Build the standard components.
SUBDIRS (Expat CxxTypes Parsers Generators Executables)

# Optionally build the Tcl wrapper facility.
OPTION (CABLE_TCL_FACILITY "Build CABLE's Tcl wrapping facility." ON)
IF (CABLE_TCL_FACILITY)
  SUBDIRS(CableTclFacility)
ENDIF (CABLE_TCL_FACILITY)

# Configure the CABLE version number in the build.
CONFIGURE_FILE (${CABLE_SOURCE_DIR}/cableVersion.h.in
                ${CABLE_BINARY_DIR}/cableVersion.h IMMEDIATE)

# Need to include generated headers.
INCLUDE_DIRECTORIES (${CABLE_BINARY_DIR})

# Make a distinct copy of the executable and library paths so another
# program's LOAD_CACHE command can get them.
SET (CABLE_EXECUTABLE_PATH ${EXECUTABLE_OUTPUT_PATH}
     CACHE INTERNAL "Single output directory for building all executables.")
SET (CABLE_LIBRARY_PATH ${LIBRARY_OUTPUT_PATH}
     CACHE INTERNAL "Single output directory for building all libraries.")
