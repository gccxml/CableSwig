/* -----------------------------------------------------------------------------
 * swigmod.h
 *
 *     Main header file for SWIG modules
 *
 * Author(s) : David Beazley (beazley@cs.uchicago.edu)
 *
 * Copyright (C) 1998-2000.  The University of Chicago
 * Copyright (C) 1995-1998.  The University of Utah and The Regents of the
 *                           University of California.
 *
 * See the file LICENSE for information on usage and redistribution.
 *
 * Header
 * ----------------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "swigver.h"

extern "C" {
#include "swig.h"
extern Hash  *Preprocessor_define(const String_or_char *str, int swigmacro);
}

#include "swigwarn.h"

#define NOT_VIRTUAL     0
#define PLAIN_VIRTUAL   1
#define PURE_VIRTUAL    2

extern  char     *input_file;
extern  int       line_number;
extern  int       start_line;
extern  int       CPlusPlus;                        // C++ mode
extern  int       Extend;                           // Extend mode
extern  int       NoInclude;                        // NoInclude flag
extern  int       Verbose;
extern  int       IsVirtual;
extern  int       ImportMode;
extern  int       NoExcept;                         // -no_except option

/* Miscellaneous stuff */

#define  tab2   "  "
#define  tab4   "    "
#define  tab8   "        "

class Dispatcher {
 public:
  
  virtual int emit_one(Node *n);
  virtual int emit_children(Node *n);
  virtual int defaultHandler(Node *n);

  /* Top of the parse tree */
  virtual int  top(Node *n) = 0;
  
  /* SWIG directives */

  virtual int applyDirective(Node *n);
  virtual int clearDirective(Node *n);
  virtual int constantDirective(Node *n);
  virtual int extendDirective(Node *n);
  virtual int fragmentDirective(Node *n);
  virtual int importDirective(Node *n);
  virtual int includeDirective(Node *n);
  virtual int insertDirective(Node *n);
  virtual int moduleDirective(Node *n);
  virtual int nativeDirective(Node *n);
  virtual int pragmaDirective(Node *n);
  virtual int typemapDirective(Node *n);
  virtual int typemapitemDirective(Node *n);
  virtual int typemapcopyDirective(Node *n);
  virtual int typesDirective(Node *n);

  /* C/C++ parsing */
  
  virtual int cDeclaration(Node *n);
  virtual int externDeclaration(Node *n);
  virtual int enumDeclaration(Node *n);
  virtual int enumvalueDeclaration(Node *n);
  virtual int classDeclaration(Node *n);
  virtual int classforwardDeclaration(Node *n);
  virtual int constructorDeclaration(Node *n);
  virtual int destructorDeclaration(Node *n);
  virtual int accessDeclaration(Node *n);
  virtual int usingDeclaration(Node *n);
  virtual int namespaceDeclaration(Node *n);
  virtual int templateDeclaration(Node *n);
};

/************************************************************************
 * class language:
 *
 * This class defines the functions that need to be supported by the
 * scripting language being used.    The translator calls these virtual
 * functions to output different types of code for different languages.
 *************************************************************************/

class Language : public Dispatcher {
public:
  Language();
  virtual ~Language();
  virtual int emit_one(Node *n);

  /* Parse command line options */

  virtual void main(int argc, char *argv[]);

  /* Top of the parse tree */

  virtual int  top(Node *n);
  
  /* SWIG directives */
  

  virtual int applyDirective(Node *n);
  virtual int clearDirective(Node *n);
  virtual int constantDirective(Node *n);
  virtual int extendDirective(Node *n);
  virtual int fragmentDirective(Node *n);
  virtual int importDirective(Node *n);
  virtual int includeDirective(Node *n);
  virtual int insertDirective(Node *n);
  virtual int moduleDirective(Node *n);
  virtual int nativeDirective(Node *n);
  virtual int pragmaDirective(Node *n);
  virtual int typemapDirective(Node *n);
  virtual int typemapcopyDirective(Node *n);
  virtual int typesDirective(Node *n);

