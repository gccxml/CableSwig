/* --- DECLARE_MODULE_INIT and CALL_MODULE_INIT definitions */
#ifdef SWIGPERL
#define CS_MODULE_INIT_DEFINED 1
#ifndef PERL_OBJECT
#ifndef MULTIPLICITY
#define SWIG_INIT_ARGS CV* cv;
#define SWIG_INIT_CALL_ARGS cv
#else
#define SWIG_INIT_ARGS pTHXo_ CV* cv
#define SWIG_INIT_CALL_ARGS cv
#endif
#else
#define SWIG_INIT_ARGS CV *cv, CPerlObj *
#define SWIG_INIT_CALL_ARGS cv, 0
#endif
#define DECLARE_MODULE_INIT(name, Capitalname) \
extern "C" int boot_##name( SWIG_INIT_ARGS );

#define CALL_MODULE_INIT(name, Capitalname) \
boot_##name( SWIG_INIT_CALL_ARGS );
#endif

#ifdef SWIG_TCL_INT
#define CS_MODULE_INIT_DEFINED 1
#define DECLARE_MODULE_INIT(name, Capitalname) \
extern "C" int Capitalname##_Init( Tcl_Interp* );
#define CALL_MODULE_INIT(name, Capitalname) \
Capitalname##_Init( interp );
#endif

#ifdef SWIGPYTHON
#define CS_MODULE_INIT_DEFINED 1
#define DECLARE_MODULE_INIT(name, Capitalname) \
extern "C" int init_##name( );
#define CALL_MODULE_INIT(name, Capitalname) \
init_##name( );
#endif

#ifndef CS_MODULE_INIT_DEFINED
#define DECLARE_MODULE_INIT(name, Capitalname)
#define CALL_MODULE_INIT(name, Capitalname)
#endif
