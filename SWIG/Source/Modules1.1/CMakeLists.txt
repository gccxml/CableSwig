SET(MODULE_SOURCES
main.cxx module.cxx emit.cxx overload.cxx lang.cxx 
typepass.cxx allocate.cxx browser.cxx contract.cxx 
swigmain.cxx tcl8.cxx python.cxx perl5.cxx guile.cxx 
ruby.cxx mzscheme.cxx java.cxx php4.cxx ocaml.cxx 
xml.cxx pike.cxx s-exp.cxx
)

ADD_LIBRARY(modules11 ${MODULE_SOURCES})