  /* C/C++ parsing */
  
  virtual int cDeclaration(Node *n);
  virtual int externDeclaration(Node *n);
  virtual int enumDeclaration(Node *n);
  virtual int enumvalueDeclaration(Node *n);
  virtual int classDeclaration(Node *n);
  virtual int classforwardDeclaration(Node *n);
  virtual int constructorDeclaration(Node *n);
  virtual int destructorDeclaration(Node *n);
  virtual int accessDeclaration(Node *n);
  virtual int namespaceDeclaration(Node *n);
  virtual int usingDeclaration(Node *n);

  /* Function handlers */

  virtual int functionHandler(Node *n);
  virtual int globalfunctionHandler(Node *n);
  virtual int memberfunctionHandler(Node *n);
  virtual int staticmemberfunctionHandler(Node *n);
  virtual int callbackfunctionHandler(Node *n);

  /* Variable handlers */

  virtual int variableHandler(Node *n);
  virtual int globalvariableHandler(Node *n);
  virtual int membervariableHandler(Node *n);
  virtual int staticmembervariableHandler(Node *n);

  /* C++ handlers */

  virtual int memberconstantHandler(Node *n);
  virtual int constructorHandler(Node *n);
  virtual int copyconstructorHandler(Node *n);
  virtual int destructorHandler(Node *n);
  virtual int classHandler(Node *n);

  /* Miscellaneous */

  virtual int typedefHandler(Node *n);

  /* Low-level code generation */

  virtual int constantWrapper(Node *n);
  virtual int variableWrapper(Node *n);
  virtual int functionWrapper(Node *n);
  virtual int nativeWrapper(Node *n);

  /* Miscellaneous */

  virtual  int  validIdentifier(String *s);        /* valid identifier? */
  virtual  int  addSymbol(String *s, Node *n);     /* Add symbol        */
  virtual  Node *symbolLookup(String *s);          /* Symbol lookup     */
  virtual  Node *classLookup(SwigType *s);         /* Class lookup      */

 protected:
  /* Patch C++ pass-by-value */
  static void patch_parms(Parm *p);

  /* Allow overloaded functions */
  void   allow_overloading(int val = 1);

  /* Allow multiple-input typemaps */
  void   allow_multiple_input(int val = 1);

  /* Wrapping class query */
  int is_wrapping_class();

  /* Return the node for the current class */
  Node *getCurrentClass() const;
    
  /* Return the real name of the current class */
  String *getClassName() const;
  
  /* Return the current class prefix */
  String *getClassPrefix() const;

  /* Fully qualified type name to use */
  String *getClassType() const;

 private:
  Hash   *symbols;
  Hash   *classtypes;
  int     overloading;
  int     multiinput;
};

extern  int   SWIG_main(int, char **, Language *);
extern  void  emit_args(SwigType *, ParmList *, Wrapper *f);
extern  void  SWIG_exit(int);           /* use EXIT_{SUCCESS,FAILURE} */
extern  void  SWIG_config_file(const String_or_char *);
extern  void  SWIG_config_cppext(const char *ext);

extern "C"    void  SWIG_typemap_lang(const char *);
extern void   SWIG_library_directory(const char *);
extern int    emit_num_arguments(ParmList *);
extern int    emit_num_required(ParmList *);
extern int    emit_isvarargs(ParmList *);
extern void   emit_attach_parmmaps(ParmList *, Wrapper *f);
extern void   emit_action(Node *n, Wrapper *f);
extern List  *Swig_overload_rank(Node *n);
extern String *Swig_overload_dispatch(Node *n, const String_or_char *fmt, int *);


extern "C" {
  typedef Language *(*ModuleFactory)(void);
}

extern void   Swig_register_module(const char *name, ModuleFactory fac);
extern ModuleFactory Swig_find_module(const char *name);

/* swig11.h ends here */









