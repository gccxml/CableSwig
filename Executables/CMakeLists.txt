# create the executable that runs cable-swig
ADD_EXECUTABLE( cswig CableSwig.cxx cableSwigMain.cxx)

TARGET_LINK_LIBRARIES(cswig
        modules11 
        cparse
        cpp
        swig
        doh
        CableGenerators
        CableParsers
        CableExpat 
        CxxTypes 
        )
ADD_DEFINITIONS(-D_cxx_STATIC)
# create the executable that generates instance index files
ADD_EXECUTABLE( cableidx cableidx.cxx)

TARGET_LINK_LIBRARIES(cableidx
        CableGenerators
        CableParsers
        CableExpat 
        CxxTypes 
        )