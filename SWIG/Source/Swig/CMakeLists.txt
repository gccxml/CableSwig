SET(SRCS         wrapfunc.c naming.c tree.c stype.c typesys.c scanner.c include.c getopt.c misc.c 
		parms.c cwrap.c typemap.c warn.c symbol.c error.c fragment.c)
ADD_LIBRARY(swig ${SRCS})
