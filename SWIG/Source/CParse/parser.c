
/*  A Bison parser, made from parser.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	ID	258
#define	HBLOCK	259
#define	POUND	260
#define	STRING	261
#define	INCLUDE	262
#define	IMPORT	263
#define	INSERT	264
#define	CHARCONST	265
#define	NUM_INT	266
#define	NUM_FLOAT	267
#define	NUM_UNSIGNED	268
#define	NUM_LONG	269
#define	NUM_ULONG	270
#define	NUM_LONGLONG	271
#define	NUM_ULONGLONG	272
#define	TYPEDEF	273
#define	TYPE_INT	274
#define	TYPE_UNSIGNED	275
#define	TYPE_SHORT	276
#define	TYPE_LONG	277
#define	TYPE_FLOAT	278
#define	TYPE_DOUBLE	279
#define	TYPE_CHAR	280
#define	TYPE_VOID	281
#define	TYPE_SIGNED	282
#define	TYPE_BOOL	283
#define	TYPE_TYPEDEF	284
#define	TYPE_RAW	285
#define	LPAREN	286
#define	RPAREN	287
#define	COMMA	288
#define	SEMI	289
#define	EXTERN	290
#define	INIT	291
#define	LBRACE	292
#define	RBRACE	293
#define	PERIOD	294
#define	CONST	295
#define	VOLATILE	296
#define	STRUCT	297
#define	UNION	298
#define	EQUAL	299
#define	SIZEOF	300
#define	MODULE	301
#define	LBRACKET	302
#define	RBRACKET	303
#define	ILLEGAL	304
#define	CONSTANT	305
#define	NAME	306
#define	RENAME	307
#define	NAMEWARN	308
#define	EXTEND	309
#define	PRAGMA	310
#define	FEATURE	311
#define	VARARGS	312
#define	ENUM	313
#define	CLASS	314
#define	TYPENAME	315
#define	PRIVATE	316
#define	PUBLIC	317
#define	PROTECTED	318
#define	COLON	319
#define	STATIC	320
#define	VIRTUAL	321
#define	FRIEND	322
#define	THROW	323
#define	CATCH	324
#define	USING	325
#define	NAMESPACE	326
#define	NATIVE	327
#define	INLINE	328
#define	TYPEMAP	329
#define	EXCEPT	330
#define	ECHO	331
#define	APPLY	332
#define	CLEAR	333
#define	SWIGTEMPLATE	334
#define	FRAGMENT	335
#define	WARN	336
#define	LESSTHAN	337
#define	GREATERTHAN	338
#define	MODULO	339
#define	DELETE	340
#define	TYPES	341
#define	PARMS	342
#define	NONID	343
#define	DSTAR	344
#define	DCNOT	345
#define	TEMPLATE	346
#define	OPERATOR	347
#define	COPERATOR	348
#define	PARSETYPE	349
#define	CAST	350
#define	LOR	351
#define	LAND	352
#define	OR	353
#define	XOR	354
#define	AND	355
#define	LSHIFT	356
#define	RSHIFT	357
#define	PLUS	358
#define	MINUS	359
#define	STAR	360
#define	SLASH	361
#define	UMINUS	362
#define	NOT	363
#define	LNOT	364
#define	DCOLON	365

#line 1 "parser.y"

/* -----------------------------------------------------------------------------
 * parser.y
 *
 *     YACC parser for SWIG.   The grammar is a somewhat broken subset of C/C++.
 *     This file is a bit of a mess and probably needs to be rewritten at
 *     some point.  Beware.
 *
 * Author(s) : David Beazley (beazley@cs.uchicago.edu)
 *
 * Copyright (C) 1998-2001.  The University of Chicago
 * Copyright (C) 1995-1998.  The University of Utah and The Regents of the
 *                           University of California.
 *
 * See the file LICENSE for information on usage and redistribution.
 * ----------------------------------------------------------------------------- */

#define yylex yylex

char cvsroot_parser_y[] = "Header";

#include "cparse.h"
#include "preprocessor.h"
#include <ctype.h>

/* We do this for portability */
#undef alloca
#define alloca malloc

/* -----------------------------------------------------------------------------
 *                               Externals
 * ----------------------------------------------------------------------------- */

extern int   yylex();
extern void  yyerror (const char *s);

/* scanner.cxx */

extern int  cparse_line;
extern int  cparse_start_line;
extern void skip_balanced(int startchar, int endchar);
extern void skip_decl(void);
extern void scanner_check_typedef(void);
extern void scanner_ignore_typedef(void);
extern void scanner_last_id(int);
extern void scanner_clear_rename(void);
extern void start_inline(char *, int);
extern String *scanner_ccode;
extern int Swig_cparse_template_expand(Node *n, String *rname, ParmList *tparms);
extern Node *Swig_cparse_template_locate(String *name, ParmList *tparms);

/* NEW Variables */

extern void generate_all(Node *);

static Node    *top = 0;      /* Top of the generated parse tree */
static int      unnamed = 0;  /* Unnamed datatype counter */
static Hash    *extendhash = 0;     /* Hash table of added methods */
static Hash    *classes = 0;        /* Hash table of classes */
static Symtab  *prev_symtab = 0;
static Node    *current_class = 0;
       String  *ModuleName = 0;
static Node    *module_node = 0;
static String  *Classprefix = 0;  
static String  *Namespaceprefix = 0;
static int      inclass = 0;
static char    *last_cpptype = 0;
static int      inherit_list = 0;
static Parm    *template_parameters = 0;

/* -----------------------------------------------------------------------------
 *                            Assist Functions
 * ----------------------------------------------------------------------------- */

static Node *new_node(const String_or_char *tag) {
  Node *n = NewHash();
  set_nodeType(n,tag);
  Setfile(n,cparse_file);
  Setline(n,cparse_line);
  return n;
}

/* Copies a node.  Does not copy tree links or symbol table data (except for
   sym:name) */

static Node *copy_node(Node *n) {
  Node *nn;
  String *key;
  nn = NewHash();
  Setfile(nn,Getfile(n));
  Setline(nn,Getline(n));
  for (key = Firstkey(n); key; key = Nextkey(n)) {
    if ((Strcmp(key,"nextSibling") == 0) ||
	(Strcmp(key,"previousSibling") == 0) ||
	(Strcmp(key,"parentNode") == 0) ||
	(Strcmp(key,"lastChild") == 0)) {
      continue;
    }
    if (Strncmp(key,"csym:",5) == 0) continue;
    /* We do copy sym:name.  For templates */
    if ((Strcmp(key,"sym:name") == 0) || 
	(Strcmp(key,"sym:weak") == 0) ||
	(Strcmp(key,"sym:typename") == 0)) {
      Setattr(nn,key, Copy(Getattr(n,key)));
      continue;
    }
    if (Strcmp(key,"sym:symtab") == 0) {
      Setattr(nn,"sym:needs_symtab", "1");
    }
    /* We don't copy any other symbol table attributes */
    if (Strncmp(key,"sym:",4) == 0) {
      continue;
    }
    /* If children.  We copy them recursively using this function */
    if (Strcmp(key,"firstChild") == 0) {
      /* Copy children */
      Node *cn = Getattr(n,key);
      while (cn) {
	appendChild(nn,copy_node(cn));
	cn = nextSibling(cn);
      }
      continue;
    }
    /* We don't copy the symbol table.  But we drop an attribute 
       requires_symtab so that functions know it needs to be built */

    if (Strcmp(key,"symtab") == 0) {
      /* Node defined a symbol table. */
      Setattr(nn,"requires_symtab","1");
      continue;
    }
    /* Can't copy nodes */
    if (Strcmp(key,"node") == 0) {
      continue;
    }
    if ((Strcmp(key,"parms") == 0) || (Strcmp(key,"pattern") == 0) || (Strcmp(key,"throws") == 0))  {
      Setattr(nn,key,CopyParmList(Getattr(n,key)));
      continue;
    }
    /* Looks okay.  Just copy the data using Copy */
    Setattr(nn, key, Copy(Getattr(n,key)));
  }
  return nn;
}

/* -----------------------------------------------------------------------------
 *                              Variables
 * ----------------------------------------------------------------------------- */

      char  *typemap_lang = 0;    /* Current language setting */

static int cplus_mode  = 0;
static String  *class_rename = 0;

/* C++ modes */

#define  CPLUS_PUBLIC    1
#define  CPLUS_PRIVATE   2
#define  CPLUS_PROTECTED 3

void SWIG_typemap_lang(const char *tm_lang) {
  typemap_lang = Swig_copy_string(tm_lang);
}

/* -----------------------------------------------------------------------------
 *                           Assist functions
 * ----------------------------------------------------------------------------- */

/* Perform type-promotion for binary operators */
static int promote(int t1, int t2) {
  return t1 > t2 ? t1 : t2;
}

static String *yyrename = 0;

/* Forward renaming operator */
static Hash   *rename_hash = 0;
static Hash   *namewarn_hash = 0;
static Hash   *features_hash = 0;

static String *feature_identifier_fix(String *s) {
  if (SwigType_istemplate(s)) {
    String *tp, *ts, *ta, *tq;
    tp = SwigType_templateprefix(s);
    ts = SwigType_templatesuffix(s);
    ta = SwigType_templateargs(s);
    tq = Swig_symbol_type_qualify(ta,0);
    Append(tp,tq);
    Append(tp,ts);
    Delete(ts);
    Delete(ta);
    Delete(tq);
    return tp;
  } else {
    return NewString(s);
  }
}

static void
rename_add(char *name, SwigType *decl, char *newname) {
  String *nname;
  if (!rename_hash) rename_hash = NewHash();
  if (Namespaceprefix) {
    nname = NewStringf("%s::%s",Namespaceprefix, name);
  } else {
    nname = NewString(name);
  }
  Swig_name_object_set(rename_hash,nname,decl,NewString(newname));
  Delete(nname);
}

static void
namewarn_add(char *name, SwigType *decl, char *warning) {
  String *nname;
  if (!namewarn_hash) namewarn_hash = NewHash();
  if (Namespaceprefix) {
    nname = NewStringf("%s::%s",Namespaceprefix, name);
  } else {
    nname = NewString(name);
  }

  Swig_name_object_set(namewarn_hash,nname,decl,NewString(warning));
  Delete(nname);
}

static void
rename_inherit(String *base, String *derived) {
  /*  Printf(stdout,"base = '%s', derived = '%s'\n", base, derived); */
  Swig_name_object_inherit(rename_hash,base,derived);
  Swig_name_object_inherit(namewarn_hash,base,derived);
  Swig_name_object_inherit(features_hash,base,derived);
}

/* Generate the symbol table name for an object */
/* This is a bit of a mess. Need to clean up */
static String *add_oldname = 0;

static String *make_name(String *name,SwigType *decl) {
  String *rn = 0;
  String *origname = name;
  int     destructor = 0;

  if (name && (*(Char(name)) == '~')) {
    destructor = 1;
  }
  if (yyrename) {
    String *s = yyrename;
    yyrename = 0;
    if (destructor) {
      Insert(s,0,"~");
    }
    return s;
  }
  if (!name) return 0;
  /* Check to see if the name is in the hash */
  if (!rename_hash) {
    if (add_oldname) return Copy(add_oldname);
    return origname;
  }
  rn = Swig_name_object_get(rename_hash, Namespaceprefix, name, decl);
  if (!rn) {
    if (add_oldname) return Copy(add_oldname);
    return name;
  }
  if (destructor) {
    if (Strcmp(rn,"$ignore") != 0) {
      String *s = NewStringf("~%s", rn);
      return s;
    }
  }
  return Copy(rn);
}

/* Generate an unnamed identifier */
static String *make_unnamed() {
  unnamed++;
  return NewStringf("$unnamed%d$",unnamed);
}

/* Generate the symbol table name for an object */
static String *name_warning(String *name,SwigType *decl) {
  String *rn = 0;
  if (!name) return 0;

  /* Check to see if the name is in the hash */
  if (!namewarn_hash) return 0;
  rn = Swig_name_object_get(namewarn_hash, Namespaceprefix,name,decl);
  if (!rn) return 0;
  return rn;
}

/* Add declaration list to symbol table */
static int  add_only_one = 0;


static void add_symbols(Node *n) {
  String *decl;
  String *wrn = 0;

  /* Don't add symbols for private/protected members */
  if (inclass && (cplus_mode != CPLUS_PUBLIC)) {
    while (n) {
      Swig_symbol_add(0, n);       /* Add to C symbol table */
      if (cplus_mode == CPLUS_PRIVATE) {
	Setattr(n,"access", "private");
      } else {
	Setattr(n,"access", "protected");
      }
      if (add_only_one) break;
      n = nextSibling(n);
    }
    return;
  }
  while (n) {
    String *symname;
    if (Getattr(n,"sym:name")) {
      n = nextSibling(n);
      continue;
    }
    decl = Getattr(n,"decl");
    if (!SwigType_isfunction(decl)) {
      symname = make_name(Getattr(n,"name"),0);
      if (!symname) {
	symname = Getattr(n,"unnamed");
      }
      if (symname) {
	wrn = name_warning(symname,0);
	Swig_features_get(features_hash, Namespaceprefix, Getattr(n,"name"), 0, n);
      }
    } else {
      SwigType *fdecl = Copy(decl);
      SwigType *fun = SwigType_pop_function(fdecl);
      symname = make_name(Getattr(n,"name"),fun);
      wrn = name_warning(symname,fun);
      
      Swig_features_get(features_hash,Namespaceprefix,Getattr(n,"name"),fun,n);
      Delete(fdecl);
      Delete(fun);
    }
    if (!symname) {
      n = nextSibling(n);
      continue;
    }
    if (strncmp(Char(symname),"$ignore",7) == 0) {
      char *c = Char(symname)+7;
      Setattr(n,"feature:ignore","1");
      if (strlen(c)) {
	Swig_warning(0,Getfile(n), Getline(n), "%s\n",c+1);
      }
      Swig_symbol_add(0, n);
    } else {
      Node *c;
      if ((wrn) && (Len(wrn))) {
	Swig_warning(0,Getfile(n),Getline(n), "%s\n", wrn);
      }
      if (Strcmp(nodeType(n),"enum") != 0) {
	c = Swig_symbol_add(symname,n);
	if (c != n) {
	  if (Getattr(n,"sym:weak")) {
	    Setattr(n,"sym:name",symname);
	  } else if ((Strcmp(nodeType(n),"template") == 0) && (Strcmp(Getattr(n,"templatetype"),"cdecl") == 0)) {
	    Setattr(n,"sym:name",symname);
	  } else {
	    String *e = NewString("");
	    Printf(e,"Identifier '%s' redeclared (ignored).", symname);
	    if (Cmp(symname,Getattr(n,"name"))) {
	      Printf(e," (Renamed from '%s')", SwigType_namestr(Getattr(n,"name")));
	    }
	    Printf(e,"\n%s:%d: Previous declaration of '%s'", Getfile(c),Getline(c),symname);
	    if (Cmp(symname,Getattr(c,"name"))) {
	      Printf(e," (Renamed from '%s')", SwigType_namestr(Getattr(c,"name")));
	    }
	    Swig_warning(WARN_PARSE_REDEFINED,Getfile(n), Getline(n),"%s\n", e);
	    Setattr(n,"error",e);
	  }
	}
      } else {
	Setattr(n,"sym:name", symname);
      }
    }
    if (add_only_one) return;
    n = nextSibling(n);
  }
}


/* add symbols a parse tree node copy */

void add_symbols_copy(Node *n) {
  String *name;
  int    emode = 0;

  while (n) {

    if (Strcmp(nodeType(n),"access") == 0) {
      String *kind = Getattr(n,"kind");
      if (Strcmp(kind,"public") == 0) {
	cplus_mode = CPLUS_PUBLIC;
      } else if (Strcmp(kind,"private") == 0) {
	cplus_mode = CPLUS_PRIVATE;
      } else if (Strcmp(kind,"protected") == 0) {
	cplus_mode = CPLUS_PROTECTED;
      }
      n = nextSibling(n);
      continue;
    }

    add_oldname = Getattr(n,"sym:name");
    if ((add_oldname) || (Getattr(n,"sym:needs_symtab"))) {
      if (add_oldname) {
	DohIncref(add_oldname);
	/* If already renamed, we used that name */
	if (Strcmp(add_oldname, Getattr(n,"name")) != 0) {
	  yyrename = add_oldname;
	}
      }
      Delattr(n,"sym:needs_symtab");
      Delattr(n,"sym:name");

      add_only_one = 1;
      add_symbols(n);

      if (Getattr(n,"partialargs")) {
	Swig_symbol_cadd(Getattr(n,"partialargs"),n);
      }
      add_only_one = 0;
      name = Getattr(n,"name");
      if (Getattr(n,"requires_symtab")) {
	Swig_symbol_newscope();
	Swig_symbol_setscopename(name);
	Namespaceprefix = Swig_symbol_qualifiedscopename(0);
      }
      if (Strcmp(nodeType(n),"class") == 0) {
	inclass = 1;
	if (Strcmp(Getattr(n,"kind"),"class") == 0) {
	  cplus_mode = CPLUS_PRIVATE;
	} else {
	  cplus_mode = CPLUS_PUBLIC;
	}
      }
      if (Strcmp(nodeType(n),"extend") == 0) {
	emode = cplus_mode;
	cplus_mode = CPLUS_PUBLIC;
      }
      add_symbols_copy(firstChild(n));
      if (Strcmp(nodeType(n),"extend") == 0) {
	cplus_mode = emode;
      }
      if (Getattr(n,"requires_symtab")) {
	Setattr(n,"symtab", Swig_symbol_popscope());
	Delattr(n,"requires_symtab");
	Namespaceprefix = Swig_symbol_qualifiedscopename(0);
      }
      if (add_oldname) {
	Delete(add_oldname);
      }
      if (Strcmp(nodeType(n),"class") == 0) {
	inclass = 0;
      }
      add_oldname = 0;
    } else {
      if (Strcmp(nodeType(n),"extend") == 0) {
	emode = cplus_mode;
	cplus_mode = CPLUS_PUBLIC;
      }
      add_symbols_copy(firstChild(n));
      if (Strcmp(nodeType(n),"extend") == 0) {
	cplus_mode = emode;
      }
    }
    n = nextSibling(n);
  }
}

/* Extension merge.  This function is used to handle the %extend directive
   when it appears before a class definition.   To handle this, the %extend
   actually needs to take precedence.  Therefore, we will selectively nuke symbols
   from the current symbol table, replacing them with the added methods */

static void merge_extensions(Node *am) {
  Node *n;
  Node *csym;

  n = firstChild(am);
  while (n) {
    String *symname;
    symname = Getattr(n,"sym:name");
    DohIncref(symname);
    if ((symname) && (!Getattr(n,"error"))) {
      /* Remove node from its symbol table */
      Swig_symbol_remove(n);
      csym = Swig_symbol_add(symname,n);
      if (csym != n) {
	/* Conflict with previous definition.  Nuke previous definition */
	String *e = NewString("");
	Printf(e,"Identifier '%s' redeclared (ignored).\n", symname);
	Printf(e,"%s:%d: Previous definition of tag '%s'", Getfile(n),Getline(n), symname);
	Swig_warning(WARN_PARSE_REDEFINED,Getfile(csym), Getline(csym), "%s\n", e);
	Setattr(csym,"error",e);
	Swig_symbol_remove(csym);              /* Remove class definition */
	Swig_symbol_add(symname,n);            /* Insert extend definition */
      }
    }
    n = nextSibling(n);
  }
}

/* Check for unused %extend.  Special case, don't report unused
   extensions for templates */
 
 static void check_extensions() {
   String *key;
   if (!extendhash) return;
   for (key = Firstkey(extendhash); key; key = Nextkey(extendhash)) {
     Node *n = Getattr(extendhash,key);
     if (!Strstr(key,"<")) {
       Swig_warning(WARN_PARSE_EXTEND_UNDEF,Getfile(n), Getline(n), "%%extend defined for an undeclared class %s.\n", key);
     }
   }
 }

/* Check a set of declarations to see if any are pure-abstract */

 static List *pure_abstract(Node *n) {
   List *abs = 0;
   while (n) {
     if (Cmp(nodeType(n),"cdecl") == 0) {
       String *decl = Getattr(n,"decl");
       if (SwigType_isfunction(decl)) {
	 String *init = Getattr(n,"value");
	 if (Cmp(init,"0") == 0) {
	   if (!abs) {
	     abs = NewList();
	   }
	   Append(abs,n);
	   Setattr(n,"abstract","1");
	 }
       }
     } else if (Cmp(nodeType(n),"destructor") == 0) {
       if (Cmp(Getattr(n,"value"),"0") == 0) {
	 if (!abs) {
	   abs = NewList();
	 }
	 Append(abs,n);
	 Setattr(n,"abstract","1");
       }
     }
     n = nextSibling(n);
   }
   return abs;
 }

 /* Make a classname */

 static String *make_class_name(String *name) {
   String *nname = 0;
   if (Namespaceprefix) {
     nname= NewStringf("%s::%s", Namespaceprefix, name);
   } else {
     nname = NewString(name);
   }
   if (SwigType_istemplate(nname)) {
     String *prefix, *args, *qargs;
     prefix = SwigType_templateprefix(nname);
     args   = SwigType_templateargs(nname);
     qargs  = Swig_symbol_type_qualify(args,0);
     Append(prefix,qargs);
     Delete(nname);
     nname = prefix;
   }
   return nname;
 }

 static List *make_inherit_list(String *clsname, List *names) {
   int i;
   String *derived;
   List *bases = NewList();

   if (Namespaceprefix) derived = NewStringf("%s::%s", Namespaceprefix,clsname);
   else derived = NewString(clsname);

   for (i = 0; i < Len(names); i++) {
     Node *s;
     String *base;
     String *n = Getitem(names,i);
     /* Try to figure out where this symbol is */
     s = Swig_symbol_clookup(n,0);
     if (s) {
       while (s && (Strcmp(nodeType(s),"class") != 0)) {
	 /* Not a class.  Could be a typedef though. */
	 String *storage = Getattr(s,"storage");
	 if (storage && (Strcmp(storage,"typedef") == 0)) {
	   String *nn = Getattr(s,"type");
	   s = Swig_symbol_clookup(nn,Getattr(s,"sym:symtab"));
	 } else {
	   break;
	 }
       }
       if (s && ((Strcmp(nodeType(s),"class") == 0) || (Strcmp(nodeType(s),"template") == 0))) {
	 String *q = Swig_symbol_qualified(s);
	 Append(bases,s);
	 if (q) {
	   base = NewStringf("%s::%s", q, Getattr(s,"name"));
	 } else {
	   base = NewString(Getattr(s,"name"));
	 }
       } else {
	 base = NewString(n);
       }
     } else {
       base = NewString(n);
     }
     if (base) {
       rename_inherit(base,derived);
       Delete(base);
     }
   }
   return bases;
 }

/* Structures for handling code fragments built for nested classes */

typedef struct Nested {
  String   *code;        /* Associated code fragment */
  int      line;         /* line number where it starts */
  char     *name;        /* Name associated with this nested class */
  char     *kind;        /* Kind of class */
  SwigType *type;        /* Datatype associated with the name */
  struct Nested   *next;        /* Next code fragment in list */
} Nested;

/* Some internal variables for saving nested class information */

static Nested      *nested_list = 0;

/* Add a function to the nested list */

static void add_nested(Nested *n) {
  Nested *n1;
  if (!nested_list) nested_list = n;
  else {
    n1 = nested_list;
    while (n1->next) n1 = n1->next;
    n1->next = n;
  }
}

/* Dump all of the nested class declarations to the inline processor
 * However.  We need to do a few name replacements and other munging
 * first.  This function must be called before closing a class! */

static Node *dump_nested(char *parent) {
  Nested *n,*n1;
  Node *ret = 0;
  n = nested_list;
  if (!parent) {
    nested_list = 0;
    return 0;
  }
  while (n) {
    char temp[256];
    Node *retx;
    /* Token replace the name of the parent class */
    Replace(n->code, "$classname", parent, DOH_REPLACE_ANY);
    /* Fix up the name of the datatype (for building typedefs and other stuff) */
    sprintf(temp,"%s_%s", parent,n->name);

    Append(n->type,parent);
    Append(n->type,"_");
    Append(n->type,n->name);

    /* Add the appropriate declaration to the C++ processor */
    retx = new_node("cdecl");
    Setattr(retx,"name",n->name);
    Setattr(retx,"type",Copy(n->type));
    Setattr(retx,"nested",parent);
    add_symbols(retx);
    if (ret) {
      set_nextSibling(retx,ret);
    }
    ret = retx;

    /* Insert a forward class declaration */
    /* Disabled: [ 597599 ] union in class: incorrect scope 
    retx = new_node("classforward");
    Setattr(retx,"kind",n->kind);
    Setattr(retx,"name",Copy(n->type));
    Setattr(retx,"sym:name", make_name(n->type,0));
    set_nextSibling(retx,ret);
    ret = retx; 
    */

    /* Make all SWIG created typedef structs/unions/classes unnamed else 
       redefinition errors occur - nasty hack alert.*/

    {
      const char* types_array[3] = {"struct", "union", "class"};
      int i;
      for (i=0; i<3; i++) {
	char* code_ptr = Char(n->code);
      while (code_ptr) {
        /* Replace struct name (as in 'struct name {' ) with whitespace
           name will be between struct and { */
	
        code_ptr = strstr(code_ptr, types_array[i]);
        if (code_ptr) {
	  char *open_bracket_pos;
          code_ptr += strlen(types_array[i]);
          open_bracket_pos = strstr(code_ptr, "{");
          if (open_bracket_pos) { 
            /* Make sure we don't have something like struct A a; */
            char* semi_colon_pos = strstr(code_ptr, ";");
            if (!(semi_colon_pos && (semi_colon_pos < open_bracket_pos)))
              while (code_ptr < open_bracket_pos)
                *code_ptr++ = ' ';
          }
        }
      }
      }
    }
    
    {
      /* Remove SWIG directive %constant which may be left in the SWIG created typedefs */
      char* code_ptr = Char(n->code);
      while (code_ptr) {
	code_ptr = strstr(code_ptr, "%constant");
	if (code_ptr) {
	  char* directive_end_pos = strstr(code_ptr, ";");
	  if (directive_end_pos) { 
            while (code_ptr <= directive_end_pos)
              *code_ptr++ = ' ';
	  }
	}
      }
    }
    {
      Node *head;
      head = new_node("insert");
      Setattr(head,"code",NewStringf("\n%s\n",n->code));
      set_nextSibling(head,ret);
      ret = head;
    }
      
    /* Dump the code to the scanner */
    start_inline(Char(n->code),n->line);

    n1 = n->next;
    Delete(n->code);
    free(n);
    n = n1;
  }
  nested_list = 0;
  return ret;
}

Node *Swig_cparse(File *f) {
  extern void scanner_file(File *);
  extern int yyparse();
  scanner_file(f);
  top = 0;
  yyparse();
  return top;
}


#line 767 "parser.y"
typedef union {
  char  *id;
  List  *bases;
  struct Define {
    String *val;
    String *rawval;
    int     type;
    String *qualifier;
    String *bitfield;
    Parm   *throws;
  } dtype;
  struct {
    char *type;
    char *filename;
    int   line;
  } loc;
  struct {
    char      *id;
    SwigType  *type;
    String    *defarg;
    ParmList  *parms;
    short      have_parms;
    ParmList  *throws;
  } decl;
  Parm         *tparms;
  struct {
    String     *op;
    Hash       *kwargs;
  } tmap;
  struct {
    String     *type;
    String     *us;
  } ptype;
  SwigType     *type;
  String       *str;
  Parm         *p;
  ParmList     *pl;
  int           ivalue;
  Node         *node;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		796
#define	YYFLAG		-32768
#define	YYNTBASE	111

#define YYTRANSLATE(x) ((unsigned)(x) <= 365 ? yytranslate[x] : 246)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
    76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
    86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
    96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
   106,   107,   108,   109,   110
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     6,     9,    12,    14,    16,    18,    20,    22,
    24,    26,    28,    30,    32,    34,    36,    38,    40,    42,
    44,    46,    48,    50,    52,    54,    56,    58,    60,    62,
    64,    66,    68,    69,    77,    83,    87,    93,    99,   103,
   106,   109,   115,   118,   124,   127,   135,   143,   144,   152,
   154,   156,   159,   161,   167,   173,   179,   183,   188,   192,
   200,   209,   215,   219,   221,   223,   227,   229,   234,   242,
   249,   251,   253,   261,   271,   277,   285,   287,   289,   295,
   303,   305,   309,   316,   323,   332,   334,   337,   341,   343,
   346,   350,   357,   363,   373,   376,   378,   380,   386,   392,
   394,   399,   401,   403,   406,   412,   419,   427,   436,   443,
   445,   447,   449,   451,   453,   455,   456,   466,   467,   476,
   478,   481,   486,   487,   494,   496,   498,   500,   502,   504,
   506,   510,   515,   516,   523,   524,   530,   536,   539,   540,
   547,   549,   550,   554,   556,   558,   560,   562,   564,   566,
   568,   570,   574,   576,   578,   580,   582,   584,   586,   593,
   600,   608,   617,   626,   634,   640,   643,   646,   649,   650,
   658,   659,   666,   668,   670,   672,   674,   676,   678,   680,
   682,   684,   686,   688,   690,   692,   695,   698,   701,   706,
   709,   711,   714,   716,   718,   720,   722,   724,   726,   729,
   731,   735,   737,   740,   747,   751,   753,   756,   758,   762,
   764,   766,   768,   770,   773,   777,   780,   783,   785,   788,
   791,   793,   795,   797,   799,   802,   806,   808,   811,   815,
   820,   826,   831,   833,   836,   840,   845,   851,   855,   860,
   865,   867,   870,   875,   881,   885,   890,   895,   897,   900,
   903,   907,   909,   912,   914,   917,   921,   926,   930,   935,
   938,   942,   946,   951,   955,   959,   962,   965,   967,   969,
   972,   974,   976,   978,   981,   983,   985,   987,   989,   992,
   995,   997,  1000,  1002,  1005,  1007,  1009,  1012,  1014,  1016,
  1018,  1020,  1022,  1024,  1026,  1028,  1029,  1032,  1034,  1036,
  1038,  1040,  1044,  1046,  1048,  1052,  1054,  1056,  1058,  1060,
  1066,  1068,  1070,  1074,  1079,  1085,  1091,  1098,  1100,  1102,
  1104,  1106,  1108,  1110,  1112,  1116,  1120,  1124,  1128,  1132,
  1136,  1140,  1144,  1148,  1152,  1156,  1159,  1162,  1165,  1168,
  1170,  1171,  1175,  1177,  1179,  1183,  1186,  1191,  1193,  1195,
  1197,  1199,  1201,  1203,  1205,  1207,  1209,  1211,  1216,  1222,
  1224,  1228,  1232,  1237,  1242,  1246,  1249,  1251,  1253,  1257,
  1260,  1264,  1266,  1268,  1270,  1272,  1274,  1277,  1282,  1284,
  1288,  1290,  1294,  1298,  1301,  1304,  1307,  1310,  1313,  1318,
  1320,  1324,  1326,  1330,  1334,  1337,  1340,  1343,  1346,  1348,
  1350,  1352,  1354,  1358,  1360,  1364,  1370,  1372,  1376,  1378,
  1380
};

static const short yyrhs[] = {   112,
     0,    94,   188,    34,     0,    94,     1,     0,   112,   113,
     0,   245,     0,   114,     0,   148,     0,   154,     0,    34,
     0,     1,     0,   153,     0,   115,     0,   117,     0,   118,
     0,   119,     0,   120,     0,   121,     0,   122,     0,   123,
     0,   126,     0,   127,     0,   128,     0,   129,     0,   130,
     0,   131,     0,   134,     0,   136,     0,   138,     0,   140,
     0,   145,     0,   146,     0,   147,     0,     0,    54,   242,
   235,    37,   116,   168,    38,     0,    77,   144,    37,   142,
    38,     0,    78,   142,    34,     0,    50,     3,    44,   210,
    34,     0,    50,   204,   196,   193,    34,     0,    50,     1,
    34,     0,    76,     4,     0,    76,   240,     0,    75,    31,
     3,    32,    37,     0,    75,    37,     0,    75,    31,     3,
    32,    34,     0,    75,    34,     0,    80,    31,   233,    33,
   233,    32,     4,     0,    80,    31,   233,    33,   233,    32,
    37,     0,     0,   125,   242,   240,    47,   124,   112,    48,
     0,     7,     0,     8,     0,    73,     4,     0,     4,     0,
     9,    31,   233,    32,   240,     0,     9,    31,   233,    32,
     4,     0,     9,    31,   233,    32,    37,     0,    46,   242,
   233,     0,    51,    31,   233,    32,     0,    51,    31,    32,
     0,    72,    31,     3,    32,   184,     3,    34,     0,    72,
    31,     3,    32,   184,   204,   196,    34,     0,    55,   133,
     3,    44,   132,     0,    55,   133,     3,     0,   240,     0,
     4,     0,    31,     3,    32,     0,   245,     0,   135,   196,
   233,    34,     0,   135,    31,   233,    32,   196,   227,    34,
     0,   135,    31,   233,    32,   240,    34,     0,    52,     0,
    53,     0,    56,    31,   233,    32,   196,   227,   137,     0,
    56,    31,   233,    33,   233,    32,   196,   227,    34,     0,
    56,    31,   233,    32,   137,     0,    56,    31,   233,    33,
   233,    32,    34,     0,   241,     0,    34,     0,    87,    31,
   185,    32,    34,     0,    57,    31,   139,    32,   196,   227,
    34,     0,   185,     0,    11,    33,   188,     0,    74,    31,
   141,    32,   142,   241,     0,    74,    31,   141,    32,   142,
    34,     0,    74,    31,   141,    32,   142,    44,   144,    34,
     0,   243,     0,   144,   143,     0,    33,   144,   143,     0,
   245,     0,   204,   195,     0,    31,   185,    32,     0,    31,
   185,    32,    31,   185,    32,     0,    86,    31,   185,    32,
    34,     0,    79,    31,   234,    32,   238,    82,   189,    83,
    34,     0,    81,   240,     0,   149,     0,   152,     0,    35,
   240,    37,   112,    38,     0,   184,   204,   196,   151,   150,
     0,    34,     0,    33,   196,   151,   150,     0,    37,     0,
   193,     0,   202,   193,     0,    68,    31,   185,    32,   193,
     0,   202,    68,    31,   185,    32,   193,     0,   184,    58,
   212,    37,   213,    38,    34,     0,   184,    58,   212,    37,
   213,    38,   196,   150,     0,   184,   204,    31,   185,    32,
   228,     0,   155,     0,   159,     0,   160,     0,   164,     0,
   165,     0,   175,     0,     0,   184,   225,   235,   219,    37,
   156,   168,    38,   158,     0,     0,   184,   225,    37,   157,
   168,    38,   196,   150,     0,    34,     0,   196,   150,     0,
   184,   225,   235,    34,     0,     0,    91,    82,   163,    83,
   161,   162,     0,   149,     0,   155,     0,   172,     0,   160,
     0,   159,     0,   186,     0,    70,   235,    34,     0,    70,
    71,   235,    34,     0,     0,    71,   235,    37,   166,   112,
    38,     0,     0,    71,    37,   167,   112,    38,     0,    71,
     3,    44,   235,    34,     0,   171,   168,     0,     0,    54,
    37,   169,   168,    38,   168,     0,   245,     0,     0,     1,
   170,   168,     0,   148,     0,   172,     0,   173,     0,   176,
     0,   181,     0,   174,     0,   159,     0,   177,     0,   184,
   235,    34,     0,   164,     0,   160,     0,   175,     0,   146,
     0,   147,     0,    34,     0,   184,   204,    31,   185,    32,
   228,     0,   108,   237,    31,   185,    32,   182,     0,    66,
   108,   237,    31,   185,    32,   183,     0,   184,    93,   204,
   201,    31,   185,    32,   183,     0,   184,    93,   204,   100,
    31,   185,    32,   183,     0,   184,    93,   204,    31,   185,
    32,   183,     0,    69,    31,   185,    32,    37,     0,    62,
    64,     0,    61,    64,     0,    63,    64,     0,     0,   184,
   225,     3,    37,   178,   180,    34,     0,     0,   184,   225,
    37,   179,   180,    34,     0,   196,     0,   245,     0,   131,
     0,   119,     0,   129,     0,   134,     0,   136,     0,   138,
     0,   127,     0,   140,     0,   117,     0,   118,     0,   120,
     0,   227,    34,     0,   227,    37,     0,   227,    34,     0,
   227,    44,   210,    34,     0,   227,    37,     0,    35,     0,
    35,   240,     0,    65,     0,    18,     0,    66,     0,    67,
     0,   245,     0,   186,     0,   188,   187,     0,   245,     0,
    33,   188,   187,     0,   245,     0,   205,   194,     0,    91,
    82,   225,    83,   225,   235,     0,    39,    39,    39,     0,
   190,     0,   192,   191,     0,   245,     0,    33,   192,   191,
     0,   245,     0,   188,     0,   217,     0,     6,     0,    44,
   210,     0,    44,   100,   235,     0,    44,    37,     0,    64,
    11,     0,   245,     0,   196,   193,     0,   199,   193,     0,
   193,     0,   196,     0,   199,     0,   245,     0,   201,   197,
     0,   201,   100,   197,     0,   198,     0,   100,   197,     0,
   235,    89,   197,     0,   201,   235,    89,   197,     0,   201,
   235,    89,   100,   197,     0,   235,    89,   100,   197,     0,
   235,     0,   108,   235,     0,    31,   235,    32,     0,    31,
   201,   197,    32,     0,    31,   235,    89,   197,    32,     0,
   197,    47,    48,     0,   197,    47,   216,    48,     0,   197,
    31,   185,    32,     0,   235,     0,   108,   235,     0,    31,
   201,   198,    32,     0,    31,   235,    89,   198,    32,     0,
   198,    47,    48,     0,   198,    47,   216,    48,     0,   198,
    31,   185,    32,     0,   201,     0,   201,   200,     0,   201,
   100,     0,   201,   100,   200,     0,   200,     0,   100,   200,
     0,   100,     0,   235,    89,     0,   201,   235,    89,     0,
   201,   235,    89,   200,     0,   200,    47,    48,     0,   200,
    47,   216,    48,     0,    47,    48,     0,    47,   216,    48,
     0,    31,   199,    32,     0,   200,    31,   185,    32,     0,
    31,   185,    32,     0,   105,   202,   201,     0,   105,   201,
     0,   105,   202,     0,   105,     0,   203,     0,   203,   202,
     0,    40,     0,    41,     0,   205,     0,   202,   206,     0,
   206,     0,   207,     0,    28,     0,    26,     0,    29,   232,
     0,    58,   235,     0,    30,     0,   206,   202,     0,   235,
     0,   225,   235,     0,   208,     0,   209,     0,   209,   208,
     0,    19,     0,    21,     0,    22,     0,    25,     0,    23,
     0,    24,     0,    27,     0,    20,     0,     0,   211,   216,
     0,   240,     0,    10,     0,     3,     0,   245,     0,   213,
    33,   214,     0,   214,     0,     3,     0,     3,    44,   215,
     0,   245,     0,   216,     0,    10,     0,   217,     0,    45,
    31,   204,   194,    32,     0,   218,     0,   204,     0,    31,
   216,    32,     0,    31,   216,    32,   216,     0,    31,   216,
   201,    32,   216,     0,    31,   216,   100,    32,   216,     0,
    31,   216,   201,   100,    32,   216,     0,    11,     0,    12,
     0,    13,     0,    14,     0,    15,     0,    16,     0,    17,
     0,   216,   103,   216,     0,   216,   104,   216,     0,   216,
   105,   216,     0,   216,   106,   216,     0,   216,   100,   216,
     0,   216,    98,   216,     0,   216,    99,   216,     0,   216,
   101,   216,     0,   216,   102,   216,     0,   216,    97,   216,
     0,   216,    96,   216,     0,   104,   216,     0,   108,   216,
     0,   109,   216,     0,   204,    31,     0,   220,     0,     0,
    64,   221,   222,     0,   245,     0,   223,     0,   222,    33,
   223,     0,   226,   235,     0,   226,   224,   226,   235,     0,
    62,     0,    61,     0,    63,     0,    59,     0,    42,     0,
    43,     0,    60,     0,    66,     0,   245,     0,   202,     0,
    68,    31,   185,    32,     0,   202,    68,    31,   185,    32,
     0,   245,     0,   227,   229,    34,     0,   227,   229,    37,
     0,    31,   185,    32,    34,     0,    31,   185,    32,    37,
     0,    44,   210,    34,     0,    64,   230,     0,   245,     0,
   231,     0,   230,    33,   231,     0,   235,    31,     0,    82,
   189,    83,     0,   245,     0,     3,     0,   240,     0,   233,
     0,   245,     0,   237,   236,     0,    88,   110,   237,   236,
     0,   237,     0,    88,   110,   237,     0,    92,     0,    88,
   110,    92,     0,   110,   237,   236,     0,   110,   237,     0,
   110,    92,     0,    90,   237,     0,     3,   232,     0,     3,
   239,     0,    88,   110,     3,   239,     0,     3,     0,    88,
   110,     3,     0,    92,     0,    88,   110,    92,     0,   110,
     3,   239,     0,   110,     3,     0,   110,    92,     0,    90,
     3,     0,   240,     6,     0,     6,     0,   240,     0,    37,
     0,     4,     0,    31,   243,    32,     0,   245,     0,   233,
    44,   244,     0,   233,    44,   244,    33,   243,     0,   233,
     0,   233,    33,   243,     0,   240,     0,   217,     0,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   909,   921,   924,   929,   933,   938,   939,   940,   941,   942,
   954,   967,   968,   969,   970,   971,   972,   973,   974,   975,
   976,   977,   978,   979,   980,   981,   982,   983,   984,   985,
   986,   987,   994,  1022,  1064,  1074,  1085,  1103,  1124,  1135,
  1144,  1163,  1169,  1175,  1180,  1190,  1196,  1212,  1217,  1239,
  1240,  1247,  1276,  1280,  1290,  1295,  1310,  1324,  1328,  1340,
  1346,  1372,  1378,  1385,  1386,  1389,  1390,  1398,  1409,  1453,
  1464,  1467,  1480,  1527,  1579,  1590,  1603,  1604,  1605,  1610,
  1646,  1647,  1676,  1688,  1696,  1709,  1731,  1737,  1741,  1744,
  1752,  1757,  1769,  1779,  2002,  2012,  2018,  2022,  2038,  2077,
  2081,  2100,  2106,  2111,  2116,  2121,  2133,  2141,  2189,  2236,
  2237,  2238,  2239,  2240,  2241,  2244,  2302,  2382,  2394,  2480,
  2481,  2493,  2511,  2511,  2663,  2666,  2669,  2672,  2675,  2680,
  2712,  2718,  2745,  2762,  2772,  2783,  2792,  2819,  2833,  2837,
  2843,  2844,  2853,  2864,  2865,  2869,  2870,  2871,  2872,  2873,
  2874,  2875,  2876,  2877,  2878,  2879,  2880,  2881,  2890,  2927,
  2943,  2976,  2990,  3007,  3025,  3032,  3039,  3047,  3070,  3071,
  3099,  3100,  3126,  3127,  3133,  3136,  3140,  3143,  3144,  3145,
  3146,  3147,  3148,  3149,  3150,  3153,  3156,  3159,  3166,  3173,
  3187,  3188,  3196,  3197,  3198,  3199,  3200,  3207,  3218,  3226,
  3229,  3233,  3237,  3247,  3252,  3260,  3273,  3281,  3284,  3288,
  3292,  3318,  3324,  3332,  3342,  3368,  3376,  3383,  3392,  3396,
  3400,  3407,  3424,  3441,  3449,  3457,  3466,  3470,  3479,  3490,
  3502,  3512,  3525,  3533,  3541,  3549,  3557,  3568,  3579,  3590,
  3609,  3617,  3634,  3642,  3653,  3664,  3675,  3694,  3700,  3706,
  3713,  3722,  3725,  3734,  3741,  3748,  3758,  3769,  3780,  3791,
  3798,  3805,  3808,  3825,  3835,  3842,  3848,  3853,  3859,  3863,
  3869,  3870,  3876,  3882,  3886,  3889,  3892,  3893,  3894,  3895,
  3896,  3897,  3902,  3905,  3910,  3935,  3938,  3980,  3984,  3988,
  3992,  3996,  4000,  4004,  4008,  4014,  4014,  4021,  4028,  4043,
  4044,  4049,  4060,  4063,  4069,  4082,  4085,  4094,  4105,  4106,
  4111,  4112,  4130,  4137,  4141,  4146,  4151,  4159,  4160,  4161,
  4162,  4163,  4164,  4165,  4168,  4172,  4176,  4180,  4184,  4188,
  4192,  4196,  4200,  4204,  4208,  4212,  4216,  4220,  4224,  4235,
  4240,  4240,  4241,  4244,  4249,  4255,  4265,  4277,  4278,  4279,
  4283,  4287,  4291,  4295,  4301,  4302,  4305,  4309,  4313,  4317,
  4323,  4329,  4335,  4342,  4349,  4356,  4357,  4360,  4361,  4364,
  4370,  4375,  4378,  4379,  4382,  4383,  4386,  4391,  4395,  4398,
  4401,  4404,  4409,  4413,  4416,  4419,  4425,  4432,  4437,  4441,
  4444,  4447,  4450,  4455,  4459,  4462,  4465,  4471,  4476,  4479,
  4482,  4486,  4491,  4504,  4508,  4513,  4519,  4523,  4530,  4533,
  4538
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","ID","HBLOCK",
"POUND","STRING","INCLUDE","IMPORT","INSERT","CHARCONST","NUM_INT","NUM_FLOAT",
"NUM_UNSIGNED","NUM_LONG","NUM_ULONG","NUM_LONGLONG","NUM_ULONGLONG","TYPEDEF",
"TYPE_INT","TYPE_UNSIGNED","TYPE_SHORT","TYPE_LONG","TYPE_FLOAT","TYPE_DOUBLE",
"TYPE_CHAR","TYPE_VOID","TYPE_SIGNED","TYPE_BOOL","TYPE_TYPEDEF","TYPE_RAW",
"LPAREN","RPAREN","COMMA","SEMI","EXTERN","INIT","LBRACE","RBRACE","PERIOD",
"CONST","VOLATILE","STRUCT","UNION","EQUAL","SIZEOF","MODULE","LBRACKET","RBRACKET",
"ILLEGAL","CONSTANT","NAME","RENAME","NAMEWARN","EXTEND","PRAGMA","FEATURE",
"VARARGS","ENUM","CLASS","TYPENAME","PRIVATE","PUBLIC","PROTECTED","COLON","STATIC",
"VIRTUAL","FRIEND","THROW","CATCH","USING","NAMESPACE","NATIVE","INLINE","TYPEMAP",
"EXCEPT","ECHO","APPLY","CLEAR","SWIGTEMPLATE","FRAGMENT","WARN","LESSTHAN",
"GREATERTHAN","MODULO","DELETE","TYPES","PARMS","NONID","DSTAR","DCNOT","TEMPLATE",
"OPERATOR","COPERATOR","PARSETYPE","CAST","LOR","LAND","OR","XOR","AND","LSHIFT",
"RSHIFT","PLUS","MINUS","STAR","SLASH","UMINUS","NOT","LNOT","DCOLON","program",
"interface","declaration","swig_directive","extend_directive","@1","apply_directive",
"clear_directive","constant_directive","echo_directive","except_directive","fragment_directive",
"include_directive","@2","includetype","inline_directive","insert_directive",
"module_directive","name_directive","native_directive","pragma_directive","pragma_arg",
"pragma_lang","rename_directive","rename_namewarn","feature_directive","stringbracesemi",
"varargs_directive","varargs_parms","typemap_directive","typemap_type","tm_list",
"tm_tail","typemap_parm","types_directive","template_directive","warn_directive",
"c_declaration","c_decl","c_decl_tail","initializer","c_enum_decl","c_constructor_decl",
"cpp_declaration","cpp_class_decl","@3","@4","cpp_opt_declarators","cpp_forward_class_decl",
"cpp_template_decl","@5","cpp_temp_possible","template_parms","cpp_using_decl",
"cpp_namespace_decl","@6","@7","cpp_members","@8","@9","cpp_member","cpp_constructor_decl",
"cpp_destructor_decl","cpp_conversion_operator","cpp_catch_decl","cpp_protection_decl",
"cpp_nested","@10","@11","nested_decl","cpp_swig_directive","cpp_end","cpp_vend",
"storage_class","parms","rawparms","ptail","parm","valparms","rawvalparms","valptail",
"valparm","def_args","parameter_declarator","typemap_parameter_declarator","declarator",
"notso_direct_declarator","direct_declarator","abstract_declarator","direct_abstract_declarator",
"pointer","type_qualifier","type_qualifier_raw","type","rawtype","type_right",
"primitive_type","primitive_type_list","type_specifier","definetype","@12","ename",
"enumlist","edecl","etype","expr","exprnum","exprcompound","inherit","raw_inherit",
"@13","base_list","base_specifier","access_specifier","cpptype","opt_virtual",
"cpp_const","ctor_end","ctor_initializer","mem_initializer_list","mem_initializer",
"template_decl","idstring","idstringopt","idcolon","idcolontail","idtemplate",
"idcolonnt","idcolontailnt","string","stringbrace","options","kwargs","stringnum",
"empty", NULL
};
#endif

static const short yyr1[] = {     0,
   111,   111,   111,   112,   112,   113,   113,   113,   113,   113,
   113,   114,   114,   114,   114,   114,   114,   114,   114,   114,
   114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
   114,   114,   116,   115,   117,   118,   119,   119,   119,   120,
   120,   121,   121,   121,   121,   122,   122,   124,   123,   125,
   125,   126,   127,   127,   127,   127,   128,   129,   129,   130,
   130,   131,   131,   132,   132,   133,   133,   134,   134,   134,
   135,   135,   136,   136,   136,   136,   137,   137,   137,   138,
   139,   139,   140,   140,   140,   141,   142,   143,   143,   144,
   144,   144,   145,   146,   147,   148,   148,   148,   149,   150,
   150,   150,   151,   151,   151,   151,   152,   152,   153,   154,
   154,   154,   154,   154,   154,   156,   155,   157,   155,   158,
   158,   159,   161,   160,   162,   162,   162,   162,   162,   163,
   164,   164,   166,   165,   167,   165,   165,   168,   169,   168,
   168,   170,   168,   171,   171,   171,   171,   171,   171,   171,
   171,   171,   171,   171,   171,   171,   171,   171,   172,   173,
   173,   174,   174,   174,   175,   176,   176,   176,   178,   177,
   179,   177,   180,   180,   181,   181,   181,   181,   181,   181,
   181,   181,   181,   181,   181,   182,   182,   183,   183,   183,
   184,   184,   184,   184,   184,   184,   184,   185,   186,   186,
   187,   187,   188,   188,   188,   189,   190,   190,   191,   191,
   192,   192,   192,   193,   193,   193,   193,   193,   194,   194,
   194,   195,   195,   195,   196,   196,   196,   196,   196,   196,
   196,   196,   197,   197,   197,   197,   197,   197,   197,   197,
   198,   198,   198,   198,   198,   198,   198,   199,   199,   199,
   199,   199,   199,   199,   199,   199,   199,   200,   200,   200,
   200,   200,   200,   200,   201,   201,   201,   201,   202,   202,
   203,   203,   204,   205,   205,   206,   206,   206,   206,   206,
   206,   206,   206,   206,   207,   208,   208,   209,   209,   209,
   209,   209,   209,   209,   209,   211,   210,   210,   210,   212,
   212,   213,   213,   214,   214,   214,   215,   215,   216,   216,
   216,   216,   216,   216,   216,   216,   216,   217,   217,   217,
   217,   217,   217,   217,   218,   218,   218,   218,   218,   218,
   218,   218,   218,   218,   218,   218,   218,   218,   218,   219,
   221,   220,   220,   222,   222,   223,   223,   224,   224,   224,
   225,   225,   225,   225,   226,   226,   227,   227,   227,   227,
   228,   228,   228,   228,   228,   229,   229,   230,   230,   231,
   232,   232,   233,   233,   234,   234,   235,   235,   235,   235,
   235,   235,   236,   236,   236,   236,   237,   238,   238,   238,
   238,   238,   238,   239,   239,   239,   239,   240,   240,   241,
   241,   241,   242,   242,   243,   243,   243,   243,   244,   244,
   245
};

static const short yyr2[] = {     0,
     1,     3,     2,     2,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     0,     7,     5,     3,     5,     5,     3,     2,
     2,     5,     2,     5,     2,     7,     7,     0,     7,     1,
     1,     2,     1,     5,     5,     5,     3,     4,     3,     7,
     8,     5,     3,     1,     1,     3,     1,     4,     7,     6,
     1,     1,     7,     9,     5,     7,     1,     1,     5,     7,
     1,     3,     6,     6,     8,     1,     2,     3,     1,     2,
     3,     6,     5,     9,     2,     1,     1,     5,     5,     1,
     4,     1,     1,     2,     5,     6,     7,     8,     6,     1,
     1,     1,     1,     1,     1,     0,     9,     0,     8,     1,
     2,     4,     0,     6,     1,     1,     1,     1,     1,     1,
     3,     4,     0,     6,     0,     5,     5,     2,     0,     6,
     1,     0,     3,     1,     1,     1,     1,     1,     1,     1,
     1,     3,     1,     1,     1,     1,     1,     1,     6,     6,
     7,     8,     8,     7,     5,     2,     2,     2,     0,     7,
     0,     6,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     2,     2,     2,     4,     2,
     1,     2,     1,     1,     1,     1,     1,     1,     2,     1,
     3,     1,     2,     6,     3,     1,     2,     1,     3,     1,
     1,     1,     1,     2,     3,     2,     2,     1,     2,     2,
     1,     1,     1,     1,     2,     3,     1,     2,     3,     4,
     5,     4,     1,     2,     3,     4,     5,     3,     4,     4,
     1,     2,     4,     5,     3,     4,     4,     1,     2,     2,
     3,     1,     2,     1,     2,     3,     4,     3,     4,     2,
     3,     3,     4,     3,     3,     2,     2,     1,     1,     2,
     1,     1,     1,     2,     1,     1,     1,     1,     2,     2,
     1,     2,     1,     2,     1,     1,     2,     1,     1,     1,
     1,     1,     1,     1,     1,     0,     2,     1,     1,     1,
     1,     3,     1,     1,     3,     1,     1,     1,     1,     5,
     1,     1,     3,     4,     5,     5,     6,     1,     1,     1,
     1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     2,     2,     2,     2,     1,
     0,     3,     1,     1,     3,     2,     4,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     4,     5,     1,
     3,     3,     4,     4,     3,     2,     1,     1,     3,     2,
     3,     1,     1,     1,     1,     1,     2,     4,     1,     3,
     1,     3,     3,     2,     2,     2,     2,     2,     4,     1,
     3,     1,     3,     3,     2,     2,     2,     2,     1,     1,
     1,     1,     3,     1,     3,     5,     1,     3,     1,     1,
     0
};

static const short yydefact[] = {   411,
     0,     0,     5,     3,   411,   288,   295,   289,   290,   292,
   293,   291,   278,   294,   277,   411,   281,     0,   271,   272,
   352,   353,     0,   351,   354,     0,     0,   381,     0,     0,
   269,   411,   275,   276,   285,   286,     0,   283,   379,    10,
    53,    50,    51,     0,   194,     9,   191,   411,     0,     0,
    71,    72,   411,   411,     0,     0,   193,   195,   196,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     4,     6,    12,    13,    14,    15,
    16,    17,    18,    19,   411,    20,    21,    22,    23,    24,
    25,    26,     0,    27,    28,    29,    30,    31,    32,     7,
    96,    97,    11,     8,   110,   111,   112,   113,   114,   115,
     0,   197,   411,   387,   372,   279,     0,   280,     0,     0,
     2,   274,   270,   411,   296,     0,     0,   254,   268,     0,
   221,   203,   411,   227,   411,   252,   248,   241,   218,   282,
   287,   284,     0,     0,   377,     0,   399,   192,     0,     0,
   404,     0,   411,     0,   273,     0,     0,     0,     0,    67,
     0,   411,   411,     0,     0,   411,   135,     0,     0,    52,
     0,     0,    45,    43,    40,    41,   411,     0,   411,     0,
   411,   411,     0,    95,   411,   411,     0,     0,     0,     0,
     0,   241,   411,     0,     0,   213,   318,   319,   320,   321,
   322,   323,   324,   211,     0,   206,   411,   212,   208,   205,
   382,   380,     0,   411,   254,     0,   198,   411,     0,   248,
   283,   200,   299,   216,     0,   214,     0,   298,     0,     0,
   260,     0,     0,     0,   312,     0,   309,   311,   217,   411,
     0,   228,   253,   233,   266,   267,   242,   219,   411,     0,
   220,   411,     0,   250,   225,   249,   233,   255,   386,   385,
   384,   373,     0,   374,   398,   411,   407,     0,    57,    39,
   296,     0,   411,    59,     0,     0,     0,    63,     0,     0,
     0,    81,     0,     0,   131,     0,   411,   133,     0,     0,
    86,     0,     0,     0,    90,   222,   223,   224,    36,     0,
    87,    89,   375,     0,   376,     0,     0,     0,   130,     0,
   411,     0,     0,     0,     0,     0,     0,   233,     0,   411,
     0,   301,   411,   411,   118,   284,   371,     0,   207,   210,
   378,     0,   248,   283,   264,     0,   199,   202,   262,   250,
     0,   241,   255,   215,   297,     0,     0,   336,   337,   338,
   339,   261,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   248,   283,   234,   411,     0,   265,     0,
   245,     0,     0,   258,     0,   226,   251,   256,     0,   229,
   383,     0,     0,     0,     0,   403,     0,     0,    58,    33,
    66,     0,     0,     0,     0,     0,     0,   132,     0,     0,
   411,   411,     0,     0,    91,     0,   411,     0,     0,     0,
   123,    48,   241,     0,     0,     0,     0,    68,     0,   411,
     0,   283,     0,     0,   103,   411,     0,   122,   341,     0,
   340,   343,   411,     0,     0,   255,   411,   243,   256,     0,
   313,     0,   268,     0,   411,   335,   334,   330,   331,   329,
   332,   333,   325,   326,   327,   328,     0,   233,   235,   255,
     0,   238,     0,   247,   246,   263,   259,     0,   230,   257,
   232,    55,    56,    54,    98,   408,   410,   409,   405,    37,
    38,     0,    65,    62,    64,   402,    78,   401,     0,    75,
   411,   400,    77,     0,    82,   411,   165,   137,   136,     0,
   191,     0,     0,    44,    42,   411,    35,    88,   390,     0,
   392,     0,     0,    93,   411,   411,   411,     0,     0,   304,
     0,   303,   306,   411,   411,     0,   100,   102,    99,     0,
   104,   142,   158,     0,     0,     0,     0,   195,     0,   183,
   184,   176,   185,   181,   177,   175,   178,   179,   180,   182,
   156,   157,   144,   150,   154,   153,     0,     0,   145,   146,
   149,   155,   147,   151,   148,     0,   197,   411,   116,   209,
   204,   201,   244,   314,     0,   267,     0,     0,     0,   236,
     0,   240,   239,   231,     0,     0,   411,     0,   357,     0,
   360,     0,     0,   134,   192,   411,     0,    84,     0,    83,
     0,     0,     0,   388,     0,   411,     0,   125,   126,   129,
   128,   124,   127,     0,     0,     0,    70,     0,   411,     0,
   411,   296,   411,   109,     0,   411,   411,     0,   139,   167,
   166,   168,     0,     0,     0,   138,     0,     0,     0,   283,
   355,   342,   344,     0,   356,     0,   316,   315,     0,   310,
   237,   406,    34,     0,   411,     0,    73,    76,   411,    80,
    60,     0,     0,    92,   397,   395,   396,   391,   393,     0,
    46,    47,    49,    69,   308,   305,   307,   302,   107,     0,
     0,     0,     0,     0,   367,   411,     0,     0,   143,     0,
     0,   411,     0,     0,   411,   411,   171,   284,   152,   411,
   349,   348,   350,   411,   346,     0,   317,     0,     0,   411,
     0,    61,    85,   394,   389,     0,   108,     0,   365,   366,
   368,     0,   361,   362,   105,   101,   411,     0,   411,     0,
   119,   411,     0,     0,     0,   169,   411,   345,     0,     0,
    79,   358,     0,    74,    94,   363,   364,     0,   370,   106,
     0,     0,   411,     0,   411,   411,   411,   411,     0,   173,
   174,   347,   120,   117,     0,   359,   369,   140,   411,   160,
     0,   411,     0,     0,   159,     0,   172,   121,   161,     0,
   186,   187,   164,   411,   411,   170,   188,   190,   296,   163,
   162,     0,   189,     0,     0,     0
};

static const short yydefgoto[] = {   794,
     2,    75,    76,    77,   482,   540,   541,   542,   543,    82,
    83,    84,   516,    85,    86,   544,    88,   545,    90,   546,
   484,   159,   547,    93,   548,   490,   549,   281,   550,   290,
   180,   301,   181,    97,   551,   552,   553,   101,   529,   424,
   102,   103,   104,   105,   646,   427,   764,   554,   555,   515,
   612,   308,   556,   109,   401,   287,   557,   690,   628,   558,
   559,   560,   561,   562,   563,   564,   758,   737,   759,   565,
   770,   779,   566,   216,   217,   337,   218,   205,   206,   329,
   207,   131,   132,   295,   133,   242,   134,   219,   136,   191,
    30,    31,   235,   155,    33,    34,    35,    36,   226,   227,
   321,   521,   522,   676,   450,   237,   238,   430,   431,   568,
   642,   643,   704,    37,   644,   780,   624,   684,   720,   721,
   114,   267,   304,    38,   145,    39,   512,   604,   264,   493,
   150,   268,   479,   222
};

static const short yypact[] = {   -29,
  2265,  1840,-32768,-32768,    -9,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,    -9,-32768,    81,-32768,-32768,
-32768,-32768,   136,-32768,-32768,    54,   141,-32768,   204,  2728,
   452,   783,   452,-32768,-32768,  1041,   136,-32768,   124,-32768,
-32768,-32768,-32768,   139,-32768,-32768,   198,   225,  2308,   230,
-32768,-32768,   225,   237,   243,   254,-32768,-32768,-32768,   261,
    38,    73,   265,   303,   272,   316,   358,  2560,  2560,   361,
   373,   198,   404,   364,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,   225,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,   229,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  2602,-32768,  2434,-32768,-32768,-32768,   413,-32768,    42,   571,
-32768,   452,-32768,  1114,    29,  1328,   453,   751,    57,   136,
-32768,-32768,   220,   125,   220,   327,   170,   379,-32768,-32768,
-32768,-32768,   478,    66,-32768,   244,-32768,   253,   244,   244,
-32768,   449,     7,   495,-32768,   276,   136,   484,   491,-32768,
   244,  2476,  2518,   136,   465,    87,-32768,   466,   507,-32768,
   244,   528,-32768,-32768,-32768,   536,  2518,   523,    75,   535,
   541,   244,   244,   536,  2518,  2518,   198,   188,    49,   244,
   879,   487,   148,   756,   189,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,   496,-32768,   549,-32768,-32768,-32768,
-32768,   124,   503,  1114,   351,   569,-32768,   566,   579,   285,
   533,-32768,-32768,-32768,   136,-32768,  1702,   536,  1702,   593,
-32768,  1702,  1702,  1702,   608,   945,-32768,-32768,-32768,  1114,
   136,   391,   327,-32768,-32768,   543,-32768,-32768,  2518,  1381,
-32768,  2518,  1435,   751,   391,   327,   551,   894,-32768,-32768,
   124,-32768,   609,   536,-32768,-32768,    61,   621,-32768,-32768,
   196,   137,   220,-32768,   626,   623,   629,   619,   576,   632,
   634,-32768,   638,   639,-32768,   136,-32768,-32768,   640,   642,
-32768,   652,   657,  2560,-32768,-32768,-32768,-32768,-32768,  2560,
-32768,-32768,-32768,   665,-32768,   668,   666,   624,-32768,    44,
    30,   633,   684,   628,   137,   686,    49,   635,   894,    20,
   689,-32768,  1745,   670,-32768,   305,-32768,  2434,-32768,-32768,
-32768,   571,   165,   641,-32768,  2518,-32768,-32768,-32768,   351,
   340,   643,   633,-32768,  1088,   990,  2644,-32768,-32768,-32768,
-32768,-32768,  1702,  1702,  1702,  1702,  1702,  1702,  1702,  1702,
  1702,  1702,  1702,   862,   -14,-32768,  2518,  1488,-32768,   691,
-32768,  2316,   707,-32768,  2693,   391,   327,   865,    49,   391,
-32768,   184,  1926,   244,  1017,-32768,   709,   710,-32768,-32768,
-32768,   380,   111,   244,  2518,   495,   711,-32768,   715,  2012,
-32768,   567,  2560,   281,   725,   723,   541,   217,   244,   733,
-32768,-32768,-32768,   143,   633,    49,    14,-32768,   896,   767,
   741,   628,   744,   376,-32768,   299,  1230,-32768,-32768,   740,
-32768,-32768,   549,   136,   643,-32768,   566,-32768,   351,   347,
  1702,  1542,  1595,    24,   783,  2215,  1064,  1010,  1046,   907,
   773,   773,   457,   457,-32768,-32768,   349,   643,-32768,    49,
   749,-32768,  2725,-32768,-32768,-32768,-32768,    49,   391,   327,
   391,-32768,-32768,   536,-32768,-32768,-32768,   536,   750,-32768,
-32768,  1230,-32768,-32768,   536,-32768,-32768,-32768,   758,-32768,
   335,   536,-32768,   760,-32768,   335,-32768,-32768,-32768,  2098,
   198,  2686,   915,-32768,-32768,  2518,-32768,-32768,   246,   687,
-32768,   712,   769,-32768,   526,-32768,   335,    27,    49,   755,
   122,-32768,-32768,   445,  2518,   495,-32768,-32768,-32768,   771,
-32768,-32768,-32768,   772,   742,   754,   759,   696,   478,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,   770,  1230,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,  2351,   775,   753,-32768,-32768,
-32768,-32768,-32768,  1088,  1702,  1796,  1702,   788,   790,-32768,
   419,-32768,-32768,   391,   244,   786,  2518,   797,   764,   601,
-32768,   252,   800,-32768,   536,    26,   495,-32768,  2560,-32768,
   803,   835,    85,-32768,    92,  2434,    33,-32768,-32768,-32768,
-32768,-32768,-32768,  2644,  2184,   806,-32768,  1649,   767,   688,
  2518,   196,   777,-32768,   814,   670,  2518,  1230,-32768,-32768,
-32768,-32768,   478,   818,   495,-32768,  2644,   823,   212,   816,
-32768,   822,-32768,   703,-32768,  1230,  1088,  1088,  1702,-32768,
-32768,-32768,-32768,   826,  2518,   829,-32768,-32768,   335,-32768,
-32768,   828,   838,-32768,-32768,   246,-32768,   246,-32768,   780,
-32768,-32768,-32768,-32768,-32768,-32768,  1088,-32768,-32768,   376,
   837,   839,   136,   441,-32768,   220,   376,   842,-32768,  1230,
   849,  2518,   376,    16,  1745,   115,-32768,   851,-32768,   753,
-32768,-32768,-32768,   753,-32768,   848,  1088,   853,   857,  2518,
   856,-32768,-32768,-32768,-32768,   858,-32768,   467,-32768,   861,
-32768,   864,-32768,-32768,-32768,-32768,   220,   860,  2518,   868,
-32768,  2518,   870,   874,   876,-32768,   495,-32768,   136,   737,
-32768,-32768,   881,-32768,-32768,-32768,-32768,   136,-32768,-32768,
  1230,   882,   335,   885,  2518,  2518,   445,   495,   872,-32768,
-32768,-32768,-32768,-32768,   376,-32768,-32768,-32768,   335,-32768,
   530,   335,   888,   892,-32768,   895,-32768,-32768,-32768,   501,
-32768,-32768,-32768,   335,   335,-32768,-32768,-32768,   196,-32768,
-32768,   900,-32768,   926,   930,-32768
};

static const short yypgoto[] = {-32768,
  -247,-32768,-32768,-32768,-32768,     5,    32,    34,    36,-32768,
-32768,-32768,-32768,-32768,-32768,    40,-32768,    41,-32768,    47,
-32768,-32768,    56,-32768,    65,   350,    70,-32768,    72,-32768,
  -265,   540,   -46,-32768,    77,    80,    84,   420,  -368,   329,
-32768,-32768,-32768,   443,-32768,-32768,-32768,    12,    28,-32768,
-32768,-32768,    88,-32768,-32768,-32768,  -457,-32768,-32768,-32768,
   446,-32768,-32768,    90,-32768,-32768,-32768,-32768,   202,-32768,
-32768,  -323,     6,   345,   778,   531,    63,   363,-32768,   542,
   646,  -129,   538,-32768,   -83,   599,  -193,   -20,   -10,    -1,
   -22,-32768,   -48,    23,   -17,-32768,   940,-32768,  -256,-32768,
-32768,-32768,   362,-32768,   583,   -87,-32768,-32768,-32768,-32768,
-32768,   280,-32768,   -43,   286,  -392,   228,-32768,-32768,   241,
   975,   145,-32768,   488,  -168,   -60,-32768,  -252,   -44,   489,
     2,  -154,-32768,     0
};


#define	YYLAST		2831


static const short yytable[] = {     3,
   154,   112,   148,   248,   115,   251,    78,   111,   123,   190,
   140,   135,   122,   106,   387,   115,   291,   459,   383,   179,
   179,   178,   176,    32,   586,   208,   341,   184,   406,   107,
   137,   139,   265,    79,   147,    80,   671,    81,   223,   400,
     5,    87,    89,   331,     5,   459,   732,   151,    91,   265,
   271,     5,   151,   160,   157,   577,  -300,    92,   212,   661,
   617,  -373,   194,    29,     1,   224,    94,   195,     5,   672,
   273,    95,   113,    96,   460,   166,   213,     5,    98,   315,
   228,    99,   259,   261,   151,   100,   187,   666,   113,   108,
   412,   110,   381,   384,   668,   296,    19,    20,   590,   140,
   636,   113,   519,   593,   385,   124,   246,   113,   164,   167,
   324,   113,   209,     5,   486,   733,   147,   243,   341,   117,
   129,   126,   220,   578,   616,    26,   256,   245,   225,    28,
   286,   623,   139,   211,   139,    32,    26,   503,     5,     5,
    28,   272,   310,   388,   487,     5,    32,   488,   147,   440,
   320,   736,   115,   500,   619,   249,   241,   260,   297,   620,
    26,   129,    26,   119,    28,   115,    28,     5,   113,   146,
   689,   250,     5,   272,   128,   204,   667,   137,   298,   129,
   302,   305,   130,   669,    32,    32,   312,   472,   706,   147,
   311,     5,   322,   147,   425,   214,   113,   489,    26,    32,
   240,   147,    28,   147,   243,   223,   330,    32,    32,   256,
   189,   126,   333,   143,   696,   129,   126,   338,   130,   509,
   473,   440,   120,    26,    26,   325,   228,    28,    28,   476,
    26,     5,   728,   144,    28,    26,    32,   121,   364,    28,
   208,   129,   189,   377,   369,   179,   262,   129,   697,   147,
   130,   179,    26,   407,     5,   149,    28,    26,   265,   188,
   156,    28,    32,   125,   340,     3,   711,   158,   615,   254,
   312,    32,   139,   161,    32,    26,    26,   241,   262,    28,
    28,   147,   272,   127,   162,   658,     3,     5,   434,   266,
   263,   163,   129,   768,   269,   169,   531,   477,   445,    26,
   275,   426,   171,    28,   510,   279,   170,   274,   511,   491,
   115,   717,   496,   416,   504,   124,    26,   505,   726,   115,
    28,   312,   256,   139,   731,   432,   303,   306,   189,   377,
   517,   126,   313,   129,   316,   602,   130,   474,   428,    26,
   478,  -411,   125,    28,   444,    32,   172,   485,   492,   173,
    32,   189,   174,   256,   179,   603,   129,   252,    32,   130,
   771,   175,   127,   147,   623,   682,   530,   470,   429,   518,
   249,   438,    26,   253,    19,    20,    28,   249,   573,   367,
   580,   214,   112,   483,   340,   147,   250,    78,   111,    32,
   204,   182,   130,   250,   106,   368,   778,   126,   437,   112,
     3,   112,   588,   183,    78,   111,   302,   502,   526,   527,
   107,   106,   528,   714,    79,   715,    80,    32,    81,   523,
   576,   367,    87,    89,   135,   139,   567,   107,   470,    91,
   652,    79,   330,    80,   185,    81,   338,   368,    92,    87,
    89,   245,   626,   137,   139,   186,    91,    94,   783,   367,
   651,   210,    95,   597,    96,    92,   595,   495,   492,    98,
   790,   791,    99,   239,    94,   368,   100,   258,   589,    95,
   108,    96,   110,   589,   723,   621,    98,   724,   634,    99,
     5,   567,   270,   100,    19,    20,   277,   108,   622,   110,
   591,    19,    20,   278,   589,   591,   425,     5,   285,   112,
   746,   589,   288,   747,    78,   111,   282,   283,   659,   289,
   118,   106,   588,   662,   112,     3,   591,   638,   208,   138,
   614,   293,   639,   591,   142,   272,   610,   107,    32,   307,
   292,    79,   792,    80,   787,    81,   680,   788,   494,    87,
    89,   265,   611,    45,   789,   492,    91,    32,   165,   168,
   179,   693,   663,   513,   324,    92,   725,   567,   122,   294,
   501,   362,   363,   781,    94,   638,   782,   645,   299,    95,
   195,    96,   691,   300,   369,   319,    98,   228,   327,    99,
   192,   328,    26,   100,    45,   332,    28,   108,   694,   110,
    57,    58,    59,   370,   189,   115,   373,   750,   336,   129,
   335,   501,   130,   426,   486,   209,   147,   393,   394,    32,
   339,   221,    21,    22,   112,   244,    74,   247,   523,    78,
   111,   343,   685,   347,   257,   139,   106,   567,    32,    24,
    25,    57,    58,    59,   487,     5,   589,   488,   351,   378,
   382,   192,   107,    32,   276,   567,    79,   129,    80,    32,
    81,   284,   386,   760,    87,    89,   765,   389,   591,   390,
   391,    91,   392,   272,   395,   396,   138,   421,   204,   397,
    92,   402,   398,   403,   760,   314,   244,    32,   318,    94,
   118,   192,   326,   404,    95,   139,    96,   489,   405,   567,
     5,    98,   734,   312,    99,   115,   408,   410,   100,   645,
   409,   334,   108,   645,   110,     5,   411,   342,   236,    19,
    20,   461,   344,   125,    32,   414,   415,    32,   272,   418,
    26,   679,   464,   419,    28,   420,   139,   365,   366,   436,
   589,   439,    32,   127,   589,   255,   761,   423,   466,     5,
   130,   244,   480,   481,   228,   244,   589,   497,   498,   589,
   567,    32,   591,     5,    32,   506,   591,   761,     5,   314,
   507,   589,   589,   701,   702,   703,   514,   272,   591,   520,
   763,   591,   524,   399,   525,    26,   569,    32,    32,    28,
   582,   240,   585,   591,   591,     5,   323,   189,   587,   255,
    26,   592,   129,   606,    28,   130,   605,   126,   618,   413,
   607,   627,   417,   633,   244,   630,   244,   635,   629,   345,
   422,   346,  -141,   124,   348,   349,   350,   631,   641,   649,
   435,   650,   632,   653,    26,     5,   125,   655,    28,   126,
   413,   656,   372,   660,   664,   375,   189,   665,    26,   674,
   683,   129,    28,    26,   130,   686,   127,    28,   692,   699,
   601,   458,   376,   695,   700,   189,   380,   708,   241,   710,
   129,   712,   716,   130,     5,   244,   244,     5,   718,   625,
    26,   713,   719,   727,    28,   360,   361,   362,   363,   729,
   192,     5,   128,   192,   428,   740,   741,   129,   742,   744,
   130,   745,   240,   748,   749,   240,     5,   751,     5,   753,
   755,   192,   413,   244,   756,   777,   244,   757,   126,   315,
    26,   126,   766,   769,    28,   376,   772,   380,   486,   784,
   147,   571,   189,   785,   315,   795,   315,   129,   786,   796,
   130,   654,   138,   793,   608,   446,   447,   448,   449,   657,
   451,   452,   453,   454,   455,   456,   508,   244,   598,    26,
   463,   488,    26,    28,   687,   244,    28,   609,   599,   776,
   613,   340,   457,   309,   468,   681,    26,   572,   670,   241,
    28,   688,   241,   433,   570,   141,   469,   471,   317,   738,
   678,    26,   579,    26,   775,    28,   241,    28,   767,   739,
   116,   600,   352,   379,     0,   468,     0,     0,     0,   709,
     0,   241,     0,   241,     0,     0,   244,   358,   359,   360,
   361,   362,   363,   192,   457,     0,     0,   469,     0,     0,
     0,   441,   147,   574,     0,   455,     0,   197,   198,   199,
   200,   201,   202,   203,     0,     0,   730,     0,     0,   735,
   353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
   363,     0,     0,   640,   743,     0,     0,     0,   581,     6,
     7,     8,     9,    10,    11,    12,   584,    14,     0,     0,
     0,     0,     0,   752,     0,     0,   754,     0,     0,   192,
     0,     0,     0,     0,   192,   353,   354,   355,   356,   442,
   358,   359,   360,   361,   443,   363,     0,     0,     0,   773,
   774,     0,     0,     0,     0,     0,     0,   192,   356,   357,
   358,   359,   360,   361,   362,   363,     5,   581,     0,     0,
     0,     0,   192,     0,     0,   192,   698,     0,     0,     0,
     0,   705,     6,     7,     8,     9,    10,    11,    12,    13,
    14,    15,    16,    17,   214,   357,   358,   359,   360,   361,
   362,   363,    18,    19,    20,    21,    22,   647,     0,   648,
   126,   355,   356,   357,   358,   359,   360,   361,   362,   363,
   722,    23,    24,    25,     0,     0,     0,     0,     0,     0,
     0,     0,   422,   353,   354,   355,   356,   357,   358,   359,
   360,   361,   362,   363,     0,     0,     0,     0,     0,     0,
   677,    26,     0,     0,    27,    28,     0,     0,     0,     0,
     0,     0,     0,   215,     0,     0,     0,     0,   129,     0,
     0,     0,     0,     0,   192,     0,   762,   192,     0,     0,
   532,   707,  -411,    41,     0,   722,     0,     0,    44,     0,
     0,     0,     0,     0,     0,   192,     0,    45,  -411,  -411,
  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
     0,     0,     0,   533,    47,     0,     0,  -411,     0,  -411,
  -411,  -411,  -411,     0,     0,     0,     0,     0,     0,    49,
    50,    51,    52,   534,    54,    55,    56,  -411,  -411,  -411,
   535,   536,   537,     0,    57,   538,    59,     0,    60,    61,
     0,     0,     0,    65,     0,    67,    68,    69,    70,     0,
    72,     0,     0,     0,     0,     0,     0,  -411,     0,     0,
    74,  -411,  -411,     0,     0,     0,     0,     0,     0,     0,
     5,     0,     0,     0,     0,     0,     0,   539,   197,   198,
   199,   200,   201,   202,   203,     0,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    16,    17,   229,     0,
     0,     0,     0,     0,     0,     0,     0,    19,    20,    21,
    22,     0,   230,     0,     0,   231,     0,     0,     0,     0,
     0,     0,     0,     5,     0,    23,    24,    25,     0,     0,
     0,   197,   198,   199,   200,   201,   202,   203,     0,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,   229,     0,     0,     0,    26,     0,     0,     0,    28,
    19,    20,    21,    22,     0,   230,     0,     0,   371,     0,
     0,   232,     0,     0,     0,   233,   234,     5,    23,    24,
    25,     0,     0,     0,     0,   197,   198,   199,   200,   201,
   202,   203,     0,     6,     7,     8,     9,    10,    11,    12,
    13,    14,    15,    16,    17,   229,     0,     0,    26,     0,
     0,     0,    28,     0,    19,    20,    21,    22,     0,   230,
     0,     0,   374,     0,   232,     0,     0,     0,   233,   234,
     5,     0,    23,    24,    25,     0,     0,     0,   197,   198,
   199,   200,   201,   202,   203,     0,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    16,    17,   229,     0,
     0,     0,    26,     0,     0,     0,    28,    19,    20,    21,
    22,     0,   230,     0,     0,   462,     0,     0,   232,     0,
     0,     0,   233,   234,     5,    23,    24,    25,     0,     0,
     0,     0,   197,   198,   199,   200,   201,   202,   203,     0,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,   229,   575,     0,    26,     0,     0,     0,    28,
     0,    19,    20,    21,    22,     0,   230,     0,     0,     0,
     0,   232,     0,     0,     0,   233,   234,     5,     0,    23,
    24,    25,     0,     0,     0,   197,   198,   199,   200,   201,
   202,   203,     0,     6,     7,     8,     9,    10,    11,    12,
    13,    14,    15,    16,    17,   229,     0,     0,     0,    26,
     0,     0,     0,    28,    19,    20,    21,    22,     0,   230,
     0,     0,     0,     0,     0,   232,     0,     0,     0,   233,
   234,     5,    23,    24,    25,     0,     0,     0,   675,   197,
   198,   199,   200,   201,   202,   203,     0,     6,     7,     8,
     9,    10,    11,    12,    13,    14,    15,    16,    17,   229,
     0,     0,    26,     0,     0,     0,    28,     0,    19,    20,
    21,    22,     0,   230,     0,     0,     0,     0,   232,   129,
     0,     0,   233,   234,     5,     0,    23,    24,    25,     0,
     0,     0,   197,   198,   199,   200,   201,   202,   203,     0,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,   229,     0,     0,     0,    26,     0,     0,     0,
    28,    19,    20,    21,    22,     0,   230,     5,     0,     0,
     0,     0,   232,     0,     0,     0,   233,   234,     0,    23,
    24,    25,     0,     6,     7,     8,     9,    10,    11,    12,
    13,    14,    15,    16,    17,     0,     0,     0,     0,     0,
     0,     0,     0,    18,    19,    20,    21,    22,     0,    26,
     0,     0,     0,    28,     0,     0,     0,     0,     5,     0,
     0,     0,    23,    24,    25,   232,     0,     0,     0,   233,
   234,     0,     0,     0,     6,     7,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,     0,     0,     0,     0,
     0,     0,    26,     0,     0,    27,    28,    21,    22,    -1,
    40,     0,  -411,    41,     0,     0,    42,    43,    44,   129,
     0,     0,     0,    23,    24,    25,     0,    45,  -411,  -411,
  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
     0,     0,     0,    46,    47,     0,     0,     0,     0,  -411,
  -411,  -411,  -411,    26,     0,    48,     0,    28,     0,    49,
    50,    51,    52,    53,    54,    55,    56,  -411,  -411,  -411,
   129,     0,     0,     0,    57,    58,    59,     0,    60,    61,
    62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
    72,     0,     0,     0,     0,    73,    40,  -411,  -411,    41,
    74,  -411,    42,    43,    44,     0,     0,     0,     0,     0,
     0,     0,     0,    45,  -411,  -411,  -411,  -411,  -411,  -411,
  -411,  -411,  -411,  -411,  -411,  -411,     0,     0,     0,    46,
    47,     0,     0,   475,     0,  -411,  -411,  -411,  -411,     0,
     0,    48,     0,     0,     0,    49,    50,    51,    52,    53,
    54,    55,    56,  -411,  -411,  -411,     0,     0,     0,     0,
    57,    58,    59,     0,    60,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    70,    71,    72,     0,     0,     0,
     0,    73,    40,  -411,  -411,    41,    74,  -411,    42,    43,
    44,     0,     0,     0,     0,     0,     0,     0,     0,    45,
  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
  -411,  -411,     0,     0,     0,    46,    47,     0,     0,   499,
     0,  -411,  -411,  -411,  -411,     0,     0,    48,     0,     0,
     0,    49,    50,    51,    52,    53,    54,    55,    56,  -411,
  -411,  -411,     0,     0,     0,     0,    57,    58,    59,     0,
    60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
    70,    71,    72,     0,     0,     0,     0,    73,    40,  -411,
  -411,    41,    74,  -411,    42,    43,    44,     0,     0,     0,
     0,     0,     0,     0,     0,    45,  -411,  -411,  -411,  -411,
  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,     0,     0,
     0,    46,    47,     0,     0,   594,     0,  -411,  -411,  -411,
  -411,     0,     0,    48,     0,     0,     0,    49,    50,    51,
    52,    53,    54,    55,    56,  -411,  -411,  -411,     0,     0,
     0,     0,    57,    58,    59,     0,    60,    61,    62,    63,
    64,    65,    66,    67,    68,    69,    70,    71,    72,     0,
     0,     0,     0,    73,    40,  -411,  -411,    41,    74,  -411,
    42,    43,    44,     0,     0,     0,     0,     0,     0,     0,
     0,    45,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
  -411,  -411,  -411,  -411,     0,     0,     0,    46,    47,     0,
     0,     0,     0,  -411,  -411,  -411,  -411,     0,     0,    48,
     0,   673,     0,    49,    50,    51,    52,    53,    54,    55,
    56,  -411,  -411,  -411,     0,     0,     0,     0,    57,    58,
    59,     0,    60,    61,    62,    63,    64,    65,    66,    67,
    68,    69,    70,    71,    72,     4,     0,     5,     0,    73,
     0,  -411,     0,     0,    74,  -411,     0,     0,     0,     0,
     0,     0,     0,     6,     7,     8,     9,    10,    11,    12,
    13,    14,    15,    16,    17,     0,     0,     0,     0,     0,
     0,     0,     0,    18,    19,    20,    21,    22,   152,     0,
   153,   354,   355,   356,   357,   358,   359,   360,   361,   362,
   363,     0,    23,    24,    25,     0,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    16,    17,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    19,    20,    21,
    22,     0,    26,     5,     0,    27,    28,     0,     0,     0,
     0,     0,     0,   465,     0,    23,    24,    25,     0,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    19,    20,    21,    22,     0,    26,     0,     0,     0,    28,
     0,     0,     0,     0,     0,     0,     0,     0,   193,    24,
    25,   353,   354,   355,   356,   357,   358,   359,   360,   361,
   362,   363,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     5,     0,    26,   196,
     0,     0,    28,   637,   197,   198,   199,   200,   201,   202,
   203,     0,     6,     7,     8,     9,    10,    11,    12,    13,
    14,    15,    16,    17,     0,     0,     0,     0,     0,     0,
     0,     0,    18,    19,    20,    21,    22,     0,     5,     0,
     0,     0,     0,     0,     0,     0,   280,     0,     0,     0,
     0,    23,    24,    25,     6,     7,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,     0,     0,     0,     0,
     0,     0,     0,     0,    18,    19,    20,    21,    22,     0,
     5,    26,     0,     0,    27,    28,     0,     0,     0,     0,
     0,     0,     0,    23,    24,    25,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    16,    17,     0,     0,
     0,     0,     0,     0,     0,     0,    18,    19,    20,    21,
    22,     0,     5,    26,     0,     0,    27,    28,     0,     0,
     0,     0,     0,     0,     0,    23,    24,    25,     6,     7,
     8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
   177,     0,     0,     0,     0,     0,     0,     0,     0,    19,
    20,    21,    22,     0,     5,    26,     0,     0,    27,    28,
     0,     0,     0,     0,     0,     0,     0,    23,    24,    25,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    19,    20,    21,    22,     0,     5,    26,     0,     0,
     0,    28,     0,     0,     0,     0,     0,     0,     0,   193,
    24,    25,     6,     7,     8,     9,    10,    11,    12,    13,
    14,    15,    16,    17,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    19,    20,    21,    22,     0,   596,    26,
     0,     0,     0,    28,     0,     0,     0,     0,     0,     0,
     0,    23,    24,    25,     6,     7,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    19,    20,    21,    22,     0,
     5,    26,     0,     0,     0,    28,     0,     0,     0,     0,
   467,     0,     0,    23,    24,    25,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    16,    17,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
    22,     0,   583,    26,     0,     0,     0,    28,     0,     0,
     0,     0,     0,     0,     0,    23,    24,    25,   353,   354,
   355,   356,   357,   358,   359,   360,   361,   362,   363,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    26,     0,     0,     0,    28,
   353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
   363
};

static const short yycheck[] = {     0,
    49,     2,    47,   133,     5,   135,     2,     2,    31,    93,
    33,    32,    30,     2,   271,    16,   171,    32,   266,    68,
    69,    68,    67,     1,   482,   113,   220,    72,   294,     2,
    32,    32,     6,     2,     6,     2,     4,     2,    10,   287,
     3,     2,     2,   212,     3,    32,    31,    48,     2,     6,
    44,     3,    53,    54,    53,    32,    37,     2,   119,    34,
    34,    32,   111,     1,    94,    37,     2,   111,     3,    37,
   154,     2,    82,     2,    89,     3,   120,     3,     2,    31,
   125,     2,   143,   144,    85,     2,    85,     3,    82,     2,
    47,     2,   261,    33,     3,   179,    40,    41,   491,   122,
   558,    82,    89,   496,    44,    31,   129,    82,    71,    37,
   194,    82,   113,     3,     4,   100,     6,   128,   312,    39,
   105,    47,   124,   100,   517,    88,   137,   129,   100,    92,
    44,   524,   133,    92,   135,   113,    88,   403,     3,     3,
    92,    31,   187,   273,    34,     3,   124,    37,     6,   343,
     3,    37,   153,   401,    33,    31,   108,    92,   179,    38,
    88,   105,    88,   110,    92,   166,    92,     3,    82,    31,
   628,    47,     3,    31,   100,   113,    92,   179,   179,   105,
   181,   182,   108,    92,   162,   163,   188,     4,   646,     6,
     3,     3,   193,     6,   324,    31,    82,    87,    88,   177,
    31,     6,    92,     6,   215,    10,   207,   185,   186,   220,
   100,    47,   214,    90,     3,   105,    47,   218,   108,     3,
    37,   415,    82,    88,    88,    37,   271,    92,    92,   384,
    88,     3,   690,   110,    92,    88,   214,    34,   240,    92,
   328,   105,   100,   254,   246,   294,     3,   105,    37,     6,
   108,   300,    88,   300,     3,    31,    92,    88,     6,    31,
    31,    92,   240,    44,   100,   266,   659,    31,   516,   100,
   272,   249,   273,    31,   252,    88,    88,   108,     3,    92,
    92,     6,    31,    64,    31,    34,   287,     3,   332,    37,
   146,    31,   105,   751,   150,    31,   426,   385,   347,    88,
   156,   324,    31,    92,    88,   161,     4,    32,    92,   393,
   311,   680,   396,   315,    34,    31,    88,    37,   687,   320,
    92,   323,   333,   324,   693,   326,   182,   183,   100,   340,
   414,    47,   188,   105,   190,    90,   108,   382,    34,    88,
   385,    37,    44,    92,   346,   323,    31,   392,   393,    34,
   328,   100,    37,   364,   403,   110,   105,    31,   336,   108,
   753,     4,    64,     6,   757,   622,    68,   378,    64,   414,
    31,    32,    88,    47,    40,    41,    92,    31,    32,    31,
    32,    31,   383,     4,   100,     6,    47,   383,   383,   367,
   328,    31,   108,    47,   383,    47,   765,    47,   336,   400,
   401,   402,    68,    31,   400,   400,   407,   402,    33,    34,
   383,   400,    37,   666,   383,   668,   383,   395,   383,   420,
   443,    31,   383,   383,   445,   426,   427,   400,   439,   383,
   585,   400,   433,   400,    31,   400,   437,    47,   383,   400,
   400,   443,   526,   445,   445,    82,   400,   383,   772,    31,
    32,    39,   383,   502,   383,   400,   501,   395,   503,   383,
   784,   785,   383,    11,   400,    47,   383,    89,   491,   400,
   383,   400,   383,   496,    34,    31,   400,    37,   539,   400,
     3,   482,    34,   400,    40,    41,     3,   400,    44,   400,
   491,    40,    41,     3,   517,   496,   626,     3,    34,   500,
    34,   524,    37,    37,   500,   500,   162,   163,   592,     3,
    23,   500,    68,   597,   515,   516,   517,   566,   606,    32,
   515,   177,   566,   524,    37,    31,   515,   500,   506,   185,
     3,   500,   789,   500,    34,   500,   620,    37,   394,   500,
   500,     6,   515,    18,    44,   590,   500,   525,    61,    62,
   599,   635,   599,   409,   638,   500,   686,   558,   576,    37,
    35,   105,   106,    34,   500,   614,    37,   568,    34,   500,
   614,   500,   633,    33,   576,    89,   500,   622,    83,   500,
    93,    33,    88,   500,    18,    83,    92,   500,   637,   500,
    65,    66,    67,   249,   100,   596,   252,   727,    33,   105,
    32,    35,   108,   626,     4,   606,     6,    32,    33,   587,
    32,   124,    42,    43,   615,   128,    91,   130,   619,   615,
   615,    89,   623,    31,   137,   626,   615,   628,   606,    59,
    60,    65,    66,    67,    34,     3,   659,    37,    31,    89,
    32,   154,   615,   621,   157,   646,   615,   105,   615,   627,
   615,   164,    32,   737,   615,   615,   740,    32,   659,    37,
    32,   615,    44,    31,    33,    32,   179,   323,   606,    32,
   615,    32,    34,    32,   758,   188,   189,   655,   191,   615,
   193,   194,   195,    32,   615,   686,   615,    87,    32,   690,
     3,   615,   694,   695,   615,   696,    32,    32,   615,   700,
    33,   214,   615,   704,   615,     3,    83,   220,   126,    40,
    41,   367,   225,    44,   692,    32,    89,   695,    31,    34,
    88,    34,    32,    89,    92,    37,   727,   240,   241,    89,
   753,    89,   710,    64,   757,   137,   737,    68,    32,     3,
   108,   254,    34,    34,   789,   258,   769,    37,    34,   772,
   751,   729,   753,     3,   732,    31,   757,   758,     3,   272,
    38,   784,   785,    61,    62,    63,    34,    31,   769,     3,
    34,   772,    32,   286,    31,    88,    37,   755,   756,    92,
    32,    31,    33,   784,   785,     3,    31,   100,    31,   191,
    88,    32,   105,    82,    92,   108,   110,    47,    44,   312,
    32,    31,   315,   108,   317,    64,   319,    38,    37,   227,
   323,   229,    38,    31,   232,   233,   234,    64,    66,    32,
   333,    32,    64,    38,    88,     3,    44,    31,    92,    47,
   343,    68,   250,    34,    32,   253,   100,     3,    88,    34,
    64,   105,    92,    88,   108,    32,    64,    92,    31,    34,
   506,   364,   254,    31,    33,   100,   258,    32,   108,    31,
   105,    34,    83,   108,     3,   378,   379,     3,    32,   525,
    88,    34,    34,    32,    92,   103,   104,   105,   106,    31,
   393,     3,   100,   396,    34,    38,    34,   105,    32,    34,
   108,    34,    31,    33,    31,    31,     3,    38,     3,    32,
    31,   414,   415,   416,    31,    34,   419,    32,    47,    31,
    88,    47,    32,    32,    92,   317,    32,   319,     4,    32,
     6,   434,   100,    32,    31,     0,    31,   105,    34,     0,
   108,   587,   445,    34,   515,   353,   354,   355,   356,   590,
   358,   359,   360,   361,   362,   363,   407,   460,    34,    88,
   368,    37,    88,    92,   626,   468,    92,   515,    44,   758,
   515,   100,   364,   186,   100,   621,    88,   437,   606,   108,
    92,   627,   108,   328,   433,    36,   378,   379,   100,   700,
   619,    88,   445,    88,   757,    92,   108,    92,   748,   704,
    16,   503,    48,   100,    -1,   100,    -1,    -1,    -1,   655,
    -1,   108,    -1,   108,    -1,    -1,   519,   101,   102,   103,
   104,   105,   106,   526,   416,    -1,    -1,   419,    -1,    -1,
    -1,    32,     6,   441,    -1,   443,    -1,    11,    12,    13,
    14,    15,    16,    17,    -1,    -1,   692,    -1,    -1,   695,
    96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
   106,    -1,    -1,   566,   710,    -1,    -1,    -1,   460,    19,
    20,    21,    22,    23,    24,    25,   468,    27,    -1,    -1,
    -1,    -1,    -1,   729,    -1,    -1,   732,    -1,    -1,   592,
    -1,    -1,    -1,    -1,   597,    96,    97,    98,    99,   100,
   101,   102,   103,   104,   105,   106,    -1,    -1,    -1,   755,
   756,    -1,    -1,    -1,    -1,    -1,    -1,   620,    99,   100,
   101,   102,   103,   104,   105,   106,     3,   519,    -1,    -1,
    -1,    -1,   635,    -1,    -1,   638,   639,    -1,    -1,    -1,
    -1,   644,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,   100,   101,   102,   103,   104,
   105,   106,    39,    40,    41,    42,    43,   575,    -1,   577,
    47,    98,    99,   100,   101,   102,   103,   104,   105,   106,
   683,    58,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   695,    96,    97,    98,    99,   100,   101,   102,
   103,   104,   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,
   618,    88,    -1,    -1,    91,    92,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,   105,    -1,
    -1,    -1,    -1,    -1,   737,    -1,   739,   740,    -1,    -1,
     1,   649,     3,     4,    -1,   748,    -1,    -1,     9,    -1,
    -1,    -1,    -1,    -1,    -1,   758,    -1,    18,    19,    20,
    21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
    -1,    -1,    -1,    34,    35,    -1,    -1,    38,    -1,    40,
    41,    42,    43,    -1,    -1,    -1,    -1,    -1,    -1,    50,
    51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
    61,    62,    63,    -1,    65,    66,    67,    -1,    69,    70,
    -1,    -1,    -1,    74,    -1,    76,    77,    78,    79,    -1,
    81,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
    91,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     3,    -1,    -1,    -1,    -1,    -1,    -1,   108,    11,    12,
    13,    14,    15,    16,    17,    -1,    19,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,
    43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,     3,    -1,    58,    59,    60,    -1,    -1,
    -1,    11,    12,    13,    14,    15,    16,    17,    -1,    19,
    20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
    30,    31,    -1,    -1,    -1,    88,    -1,    -1,    -1,    92,
    40,    41,    42,    43,    -1,    45,    -1,    -1,    48,    -1,
    -1,   104,    -1,    -1,    -1,   108,   109,     3,    58,    59,
    60,    -1,    -1,    -1,    -1,    11,    12,    13,    14,    15,
    16,    17,    -1,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    -1,    -1,    88,    -1,
    -1,    -1,    92,    -1,    40,    41,    42,    43,    -1,    45,
    -1,    -1,    48,    -1,   104,    -1,    -1,    -1,   108,   109,
     3,    -1,    58,    59,    60,    -1,    -1,    -1,    11,    12,
    13,    14,    15,    16,    17,    -1,    19,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
    -1,    -1,    88,    -1,    -1,    -1,    92,    40,    41,    42,
    43,    -1,    45,    -1,    -1,    48,    -1,    -1,   104,    -1,
    -1,    -1,   108,   109,     3,    58,    59,    60,    -1,    -1,
    -1,    -1,    11,    12,    13,    14,    15,    16,    17,    -1,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    31,    32,    -1,    88,    -1,    -1,    -1,    92,
    -1,    40,    41,    42,    43,    -1,    45,    -1,    -1,    -1,
    -1,   104,    -1,    -1,    -1,   108,   109,     3,    -1,    58,
    59,    60,    -1,    -1,    -1,    11,    12,    13,    14,    15,
    16,    17,    -1,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    -1,    -1,    -1,    88,
    -1,    -1,    -1,    92,    40,    41,    42,    43,    -1,    45,
    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,   108,
   109,     3,    58,    59,    60,    -1,    -1,    -1,    10,    11,
    12,    13,    14,    15,    16,    17,    -1,    19,    20,    21,
    22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,    40,    41,
    42,    43,    -1,    45,    -1,    -1,    -1,    -1,   104,   105,
    -1,    -1,   108,   109,     3,    -1,    58,    59,    60,    -1,
    -1,    -1,    11,    12,    13,    14,    15,    16,    17,    -1,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    31,    -1,    -1,    -1,    88,    -1,    -1,    -1,
    92,    40,    41,    42,    43,    -1,    45,     3,    -1,    -1,
    -1,    -1,   104,    -1,    -1,    -1,   108,   109,    -1,    58,
    59,    60,    -1,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    39,    40,    41,    42,    43,    -1,    88,
    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,     3,    -1,
    -1,    -1,    58,    59,    60,   104,    -1,    -1,    -1,   108,
   109,    -1,    -1,    -1,    19,    20,    21,    22,    23,    24,
    25,    26,    27,    28,    29,    30,    -1,    -1,    -1,    -1,
    -1,    -1,    88,    -1,    -1,    91,    92,    42,    43,     0,
     1,    -1,     3,     4,    -1,    -1,     7,     8,     9,   105,
    -1,    -1,    -1,    58,    59,    60,    -1,    18,    19,    20,
    21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
    -1,    -1,    -1,    34,    35,    -1,    -1,    -1,    -1,    40,
    41,    42,    43,    88,    -1,    46,    -1,    92,    -1,    50,
    51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
   105,    -1,    -1,    -1,    65,    66,    67,    -1,    69,    70,
    71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
    81,    -1,    -1,    -1,    -1,    86,     1,    88,     3,     4,
    91,    92,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    18,    19,    20,    21,    22,    23,    24,
    25,    26,    27,    28,    29,    30,    -1,    -1,    -1,    34,
    35,    -1,    -1,    38,    -1,    40,    41,    42,    43,    -1,
    -1,    46,    -1,    -1,    -1,    50,    51,    52,    53,    54,
    55,    56,    57,    58,    59,    60,    -1,    -1,    -1,    -1,
    65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
    75,    76,    77,    78,    79,    80,    81,    -1,    -1,    -1,
    -1,    86,     1,    88,     3,     4,    91,    92,     7,     8,
     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    -1,    -1,    -1,    34,    35,    -1,    -1,    38,
    -1,    40,    41,    42,    43,    -1,    -1,    46,    -1,    -1,
    -1,    50,    51,    52,    53,    54,    55,    56,    57,    58,
    59,    60,    -1,    -1,    -1,    -1,    65,    66,    67,    -1,
    69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
    79,    80,    81,    -1,    -1,    -1,    -1,    86,     1,    88,
     3,     4,    91,    92,     7,     8,     9,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    -1,    -1,
    -1,    34,    35,    -1,    -1,    38,    -1,    40,    41,    42,
    43,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    52,
    53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
    -1,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
    73,    74,    75,    76,    77,    78,    79,    80,    81,    -1,
    -1,    -1,    -1,    86,     1,    88,     3,     4,    91,    92,
     7,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    -1,    -1,    -1,    34,    35,    -1,
    -1,    -1,    -1,    40,    41,    42,    43,    -1,    -1,    46,
    -1,    48,    -1,    50,    51,    52,    53,    54,    55,    56,
    57,    58,    59,    60,    -1,    -1,    -1,    -1,    65,    66,
    67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
    77,    78,    79,    80,    81,     1,    -1,     3,    -1,    86,
    -1,    88,    -1,    -1,    91,    92,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    39,    40,    41,    42,    43,     1,    -1,
     3,    97,    98,    99,   100,   101,   102,   103,   104,   105,
   106,    -1,    58,    59,    60,    -1,    19,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,
    43,    -1,    88,     3,    -1,    91,    92,    -1,    -1,    -1,
    -1,    -1,    -1,    48,    -1,    58,    59,    60,    -1,    19,
    20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    40,    41,    42,    43,    -1,    88,    -1,    -1,    -1,    92,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,
    60,    96,    97,    98,    99,   100,   101,   102,   103,   104,
   105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    88,     6,
    -1,    -1,    92,    93,    11,    12,    13,    14,    15,    16,
    17,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    39,    40,    41,    42,    43,    -1,     3,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,
    -1,    58,    59,    60,    19,    20,    21,    22,    23,    24,
    25,    26,    27,    28,    29,    30,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    39,    40,    41,    42,    43,    -1,
     3,    88,    -1,    -1,    91,    92,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    58,    59,    60,    19,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    39,    40,    41,    42,
    43,    -1,     3,    88,    -1,    -1,    91,    92,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    58,    59,    60,    19,    20,
    21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
    41,    42,    43,    -1,     3,    88,    -1,    -1,    91,    92,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    60,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    40,    41,    42,    43,    -1,     3,    88,    -1,    -1,
    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,
    59,    60,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    40,    41,    42,    43,    -1,     3,    88,
    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    58,    59,    60,    19,    20,    21,    22,    23,    24,
    25,    26,    27,    28,    29,    30,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    40,    41,    42,    43,    -1,
     3,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
    48,    -1,    -1,    58,    59,    60,    19,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,
    43,    -1,    48,    88,    -1,    -1,    -1,    92,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    58,    59,    60,    96,    97,
    98,    99,   100,   101,   102,   103,   104,   105,   106,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    92,
    96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
   106
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/local/share/bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "/usr/local/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 909 "parser.y"
{
		   Setattr(yyvsp[0].node,"classes",classes); 
		   Setattr(yyvsp[0].node,"name",ModuleName);
		   
		   if ((!module_node) && ModuleName) {
		     module_node = new_node("module");
		     Setattr(module_node,"name",ModuleName);
		   }
		   Setattr(yyvsp[0].node,"module",module_node);
		   check_extensions();
	           top = yyvsp[0].node;
               ;
    break;}
case 2:
#line 921 "parser.y"
{
                 top = Getattr(yyvsp[-1].p,"type");
               ;
    break;}
case 3:
#line 924 "parser.y"
{
                 top = 0;
               ;
    break;}
case 4:
#line 929 "parser.y"
{  
                   appendChild(yyvsp[-1].node,yyvsp[0].node);
                   yyval.node = yyvsp[-1].node;
               ;
    break;}
case 5:
#line 933 "parser.y"
{
                   yyval.node = new_node("top");
               ;
    break;}
case 6:
#line 938 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 7:
#line 939 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 8:
#line 940 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 9:
#line 941 "parser.y"
{ yyval.node = 0; ;
    break;}
case 10:
#line 942 "parser.y"
{
                  yyval.node = 0;
		  if (!Swig_error_count()) {
		    static int last_error_line = -1;
		    if (last_error_line != cparse_line) {
		      Swig_error(cparse_file, cparse_line,"Syntax error in input.\n");
		      last_error_line = cparse_line;
		      skip_decl();
		    }
		  }
               ;
    break;}
case 11:
#line 954 "parser.y"
{ 
                  if (yyval.node) {
   		      add_symbols(yyval.node);
                  }
                  yyval.node = yyvsp[0].node; 
               ;
    break;}
case 12:
#line 967 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 13:
#line 968 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 14:
#line 969 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 15:
#line 970 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 16:
#line 971 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 17:
#line 972 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 18:
#line 973 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 19:
#line 974 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 20:
#line 975 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 21:
#line 976 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 22:
#line 977 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 23:
#line 978 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 24:
#line 979 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 25:
#line 980 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 26:
#line 981 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 27:
#line 982 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 28:
#line 983 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 29:
#line 984 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 30:
#line 985 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 31:
#line 986 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 32:
#line 987 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 33:
#line 994 "parser.y"
{
               Node *cls;
	       String *clsname;
	       cplus_mode = CPLUS_PUBLIC;
	       if (!classes) classes = NewHash();
	       if (!extendhash) extendhash = NewHash();
	       clsname = make_class_name(yyvsp[-1].str);
	       /*	       Printf(stdout,"clsname = '%s'\n",clsname);*/
	       cls = Getattr(classes,clsname);
	       if (!cls) {
		 /* No previous definition. Create a new scope */
		 Node *am = Getattr(extendhash,clsname);
		 if (!am) {
		   Swig_symbol_newscope();
		   Swig_symbol_setscopename(yyvsp[-1].str);
		   prev_symtab = 0;
		 } else {
		   prev_symtab = Swig_symbol_setscope(Getattr(am,"symtab"));
		 }
		 current_class = 0;
	       } else {
		 /* Previous class definition.  Use its symbol table */
		 prev_symtab = Swig_symbol_setscope(Getattr(cls,"symtab"));
		 current_class = cls;
	       }
	       Classprefix = NewString(yyvsp[-1].str);
	       Namespaceprefix= Swig_symbol_qualifiedscopename(0);
	       Delete(clsname);
	     ;
    break;}
case 34:
#line 1022 "parser.y"
{
               String *clsname;
               yyval.node = new_node("extend");
	       Setattr(yyval.node,"symtab",Swig_symbol_popscope());
	       if (prev_symtab) {
		 Swig_symbol_setscope(prev_symtab);
	       }
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
               clsname = make_class_name(yyvsp[-4].str);
	       Setattr(yyval.node,"name",clsname);

	       /* Mark members as extend */

	       Swig_tag_nodes(yyvsp[-1].node,"feature:extend",(char*) "1");
	       if (current_class) {
		 /* We add the extension to the previously defined class */
		 appendChild(yyval.node,yyvsp[-1].node);
		 appendChild(current_class,yyval.node);
	       } else {
		 /* We store the extensions in the extensions hash */
		 Node *am = Getattr(extendhash,clsname);
		 if (am) {
		   /* Append the members to the previous extend methods */
		   appendChild(am,yyvsp[-1].node);
		 } else {
		   appendChild(yyval.node,yyvsp[-1].node);
		   Setattr(extendhash,clsname,yyval.node);
		 }
	       }
	       current_class = 0;
	       Delete(Classprefix);
	       Delete(clsname);
	       Classprefix = 0;
	       prev_symtab = 0;
	       yyval.node = 0;
	     ;
    break;}
case 35:
#line 1064 "parser.y"
{
                    yyval.node = new_node("apply");
                    Setattr(yyval.node,"pattern",Getattr(yyvsp[-3].p,"pattern"));
		    appendChild(yyval.node,yyvsp[-1].p);
               ;
    break;}
case 36:
#line 1074 "parser.y"
{
		 yyval.node = new_node("clear");
		 appendChild(yyval.node,yyvsp[-1].p);
               ;
    break;}
case 37:
#line 1085 "parser.y"
{
		   if ((yyvsp[-1].dtype.type != T_ERROR) && (yyvsp[-1].dtype.type != T_SYMBOL)) {
		     yyval.node = new_node("constant");
		     Setattr(yyval.node,"name",yyvsp[-3].id);
		     Setattr(yyval.node,"type",NewSwigType(yyvsp[-1].dtype.type));
		     Setattr(yyval.node,"value",yyvsp[-1].dtype.val);
		     Setattr(yyval.node,"storage","%constant");
		     Setattr(yyval.node,"feature:immutable","1");
		     add_symbols(yyval.node);
		   } else {
		     if (yyvsp[-1].dtype.type == T_ERROR) {
		       Swig_warning(WARN_PARSE_UNSUPPORTED_VALUE,cparse_file,cparse_line,"Unsupported constant value (ignored)\n");
		     }
		     yyval.node = 0;
		   }

	       ;
    break;}
case 38:
#line 1103 "parser.y"
{
		 if ((yyvsp[-1].dtype.type != T_ERROR) && (yyvsp[-1].dtype.type != T_SYMBOL)) {
		   SwigType_push(yyvsp[-3].type,yyvsp[-2].decl.type);
		   /* Sneaky callback function trick */
		   if (SwigType_isfunction(yyvsp[-3].type)) {
		     SwigType_add_pointer(yyvsp[-3].type);
		   }
		   yyval.node = new_node("constant");
		   Setattr(yyval.node,"name",yyvsp[-2].decl.id);
		   Setattr(yyval.node,"type",yyvsp[-3].type);
		   Setattr(yyval.node,"value",yyvsp[-1].dtype.val);
		   Setattr(yyval.node,"storage","%constant");
		   Setattr(yyval.node,"feature:immutable","1");
		   add_symbols(yyval.node);
		 } else {
		     if (yyvsp[-1].dtype.type == T_ERROR) {
		       Swig_warning(WARN_PARSE_UNSUPPORTED_VALUE,cparse_file,cparse_line,"Unsupported constant value\n");
		     }
		   yyval.node = 0;
		 }
               ;
    break;}
case 39:
#line 1124 "parser.y"
{
		 Swig_warning(WARN_PARSE_BAD_VALUE,cparse_file,cparse_line,"Bad constant value (ignored).\n");
		 yyval.node = 0;
	       ;
    break;}
case 40:
#line 1135 "parser.y"
{
		 char temp[64];
		 Replace(yyvsp[0].str,"$file",cparse_file, DOH_REPLACE_ANY);
		 sprintf(temp,"%d", cparse_line);
		 Replace(yyvsp[0].str,"$line",temp,DOH_REPLACE_ANY);
		 Printf(stderr,"%s\n", yyvsp[0].str);
		 Delete(yyvsp[0].str);
                 yyval.node = 0;
	       ;
    break;}
case 41:
#line 1144 "parser.y"
{
		 char temp[64];
		 String *s = NewString(yyvsp[0].id);
		 Replace(s,"$file",cparse_file, DOH_REPLACE_ANY);
		 sprintf(temp,"%d", cparse_line);
		 Replace(s,"$line",temp,DOH_REPLACE_ANY);
		 Printf(stderr,"%s\n", s);
		 Delete(s);
                 yyval.node = 0;
               ;
    break;}
case 42:
#line 1163 "parser.y"
{
                    skip_balanced('{','}');
		    yyval.node = 0;
		    Swig_warning(WARN_DEPRECATED_EXCEPT,cparse_file, cparse_line, "%%except is deprecated.  Use %%exception instead.");
	       ;
    break;}
case 43:
#line 1169 "parser.y"
{
                    skip_balanced('{','}');
		    yyval.node = 0;
		    Swig_warning(WARN_DEPRECATED_EXCEPT,cparse_file, cparse_line, "%%except is deprecated.  Use %%exception instead.");
               ;
    break;}
case 44:
#line 1175 "parser.y"
{
		 yyval.node = 0;
		 Swig_warning(WARN_DEPRECATED_EXCEPT,cparse_file, cparse_line, "%%except is deprecated.  Use %%exception instead.");
               ;
    break;}
case 45:
#line 1180 "parser.y"
{
		 yyval.node = 0;
		 Swig_warning(WARN_DEPRECATED_EXCEPT,cparse_file, cparse_line, "%%except is deprecated.  Use %%exception instead.");
	       ;
    break;}
case 46:
#line 1190 "parser.y"
{
                 yyval.node = new_node("fragment");
                 Setattr(yyval.node,"section", yyvsp[-2].id);
                 Setattr(yyval.node,"name",yyvsp[-4].id);
                 Setattr(yyval.node,"code",yyvsp[0].str);
                 ;
    break;}
case 47:
#line 1196 "parser.y"
{
                   skip_balanced('{','}');
		   yyval.node = new_node("fragment");
		   Setattr(yyval.node,"section",yyvsp[-2].id);
		   Setattr(yyval.node,"name",yyvsp[-4].id);
		   Delitem(scanner_ccode,0);
		   Delitem(scanner_ccode,DOH_END);
		   Setattr(yyval.node,"code",Copy(scanner_ccode));
		 ;
    break;}
case 48:
#line 1212 "parser.y"
{
                     yyvsp[-3].loc.filename = Swig_copy_string(cparse_file);
		     yyvsp[-3].loc.line = cparse_line;
		     cparse_file = Swig_copy_string(yyvsp[-1].id);
		     cparse_line = 0;
               ;
    break;}
case 49:
#line 1217 "parser.y"
{
                     yyval.node = yyvsp[-1].node;
		     cparse_file = yyvsp[-6].loc.filename;
		     cparse_line = yyvsp[-6].loc.line;
		     if (strcmp(yyvsp[-6].loc.type,"include") == 0) set_nodeType(yyval.node,"include");
		     if (strcmp(yyvsp[-6].loc.type,"import") == 0) set_nodeType(yyval.node,"import");
		     Setattr(yyval.node,"name",yyvsp[-4].id);
		     /* Search for the module (if any) */
		     {
			 Node *n = firstChild(yyval.node);
			 while (n) {
			     if (Strcmp(nodeType(n),"module") == 0) {
				 Setattr(yyval.node,"module",Getattr(n,"name"));
				 break;
			     }
			     n = nextSibling(n);
			 }
		     }
		     Setattr(yyval.node,"options",yyvsp[-5].node);
               ;
    break;}
case 50:
#line 1239 "parser.y"
{ yyval.loc.type = (char *) "include"; ;
    break;}
case 51:
#line 1240 "parser.y"
{ yyval.loc.type = (char *) "import"; ;
    break;}
case 52:
#line 1247 "parser.y"
{
                 String *cpps;
		 if (Namespaceprefix) {
		   Swig_error(cparse_file, cparse_start_line, "Error. %%inline directive inside a namespace is disallowed.\n");

		   yyval.node = 0;
		 } else {
		   yyval.node = new_node("insert");
		   Setattr(yyval.node,"code",yyvsp[0].str);
		   /* Need to run through the preprocessor */
		   Setline(yyvsp[0].str,cparse_start_line);
		   Setfile(yyvsp[0].str,cparse_file);
		   Seek(yyvsp[0].str,0,SEEK_SET);
		   cpps = Preprocessor_parse(yyvsp[0].str);
		   start_inline(Char(cpps), cparse_start_line);
		   Delete(yyvsp[0].str);
		   Delete(cpps);
		 }
	       ;
    break;}
case 53:
#line 1276 "parser.y"
{
                 yyval.node = new_node("insert");
		 Setattr(yyval.node,"code",yyvsp[0].str);
	       ;
    break;}
case 54:
#line 1280 "parser.y"
{
		 String *code = NewString("");
		 yyval.node = new_node("insert");
		 Setattr(yyval.node,"section",yyvsp[-2].id);
		 Setattr(yyval.node,"code",code);
		 if (Swig_insert_file(yyvsp[0].id,code) < 0) {
		   Swig_error(cparse_file, cparse_line, "Couldn't find '%s'.\n", yyvsp[0].id);
		   yyval.node = 0;
		 } 
               ;
    break;}
case 55:
#line 1290 "parser.y"
{
		 yyval.node = new_node("insert");
		 Setattr(yyval.node,"section",yyvsp[-2].id);
		 Setattr(yyval.node,"code",yyvsp[0].str);
               ;
    break;}
case 56:
#line 1295 "parser.y"
{
                 skip_balanced('{','}');
		 yyval.node = new_node("insert");
		 Setattr(yyval.node,"section",yyvsp[-2].id);
		 Delitem(scanner_ccode,0);
		 Delitem(scanner_ccode,DOH_END);
		 Setattr(yyval.node,"code", Copy(scanner_ccode));
	       ;
    break;}
case 57:
#line 1310 "parser.y"
{
                 yyval.node = new_node("module");
		 Setattr(yyval.node,"name",yyvsp[0].id);
		 if (yyvsp[-1].node) Setattr(yyval.node,"options",yyvsp[-1].node);
		 if (!ModuleName) ModuleName = NewString(yyvsp[0].id);
		 if (!module_node) module_node = yyval.node;
	       ;
    break;}
case 58:
#line 1324 "parser.y"
{
                 yyrename = NewString(yyvsp[-1].id);
		 yyval.node = 0;
               ;
    break;}
case 59:
#line 1328 "parser.y"
{
                   yyval.node = 0;
		   Swig_error(cparse_file,cparse_line,"Missing argument to %%name directive.\n");
	       ;
    break;}
case 60:
#line 1340 "parser.y"
{
                 yyval.node = new_node("native");
		 Setattr(yyval.node,"name",yyvsp[-4].id);
		 Setattr(yyval.node,"wrap:name",yyvsp[-1].id);
	         add_symbols(yyval.node);
	       ;
    break;}
case 61:
#line 1346 "parser.y"
{
		 if (!SwigType_isfunction(yyvsp[-1].decl.type)) {
		   Swig_error(cparse_file,cparse_line,"%%native declaration '%s' is not a function.\n", yyvsp[-1].decl.id);
		   yyval.node = 0;
		 } else {
		     Delete(SwigType_pop_function(yyvsp[-1].decl.type));
		     /* Need check for function here */
		     SwigType_push(yyvsp[-2].type,yyvsp[-1].decl.type);
		     yyval.node = new_node("native");
	             Setattr(yyval.node,"name",yyvsp[-5].id);
		     Setattr(yyval.node,"wrap:name",yyvsp[-1].decl.id);
		     Setattr(yyval.node,"type",yyvsp[-2].type);
		     Setattr(yyval.node,"parms",yyvsp[-1].decl.parms);
		     Setattr(yyval.node,"decl",yyvsp[-1].decl.type);
		 }
	         add_symbols(yyval.node);
	       ;
    break;}
case 62:
#line 1372 "parser.y"
{
                 yyval.node = new_node("pragma");
		 Setattr(yyval.node,"lang",yyvsp[-3].id);
		 Setattr(yyval.node,"name",yyvsp[-2].id);
		 Setattr(yyval.node,"value",yyvsp[0].str);
	       ;
    break;}
case 63:
#line 1378 "parser.y"
{
		yyval.node = new_node("pragma");
		Setattr(yyval.node,"lang",yyvsp[-1].id);
		Setattr(yyval.node,"name",yyvsp[0].id);
	      ;
    break;}
case 64:
#line 1385 "parser.y"
{ yyval.str = NewString(yyvsp[0].id); ;
    break;}
case 65:
#line 1386 "parser.y"
{ yyval.str = yyvsp[0].str; ;
    break;}
case 66:
#line 1389 "parser.y"
{ yyval.id = yyvsp[-1].id; ;
    break;}
case 67:
#line 1390 "parser.y"
{ yyval.id = (char *) "swig"; ;
    break;}
case 68:
#line 1398 "parser.y"
{
                    SwigType *t = yyvsp[-2].decl.type;
		    if (!Len(t)) t = 0;
		    if (yyvsp[-3].ivalue) {
		      rename_add(yyvsp[-2].decl.id,t,yyvsp[-1].id);
		    } else {
		      namewarn_add(yyvsp[-2].decl.id,t,yyvsp[-1].id);
		    }
		    yyval.node = 0;
		    scanner_clear_rename();
              ;
    break;}
case 69:
#line 1409 "parser.y"
{
		String *fixname;
		SwigType *t = yyvsp[-2].decl.type;
		fixname = feature_identifier_fix(yyvsp[-2].decl.id);
		if (!Len(t)) t = 0;
		/* Special declarator check */
		if (t) {
		  if (yyvsp[-1].dtype.qualifier) SwigType_push(t,yyvsp[-1].dtype.qualifier);
		  if (SwigType_isfunction(t)) {
		    SwigType *decl = SwigType_pop_function(t);
		    if (SwigType_ispointer(t)) {
		      String *nname = NewStringf("*%s",fixname);
		      if (yyvsp[-6].ivalue) {
			rename_add(Char(nname),decl,yyvsp[-4].id);
		      } else {
			namewarn_add(Char(nname),decl,yyvsp[-4].id);
		      }
		      Delete(nname);
		    } else {
		      if (yyvsp[-6].ivalue) {
			rename_add(Char(fixname),decl,yyvsp[-4].id);
		      } else {
			namewarn_add(Char(fixname),decl,yyvsp[-4].id);
		      }
		    }
		  } else if (SwigType_ispointer(t)) {
		    String *nname = NewStringf("*%s",fixname);
		    if (yyvsp[-6].ivalue) {
		      rename_add(Char(nname),0,yyvsp[-4].id);
		    } else {
		      namewarn_add(Char(nname),0,yyvsp[-4].id);
		    }
		    Delete(nname);
		  }
		} else {
		  if (yyvsp[-6].ivalue) {
		    rename_add(Char(fixname),0,yyvsp[-4].id);
		  } else {
		    namewarn_add(Char(fixname),0,yyvsp[-4].id);
		  }
		}
                yyval.node = 0;
		scanner_clear_rename();
              ;
    break;}
case 70:
#line 1453 "parser.y"
{
		if (yyvsp[-5].ivalue) {
		  rename_add(yyvsp[-1].id,0,yyvsp[-3].id);
		} else {
		  namewarn_add(yyvsp[-1].id,0,yyvsp[-3].id);
		}
		yyval.node = 0;
		scanner_clear_rename();
              ;
    break;}
case 71:
#line 1464 "parser.y"
{
		    yyval.ivalue = 1;
                ;
    break;}
case 72:
#line 1467 "parser.y"
{
                    yyval.ivalue = 0;
                ;
    break;}
case 73:
#line 1480 "parser.y"
{
                 String *fname;
                 String *val;
		 String *name;
		 String *fixname;
		 SwigType *t;
                 if (!features_hash) features_hash = NewHash();
		 fname = NewStringf("feature:%s",yyvsp[-4].id);
		 fixname = feature_identifier_fix(yyvsp[-2].decl.id);
		 if (Namespaceprefix) {
		   name = NewStringf("%s::%s",Namespaceprefix, fixname);
		 } else {
		   name = fixname;
		 }
		 val = yyvsp[0].str ? NewString(yyvsp[0].str) : NewString("1");
		 if (yyvsp[-2].decl.parms) {
		   Setmeta(val,"parms",yyvsp[-2].decl.parms);
		 }
		 t = yyvsp[-2].decl.type;
		 if (yyvsp[-2].decl.parms) Setmeta(val,"parms",yyvsp[-2].decl.parms);
		 if (!Len(t)) t = 0;
		 if (t) {
		   if (yyvsp[-1].dtype.qualifier) SwigType_push(t,yyvsp[-1].dtype.qualifier);
		   if (SwigType_isfunction(t)) {
		     SwigType *decl = SwigType_pop_function(t);
		     if (SwigType_ispointer(t)) {
		       String *nname = NewStringf("*%s",name);
		       Swig_feature_set(features_hash, nname, decl, fname, val);
		       Delete(nname);
		     } else {
		       Swig_feature_set(features_hash, name, decl, fname, val);
		     }
		   } else if (SwigType_ispointer(t)) {
		     String *nname = NewStringf("*%s",name);
		     Swig_feature_set(features_hash,nname,0,fname,val);
		     Delete(nname);
		   }
		 } else {
		   Swig_feature_set(features_hash,name,0,fname,val);
		 }
		 Delete(fname);
		 Delete(name);
		 yyval.node = 0;
              ;
    break;}
case 74:
#line 1527 "parser.y"
{
                 String *fname;
                 String *val;
		 String *name;
		 String *fixname;
		 SwigType *t;

                 if (!features_hash) features_hash = NewHash();
		 fname = NewStringf("feature:%s",yyvsp[-6].id);
		 fixname = feature_identifier_fix(yyvsp[-2].decl.id);
		 if (Namespaceprefix) {
		   name = NewStringf("%s::%s",Namespaceprefix, fixname);
		 } else {
		   name = fixname;
		 }
		 if (Len(yyvsp[-4].id)) {
		   val = NewString(yyvsp[-4].id);
		 } else {
		   val = 0;
		 }
		 if (yyvsp[-2].decl.parms) {
		   Setmeta(val,"parms",yyvsp[-2].decl.parms);
		 }
		 t = yyvsp[-2].decl.type;
		 if (yyvsp[-2].decl.parms) Setmeta(val,"parms",yyvsp[-2].decl.parms);
		 if (!Len(t)) t = 0;
		 if (t) {
		   if (yyvsp[-1].dtype.qualifier) SwigType_push(t,yyvsp[-1].dtype.qualifier);
		   if (SwigType_isfunction(t)) {
		     SwigType *decl = SwigType_pop_function(t);
		     if (SwigType_ispointer(t)) {
		       String *nname = NewStringf("*%s",name);
		       Swig_feature_set(features_hash, nname, decl, fname, val);
		       Delete(nname);
		     } else {
		       Swig_feature_set(features_hash, name, decl, fname, val);
		     }
		   } else if (SwigType_ispointer(t)) {
		     String *nname = NewStringf("*%s",name);
		     Swig_feature_set(features_hash,nname,0,fname,val);
		     Delete(nname);
		   }
		 } else {
		   Swig_feature_set(features_hash,name,0,fname,val);
		 }
		 Delete(fname);
		 Delete(name);
		 yyval.node = 0;
              ;
    break;}
case 75:
#line 1579 "parser.y"
{
		String *name;
		String *fname = NewStringf("feature:%s",yyvsp[-2].id);
		if (!features_hash) features_hash = NewHash();
		if (Namespaceprefix) name = NewStringf("%s::", Namespaceprefix);
		else name = NewString("");
		Swig_feature_set(features_hash,name,0,fname,(yyvsp[0].str ? NewString(yyvsp[0].str) : NewString("1")));
		Delete(name);
		Delete(fname);
		yyval.node = 0;
              ;
    break;}
case 76:
#line 1590 "parser.y"
{
		String *name;
		String *fname = NewStringf("feature:%s",yyvsp[-4].id);
		if (!features_hash) features_hash = NewHash();
		if (Namespaceprefix) name = NewStringf("%s::", Namespaceprefix);
		else name = NewString("");
		Swig_feature_set(features_hash,name,0,fname,(Len(yyvsp[-2].id) ? NewString(yyvsp[-2].id) : 0));
		Delete(name);
		Delete(fname);
		yyval.node = 0;
              ;
    break;}
case 77:
#line 1603 "parser.y"
{ yyval.str = yyvsp[0].str; ;
    break;}
case 78:
#line 1604 "parser.y"
{ yyval.str = 0; ;
    break;}
case 79:
#line 1605 "parser.y"
{ yyval.str = yyvsp[-2].pl; ;
    break;}
case 80:
#line 1610 "parser.y"
{
                 Parm *val;
		 String *name;
		 SwigType *t;
                 if (!features_hash) features_hash = NewHash();
		 if (Namespaceprefix) name = NewStringf("%s::%s", Namespaceprefix, yyvsp[-2].decl.id);
		 else name = NewString(yyvsp[-2].decl.id);
		 val = yyvsp[-4].pl;
		 if (yyvsp[-2].decl.parms) {
		   Setmeta(val,"parms",yyvsp[-2].decl.parms);
		 }
		 t = yyvsp[-2].decl.type;
		 if (!Len(t)) t = 0;
		 if (t) {
		   if (yyvsp[-1].dtype.qualifier) SwigType_push(t,yyvsp[-1].dtype.qualifier);
		   if (SwigType_isfunction(t)) {
		     SwigType *decl = SwigType_pop_function(t);
		     if (SwigType_ispointer(t)) {
		       String *nname = NewStringf("*%s",name);
		       Swig_feature_set(features_hash, nname, decl, "feature:varargs", val);
		       Delete(nname);
		     } else {
		       Swig_feature_set(features_hash, name, decl, "feature:varargs", val);
		     }
		   } else if (SwigType_ispointer(t)) {
		     String *nname = NewStringf("*%s",name);
		     Swig_feature_set(features_hash,nname,0,"feature:varargs",val);
		     Delete(nname);
		   }
		 } else {
		   Swig_feature_set(features_hash,name,0,"feature:varargs",val);
		 }
		 Delete(name);
		 yyval.node = 0;
              ;
    break;}
case 81:
#line 1646 "parser.y"
{ yyval.pl = yyvsp[0].pl; ;
    break;}
case 82:
#line 1647 "parser.y"
{ 
		  int i;
		  int n;
		  Parm *p;
		  n = atoi(Char(yyvsp[-2].dtype.val));
		  if (n <= 0) {
		    Swig_error(cparse_file, cparse_line,"Argument count in %%varargs must be positive.\n");
		    yyval.pl = 0;
		  } else {
		    yyval.pl = Copy(yyvsp[0].p);
		    Setattr(yyval.pl,"name","VARARGS_SENTINEL");
		    for (i = 0; i < n; i++) {
		      p = Copy(yyvsp[0].p);
		      set_nextSibling(p,yyval.pl);
		      yyval.pl = p;
		    }
		  }
                ;
    break;}
case 83:
#line 1676 "parser.y"
{
		   yyval.node = 0;
		   if (yyvsp[-3].tmap.op) {
		     yyval.node = new_node("typemap");
		     Setattr(yyval.node,"method",yyvsp[-3].tmap.op);
		     Setattr(yyval.node,"code",NewString(yyvsp[0].str));
		     if (yyvsp[-3].tmap.kwargs) {
		       Setattr(yyval.node,"kwargs", yyvsp[-3].tmap.kwargs);
		     }
		     appendChild(yyval.node,yyvsp[-1].p);
		   }
	       ;
    break;}
case 84:
#line 1688 "parser.y"
{
		 yyval.node = 0;
		 if (yyvsp[-3].tmap.op) {
		   yyval.node = new_node("typemap");
		   Setattr(yyval.node,"method",yyvsp[-3].tmap.op);
		   appendChild(yyval.node,yyvsp[-1].p);
		 }
	       ;
    break;}
case 85:
#line 1696 "parser.y"
{
		   yyval.node = 0;
		   if (yyvsp[-5].tmap.op) {
		     yyval.node = new_node("typemapcopy");
		     Setattr(yyval.node,"method",yyvsp[-5].tmap.op);
		     Setattr(yyval.node,"pattern", Getattr(yyvsp[-1].p,"pattern"));
		     appendChild(yyval.node,yyvsp[-3].p);
		   }
	       ;
    break;}
case 86:
#line 1709 "parser.y"
{
		 Hash *p;
		 String *name;
		 p = nextSibling(yyvsp[0].node);
		 if (p && (!Getattr(p,"value"))) {
		   /* two argument typemap form */
		   name = Getattr(yyvsp[0].node,"name");
		   if (!name || (Strcmp(name,typemap_lang))) {
		     yyval.tmap.op = 0;
		     yyval.tmap.kwargs = 0;
		   } else {
		     yyval.tmap.op = Getattr(p,"name");
		     yyval.tmap.kwargs = nextSibling(p);
		   }
		 } else {
		   /* one-argument typemap-form */
		   yyval.tmap.op = Getattr(yyvsp[0].node,"name");
		   yyval.tmap.kwargs = p;
		 }
                ;
    break;}
case 87:
#line 1731 "parser.y"
{
                 yyval.p = yyvsp[-1].p;
		 set_nextSibling(yyval.p,yyvsp[0].p);
		;
    break;}
case 88:
#line 1737 "parser.y"
{
                 yyval.p = yyvsp[-1].p;
		 set_nextSibling(yyval.p,yyvsp[0].p);
                ;
    break;}
case 89:
#line 1741 "parser.y"
{ yyval.p = 0;;
    break;}
case 90:
#line 1744 "parser.y"
{
		  SwigType_push(yyvsp[-1].type,yyvsp[0].decl.type);
		  yyval.p = new_node("typemapitem");
		  Setattr(yyval.p,"pattern",NewParm(yyvsp[-1].type,yyvsp[0].decl.id));
		  Setattr(yyval.p,"parms", yyvsp[0].decl.parms);
		  /*		  $$ = NewParm($1,$2.id);
				  Setattr($$,"parms",$2.parms); */
                ;
    break;}
case 91:
#line 1752 "parser.y"
{
                  yyval.p = new_node("typemapitem");
		  Setattr(yyval.p,"pattern",yyvsp[-1].pl);
		  /*		  Setattr($$,"multitype",$2); */
               ;
    break;}
case 92:
#line 1757 "parser.y"
{
		 yyval.p = new_node("typemapitem");
		 Setattr(yyval.p,"pattern", yyvsp[-4].pl);
		 /*                 Setattr($$,"multitype",$2); */
		 Setattr(yyval.p,"parms",yyvsp[-1].pl);
               ;
    break;}
case 93:
#line 1769 "parser.y"
{
                   yyval.node = new_node("types");
		   Setattr(yyval.node,"parms",yyvsp[-2].pl);
               ;
    break;}
case 94:
#line 1779 "parser.y"
{
                  Parm *p, *tp;
		  Node *n;
		  Node *nspace = 0, *nspace_inner = 0;
		  Node *tnode = 0;
		  Symtab *tscope = 0;
		  int     specialized = 0;
		  yyval.node = 0;

		  tscope = Swig_symbol_current();          /* Get the current scope */

		  /* If the template name is qualified.  We need to create or lookup namespace entries */
		  if (Swig_scopename_check(yyvsp[-4].str)) {
		    String *prefix, *base;
		    Node   *ns;
		    prefix = Swig_scopename_prefix(yyvsp[-4].str);
		    base = Swig_scopename_last(yyvsp[-4].str);
		    
		    /* Try to locate the scope */
		    ns = Swig_symbol_clookup(prefix,0);
		    if (!ns) {
		      Swig_error(cparse_file,cparse_line,"Undefined scope '%s'\n", prefix);
		    } else {
		      if (Strcmp(nodeType(ns),"namespace") != 0) {
			Swig_error(cparse_file,cparse_line,"'%s' is not defined as namespace.\n", prefix);
			ns = 0;
		      } else {
			/*			Swig_symbol_setscope(Getattr(ns,"symtab"));
						Namespaceprefix = Swig_symbol_qualifiedscopename(0); */
		      }
		    }

		    /* Create namespace nodes to enclose the template declaration */
		    if (ns) {
		      List *scopes;
		      String *sname;
		      String *name = NewString(prefix);
		      scopes = NewList();
		      while (name) {
			String *tprefix;
			String *base = Swig_scopename_last(name);
			Insert(scopes,0,base);
			tprefix = Swig_scopename_prefix(name);
			Delete(name);
			name = tprefix;
		      }
		      for (sname = Firstitem(scopes); sname; sname = Nextitem(scopes)) {
			Node *ns1,*ns2;

			ns1 = Swig_symbol_clookup(sname,0);
			assert(ns1);
			if (Strcmp(nodeType(ns1),"namespace") == 0) {
			  if (Getattr(ns1,"alias")) {
			    ns1 = Getattr(ns1,"namespace");
			  }
			} else {
			  assert(0);
			}
			ns2 = new_node("namespace");
			Setattr(ns2,"name",sname);
			Setattr(ns2,"symtab", Getattr(ns1,"symtab"));
			add_symbols(ns2);
			Swig_symbol_setscope(Getattr(ns1,"symtab"));
			Namespaceprefix = Swig_symbol_qualifiedscopename(0);
			if (nspace_inner) {
			  appendChild(nspace_inner,ns2);
			}
			nspace_inner = ns2;
			if (!nspace) nspace = ns2;
		      }
		      yyvsp[-4].str = base;
		    }
		  }

		  n = Swig_cparse_template_locate(yyvsp[-4].str,yyvsp[-2].p);

		  /* Patch the argument types to respect namespaces */
		  p = yyvsp[-2].p;
		  while (p) {
		    if (!Getattr(p,"value")) {
		      SwigType *ty = Getattr(p,"type");
		      if (ty) {
			ty = Swig_symbol_type_qualify(ty,0);
			/*			ty = Swig_symbol_typedef_reduce(ty,0); */
			Setattr(p,"type",ty);
		      }
		    }
		    p = nextSibling(p);
		  }
		  /* Look for the template */

		  if (n && (Strcmp(nodeType(n),"template") == 0)) {
		    Parm *tparms = Getattr(n,"templateparms");
		    if (!tparms) specialized = 1;
		    if (!specialized && ((ParmList_len(yyvsp[-2].p) > ParmList_len(tparms)))) {
		      Swig_error(cparse_file, cparse_line, "Too many template parameters. Maximum of %d.\n", ParmList_len(tparms));
		    } else if (!specialized && ((ParmList_len(yyvsp[-2].p) < ParmList_numrequired(tparms)))) {
		      Swig_error(cparse_file, cparse_line, "Not enough template parameters specified. %d required.\n", ParmList_numrequired(tparms));
		    } else {
		      int  def_supplied = 0;
		      /* Expand the template */
		      ParmList *temparms;
		      if (specialized) temparms = CopyParmList(yyvsp[-2].p);
		      else temparms = CopyParmList(tparms);
		      /* Create typedef's and arguments */
		      p = yyvsp[-2].p;
		      tp = temparms;
		      while (p) {
			String *value = Getattr(p,"value");
			if (def_supplied) {
			  Setattr(p,"default","1");
			}
			if (value) {
			  Setattr(tp,"value",value);
			} else {
			  SwigType *ty = Getattr(p,"type");
			  if (ty) {
			    Setattr(tp,"type",ty);
			  }
			  Delattr(tp,"value");
			}
			p = nextSibling(p);
			tp = nextSibling(tp);
			if (!p && tp) {
			  p = tp;
			  def_supplied = 1;
			}
		      }

		      yyval.node = copy_node(n);
		      /* We need to set the node name based on name used to instantiate */
		      Setattr(yyval.node,"name",Copy(yyvsp[-4].str));
		      if (!specialized) {
			Delattr(yyval.node,"sym:typename");
		      } else {
			Setattr(yyval.node,"sym:typename","1");
		      }
		      if (yyvsp[-6].id) {
			Swig_cparse_template_expand(yyval.node,yyvsp[-6].id,temparms);
			Setattr(yyval.node,"sym:name",yyvsp[-6].id);
		      } else {
			static int cnt = 0;
			String *nname = NewStringf("__dummy_%d__", cnt++);
			Swig_cparse_template_expand(yyval.node,nname,temparms);
			Setattr(yyval.node,"sym:name",nname);
			Setattr(yyval.node,"feature:ignore","1");
		      }
		      Delattr(yyval.node,"templatetype");
		      Setattr(yyval.node,"template",n);
		      tnode = yyval.node;
		      Setfile(yyval.node,cparse_file);
		      Setline(yyval.node,cparse_line);
		      Delete(temparms);
		      
		      add_symbols_copy(yyval.node);
		      if (Strcmp(nodeType(yyval.node),"class") == 0) {

			/* Identify pure abstract methods */
			Setattr(yyval.node,"abstract", pure_abstract(firstChild(yyval.node)));
			
                        /* Set up inheritance in symbol table */
			{
			  Symtab  *csyms;
			  List *baselist = Getattr(yyval.node,"baselist");
			  csyms = Swig_symbol_current();
			  Swig_symbol_setscope(Getattr(yyval.node,"symtab"));
			  if (baselist) {
			    List *bases = make_inherit_list(Getattr(yyval.node,"name"),baselist);
			    if (bases) {
			      Node *s;
			      for (s = Firstitem(bases); s; s = Nextitem(bases)) {
				Symtab *st = Getattr(s,"symtab");
				if (st) {
				  Swig_symbol_inherit(st);
				}
			      }
			    }
			  }
			  Swig_symbol_setscope(csyms);
			}

			/* Merge in addmethods for this class */
			
			/* !!! This may be broken.  We may have to
			   add the addmethods at the beginning of
			   the class */
			
			if (extendhash) {
			  String *clsname;
			  Node *am;
			  if (Namespaceprefix) {
			    clsname = NewStringf("%s::%s", Namespaceprefix, Getattr(yyval.node,"name"));
			  } else {
			    clsname = Getattr(yyval.node,"name");
			  }
			  am = Getattr(extendhash,clsname);
			  if (am) {
			    merge_extensions(am);
			    appendChild(yyval.node,am);
			    Delattr(extendhash,clsname);
			  }
			}
			/* Add to classes hash */
			if (!classes) classes = NewHash();
			Setattr(classes,Swig_symbol_qualifiedscopename(yyval.node),yyval.node);
		      }
		    }
		    if (yyval.node && nspace) {
		      appendChild(nspace_inner,yyval.node);
		      yyval.node = nspace;
		    }
		  }
   	          Swig_symbol_setscope(tscope);
		  Namespaceprefix = Swig_symbol_qualifiedscopename(0);

                ;
    break;}
case 95:
#line 2002 "parser.y"
{
		  Swig_warning(0,cparse_file, cparse_line,"%s\n", yyvsp[0].id);
		  yyval.node = 0;
               ;
    break;}
case 96:
#line 2012 "parser.y"
{
                    yyval.node = yyvsp[0].node; 
                    if (yyval.node) {
   		      add_symbols(yyval.node);
   	            }
                ;
    break;}
case 97:
#line 2018 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 98:
#line 2022 "parser.y"
{ 
		  if (Strcmp(yyvsp[-3].id,"C") == 0) {
		    yyval.node = new_node("extern");
		    Setattr(yyval.node,"name",yyvsp[-3].id);
		    appendChild(yyval.node,firstChild(yyvsp[-1].node));
		  } else {
		     Swig_warning(WARN_PARSE_UNDEFINED_EXTERN,cparse_file, cparse_line,"Unrecognized extern type \"%s\" (ignored).\n", yyvsp[-3].id);
		     yyval.node = 0;
		  }
                ;
    break;}
case 99:
#line 2038 "parser.y"
{
              yyval.node = new_node("cdecl");
	      if (yyvsp[-1].dtype.qualifier) SwigType_push(yyvsp[-2].decl.type,yyvsp[-1].dtype.qualifier);
	      Setattr(yyval.node,"type",yyvsp[-3].type);
	      Setattr(yyval.node,"storage",yyvsp[-4].id);
	      Setattr(yyval.node,"name",yyvsp[-2].decl.id);
	      Setattr(yyval.node,"decl",yyvsp[-2].decl.type);
	      Setattr(yyval.node,"parms",yyvsp[-2].decl.parms);
	      Setattr(yyval.node,"value",yyvsp[-1].dtype.val);
	      Setattr(yyval.node,"throws",yyvsp[-1].dtype.throws);
	      if (!yyvsp[0].node) {
		if (Len(scanner_ccode)) {
		  Setattr(yyval.node,"code",Copy(scanner_ccode));
		}
	      } else {
		Node *n = yyvsp[0].node;
		/* Inherit attributes */
		while (n) {
		  Setattr(n,"type",Copy(yyvsp[-3].type));
		  Setattr(n,"storage",yyvsp[-4].id);
		  n = nextSibling(n);
		}
	      }
	      if (yyvsp[-1].dtype.bitfield) {
		Setattr(yyval.node,"bitfield", yyvsp[-1].dtype.bitfield);
	      }

	      /* Look for "::" declarations (ignored) */
	      if (Strstr(yyvsp[-2].decl.id,"::")) {
		Delete(yyval.node);
		yyval.node = yyvsp[0].node;
	      } else {
		set_nextSibling(yyval.node,yyvsp[0].node);
	      }
           ;
    break;}
case 100:
#line 2077 "parser.y"
{ 
                   yyval.node = 0;
                   Clear(scanner_ccode); 
               ;
    break;}
case 101:
#line 2081 "parser.y"
{
		 yyval.node = new_node("cdecl");
		 if (yyvsp[-1].dtype.qualifier) SwigType_push(yyvsp[-2].decl.type,yyvsp[-1].dtype.qualifier);
		 Setattr(yyval.node,"name",yyvsp[-2].decl.id);
		 Setattr(yyval.node,"decl",yyvsp[-2].decl.type);
		 Setattr(yyval.node,"parms",yyvsp[-2].decl.parms);
		 Setattr(yyval.node,"value",yyvsp[-1].dtype.val);
		 Setattr(yyval.node,"throws",yyvsp[-1].dtype.throws);
		 if (yyvsp[-1].dtype.bitfield) {
		   Setattr(yyval.node,"bitfield", yyvsp[-1].dtype.bitfield);
		 }
		 if (!yyvsp[0].node) {
		   if (Len(scanner_ccode)) {
		     Setattr(yyval.node,"code",Copy(scanner_ccode));
		   }
		 } else {
		   set_nextSibling(yyval.node,yyvsp[0].node);
		 }
	       ;
    break;}
case 102:
#line 2100 "parser.y"
{ 
                   skip_balanced('{','}');
                   yyval.node = 0;
               ;
    break;}
case 103:
#line 2106 "parser.y"
{ 
                   yyval.dtype = yyvsp[0].dtype; 
                   yyval.dtype.qualifier = 0;
		   yyval.dtype.throws = 0;
              ;
    break;}
case 104:
#line 2111 "parser.y"
{ 
                   yyval.dtype = yyvsp[0].dtype; 
		   yyval.dtype.qualifier = yyvsp[-1].str;
		   yyval.dtype.throws = 0;
	      ;
    break;}
case 105:
#line 2116 "parser.y"
{ 
		   yyval.dtype = yyvsp[0].dtype; 
                   yyval.dtype.qualifier = 0;
		   yyval.dtype.throws = yyvsp[-2].pl;
              ;
    break;}
case 106:
#line 2121 "parser.y"
{ 
                   yyval.dtype = yyvsp[0].dtype; 
                   yyval.dtype.qualifier = yyvsp[-5].str;
		   yyval.dtype.throws = yyvsp[-2].pl;
              ;
    break;}
case 107:
#line 2133 "parser.y"
{
                  yyval.node = new_node("enum");
		  Setattr(yyval.node,"name",yyvsp[-4].id);
		  appendChild(yyval.node,yyvsp[-2].node);
		  add_symbols(yyval.node);           /* Add to tag space */
		  add_symbols(yyvsp[-2].node);       /* Add enum values to id space */
	       ;
    break;}
case 108:
#line 2141 "parser.y"
{
		 Node *n;
		 SwigType *ty = 0;
		 String   *unnamed = 0;

		 yyval.node = new_node("enum");
		 if (yyvsp[-5].id) {
		   Setattr(yyval.node,"name",yyvsp[-5].id);
		   ty = NewStringf("enum %s", yyvsp[-5].id);
		 } else if (yyvsp[-1].decl.id){
		   unnamed = make_unnamed();
		   ty = NewStringf("enum %s", unnamed);
		   Setattr(yyval.node,"unnamed",unnamed);
		   /* WF 20/12/2001: Cannot get sym:name and symtab set without setting name - fix!
		      // I don't think sym:name should be set. */
		   Setattr(yyval.node,"name",yyvsp[-1].decl.id);
		   Setattr(yyval.node,"tdname",yyvsp[-1].decl.id);
		   Setattr(yyval.node,"storage",yyvsp[-7].id);
		 }
		 appendChild(yyval.node,yyvsp[-3].node);
		 n = new_node("cdecl");
		 Setattr(n,"type",ty);
		 Setattr(n,"name",yyvsp[-1].decl.id);
		 Setattr(n,"storage",yyvsp[-7].id);
		 Setattr(n,"decl",yyvsp[-1].decl.type);
		 Setattr(n,"parms",yyvsp[-1].decl.parms);
		 Setattr(n,"unnamed",unnamed);
		 if (yyvsp[0].node) {
		   Node *p = yyvsp[0].node;
		   set_nextSibling(n,p);
		   while (p) {
		     Setattr(p,"type",Copy(ty));
		     Setattr(p,"unnamed",unnamed);
		     Setattr(p,"storage",yyvsp[-7].id);
		     p = nextSibling(p);
		   }
		 } else {
		   if (Len(scanner_ccode)) {
		     Setattr(n,"code",Copy(scanner_ccode));
		   }
		 }
		 add_symbols(yyval.node);        /* Add enum to tag space */
		 set_nextSibling(yyval.node,n);
		 add_symbols(yyvsp[-3].node);       /* Add to id space */
	         add_symbols(n);
	       ;
    break;}
case 109:
#line 2189 "parser.y"
{
                   /* This is a sick hack.  If the ctor_end has parameters,
                      and the parms paremeter only has 1 parameter, this
                      could be a declaration of the form:

                         type (id)(parms)

			 Otherwise it's an error. */
                    int err = 0;
                    yyval.node = 0;

		    if ((ParmList_len(yyvsp[-2].pl) == 1) && (!Swig_scopename_check(yyvsp[-4].type))) {
		      SwigType *ty = Getattr(yyvsp[-2].pl,"type");
		      String *name = Getattr(yyvsp[-2].pl,"name");
		      err = 1;
		      if (!name) {
			yyval.node = new_node("cdecl");
			Setattr(yyval.node,"type",yyvsp[-4].type);
			Setattr(yyval.node,"storage",yyvsp[-5].id);
			Setattr(yyval.node,"name",ty);

			if (yyvsp[0].decl.have_parms) {
			  SwigType *decl = NewString("");
			  SwigType_add_function(decl,yyvsp[0].decl.parms);
			  Setattr(yyval.node,"decl",decl);
			  Setattr(yyval.node,"parms",yyvsp[0].decl.parms);
			  if (Len(scanner_ccode)) {
			    Setattr(yyval.node,"code",Copy(scanner_ccode));
			  }
			}
			if (yyvsp[0].decl.defarg) {
			  Setattr(yyval.node,"value",yyvsp[0].decl.defarg);
			}
			Setattr(yyval.node,"throws",yyvsp[0].decl.throws);
			err = 0;
		      }
		    }
		    if (err) {
		      Swig_error(cparse_file,cparse_line,"Syntax error in input.\n");
		    }
                ;
    break;}
case 110:
#line 2236 "parser.y"
{  yyval.node = yyvsp[0].node; ;
    break;}
case 111:
#line 2237 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 112:
#line 2238 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 113:
#line 2239 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 114:
#line 2240 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 115:
#line 2241 "parser.y"
{ yyval.node = 0; ;
    break;}
case 116:
#line 2247 "parser.y"
{
                   List *bases = 0;
                   class_rename = make_name(yyvsp[-2].str,0);
		   Classprefix = NewString(yyvsp[-2].str);
		   /* Deal with inheritance  */
		   if (yyvsp[-1].bases) {
		     bases = make_inherit_list(yyvsp[-2].str,yyvsp[-1].bases);
		   }
		   if (SwigType_istemplate(yyvsp[-2].str)) {
		     String *fbase, *tbase, *prefix;
		     prefix = SwigType_templateprefix(yyvsp[-2].str);
		     if (Namespaceprefix) {
		       fbase = NewStringf("%s::%s", Namespaceprefix,yyvsp[-2].str);
		       tbase = NewStringf("%s::%s", Namespaceprefix, prefix);
		     } else {
		       fbase = Copy(yyvsp[-2].str);
		       tbase = Copy(prefix);
		     }
		     rename_inherit(tbase,fbase);
		     Delete(fbase);
		     Delete(tbase);
		     Delete(prefix);
		   }
                   if (strcmp(yyvsp[-3].id,"class") == 0) {
		     cplus_mode = CPLUS_PRIVATE;
		   } else {
		     cplus_mode = CPLUS_PUBLIC;
		   }
		   Swig_symbol_newscope();
		   Swig_symbol_setscopename(yyvsp[-2].str);
		   if (bases) {
		     Node *s;
		     for (s = Firstitem(bases); s; s = Nextitem(bases)) {
		       Symtab *st = Getattr(s,"symtab");
		       if (st) {
			 Swig_symbol_inherit(st); 
		       }
		     }
		   }
		   Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		   cparse_start_line = cparse_line;

		   /* If there are active template parameters, we need to make sure they are
                      placed in the class symbol table so we can catch shadows */

		   if (template_parameters) {
		     Parm *tp = template_parameters;
		     while(tp) {
		       Node *tn = new_node("templateparm");
		       Setattr(tn,"name",Getattr(tp,"name"));
		       Swig_symbol_cadd(Copy(Getattr(tp,"name")),tn);
		       tp = nextSibling(tp);
		     }
		   }
		   inclass = 1;
               ;
    break;}
case 117:
#line 2302 "parser.y"
{
		 Node *p;
		 SwigType *ty;
		 inclass = 0;
		 yyval.node = new_node("class");
		 Setline(yyval.node,cparse_start_line);
		 Setattr(yyval.node,"name",yyvsp[-6].str);
		 Setattr(yyval.node,"kind",yyvsp[-7].id);
		 Setattr(yyval.node,"baselist",yyvsp[-5].bases);
		 Setattr(yyval.node,"allows_typedef","1");
		 /* Check for pure-abstract class */
		 Setattr(yyval.node,"abstract", pure_abstract(yyvsp[-2].node));
		 
		 /* This bit of code merges in a previously defined %extend directive (if any) */
		 if (extendhash) {
		   String *clsname = Swig_symbol_qualifiedscopename(0);
		   Node *am = Getattr(extendhash,clsname);
		   if (am) {
		     merge_extensions(am);
		     appendChild(yyval.node,am);
		     Delattr(extendhash,clsname);
		   }
		   Delete(clsname);
		 }
		 if (!classes) classes = NewHash();
		 Setattr(classes,Swig_symbol_qualifiedscopename(0),yyval.node);
		 

		 appendChild(yyval.node,yyvsp[-2].node);
		 p = yyvsp[0].node;
		 if (p) {
		   set_nextSibling(yyval.node,p);
		 }
		 
		 if (cparse_cplusplus) {
		   ty = NewString(yyvsp[-6].str);
		 } else {
		   ty = NewStringf("%s %s", yyvsp[-7].id,yyvsp[-6].str);
		 }
		 while (p) {
		   Setattr(p,"storage",yyvsp[-8].id);
		   Setattr(p,"type",ty);
		   p = nextSibling(p);
		 }
		 /* Dump nested classes */
		 {
		   String *name = yyvsp[-6].str;
		   if (yyvsp[0].node) {
		     SwigType *decltype = Getattr(yyvsp[0].node,"decl");
		     if (Cmp(yyvsp[-8].id,"typedef") == 0) {
		       if (!decltype || !Len(decltype)) {
			 name = Getattr(yyvsp[0].node,"name");
			 Setattr(yyval.node,"tdname",Copy(name));

			 /* Use typedef name as class name */
			 if (class_rename && (Strcmp(class_rename,yyvsp[-6].str) == 0)) {
			   class_rename = NewString(name);
			 }
			 if (!Getattr(classes,name)) {
			   Setattr(classes,name,yyval.node);
			 }
			 Setattr(yyval.node,"decl",decltype);
		       }
		     }
		   }
		   appendChild(yyval.node,dump_nested(Char(name)));
		 }
		 Setattr(yyval.node,"symtab",Swig_symbol_popscope());

		 yyrename = NewString(class_rename);
		 Classprefix = 0;
		 Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		 add_symbols(yyval.node);
		 if (yyvsp[0].node)
		   add_symbols(yyvsp[0].node);

	       ;
    break;}
case 118:
#line 2382 "parser.y"
{
	       class_rename = make_name(0,0);
	       if (strcmp(yyvsp[-1].id,"class") == 0) {
		 cplus_mode = CPLUS_PRIVATE;
	       } else {
		 cplus_mode = CPLUS_PUBLIC;
	       }
	       Swig_symbol_newscope();
	       cparse_start_line = cparse_line;
	       inclass = 1;
	       Classprefix = NewString("");
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
             ;
    break;}
case 119:
#line 2394 "parser.y"
{
	       String *unnamed;
	       Node *n, *p, *pp = 0;
	       Classprefix = 0;
	       inclass = 0;
	       unnamed = make_unnamed();
	       yyval.node = new_node("class");
	       Setline(yyval.node,cparse_start_line);
	       Setattr(yyval.node,"kind",yyvsp[-6].id);
	       Setattr(yyval.node,"storage",yyvsp[-7].id);
	       Setattr(yyval.node,"unnamed",unnamed);
	       Setattr(yyval.node,"allows_typedef","1");

	       /* Check for pure-abstract class */
	       Setattr(yyval.node,"abstract", pure_abstract(yyvsp[-3].node));

	       n = new_node("cdecl");
	       Setattr(n,"name",yyvsp[-1].decl.id);
	       Setattr(n,"unnamed",unnamed);
	       Setattr(n,"type",unnamed);
	       Setattr(n,"decl",yyvsp[-1].decl.type);
	       Setattr(n,"parms",yyvsp[-1].decl.parms);
	       Setattr(n,"storage",yyvsp[-7].id);
	       pp = n;
	       if (yyvsp[0].node) {
		 set_nextSibling(n,yyvsp[0].node);
		 p = yyvsp[0].node;
		 while (p) {
		   pp = p;
		   Setattr(p,"unnamed",unnamed);
		   Setattr(p,"type",Copy(unnamed));
		   Setattr(p,"storage",yyvsp[-7].id);
		   p = nextSibling(p);
		 }
	       }
	       set_nextSibling(yyval.node,n);
	       {
		 /* If a proper typedef name was given, we'll use it to set the scope name */
		 String *name = 0;
		 if (yyvsp[-7].id && (strcmp(yyvsp[-7].id,"typedef") == 0)) {
		   if (!Len(yyvsp[-1].decl.type)) {	
		     name = yyvsp[-1].decl.id;
		     Setattr(yyval.node,"tdname",name);
		     Setattr(yyval.node,"name",name);
		     if (!class_rename) class_rename = NewString(name);
		     Swig_symbol_setscopename(name);

		     /* If a proper name given, we use that as the typedef, not unnamed */
		     Clear(unnamed);
		     Append(unnamed, name);
		     
		     n = nextSibling(n);
		     set_nextSibling(yyval.node,n);

		     /* Check for previous extensions */
		     if (extendhash) {
		       String *clsname = Swig_symbol_qualifiedscopename(0);
		       Node *am = Getattr(extendhash,clsname);
		       if (am) {
			 /* Merge the extension into the symbol table */
			 merge_extensions(am);
			 appendChild(yyval.node,am);
			 Delattr(extendhash,clsname);
		       }
		       Delete(clsname);
		     }
		     if (!classes) classes = NewHash();
		     Setattr(classes,Swig_symbol_qualifiedscopename(0),yyval.node);
		   } else {
		     Swig_symbol_setscopename((char*)"<unnamed>");
		   }
		 }
		 appendChild(yyval.node,yyvsp[-3].node);
		 appendChild(yyval.node,dump_nested(Char(name)));
	       }
	       /* Pop the scope */
	       Setattr(yyval.node,"symtab",Swig_symbol_popscope());
	       if (class_rename) {
		 yyrename = NewString(class_rename);
	       }
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	       add_symbols(yyval.node);
	       add_symbols(n);
              ;
    break;}
case 120:
#line 2480 "parser.y"
{ yyval.node = 0; ;
    break;}
case 121:
#line 2481 "parser.y"
{
                        yyval.node = new_node("cdecl");
                        Setattr(yyval.node,"name",yyvsp[-1].decl.id);
                        Setattr(yyval.node,"decl",yyvsp[-1].decl.type);
                        Setattr(yyval.node,"parms",yyvsp[-1].decl.parms);
			set_nextSibling(yyval.node,yyvsp[0].node);
                    ;
    break;}
case 122:
#line 2493 "parser.y"
{
              if (yyvsp[-3].id && (Strcmp(yyvsp[-3].id,"friend") == 0)) {
		/* Ignore */
                yyval.node = 0; 
	      } else {
		yyval.node = new_node("classforward");
		Setattr(yyval.node,"kind",yyvsp[-2].id);
		Setattr(yyval.node,"name",yyvsp[-1].str);
		Setattr(yyval.node,"sym:weak", "1");
		add_symbols(yyval.node);
	      }
             ;
    break;}
case 123:
#line 2511 "parser.y"
{ template_parameters = yyvsp[-1].tparms; ;
    break;}
case 124:
#line 2511 "parser.y"
{
		      String *tname = 0;
		      int     error = 0;

                      template_parameters = 0;
                      yyval.node = yyvsp[0].node;
		      if (yyval.node) tname = Getattr(yyval.node,"name");
		      
		      /* Check if the class is a template specialization */
		      if ((yyval.node) && (Strstr(tname,"<")) && (Strncmp(tname,"operator ",9) != 0)) {
			/* If a specialization.  Check if defined. */
			Node *tempn = 0;
			{
			  String *tbase = SwigType_templateprefix(tname);
			  tempn = Swig_symbol_clookup_local(tbase,0);
			  if (!tempn || (Strcmp(nodeType(tempn),"template") != 0)) {
			    Swig_warning(WARN_PARSE_TEMPLATE_SP_UNDEF, Getfile(yyval.node),Getline(yyval.node),"Specialization of non-template '%s'.\n", tbase);
			    tempn = 0;
			    error = 1;
			  }
			  Delete(tbase);
			}
			Setattr(yyval.node,"specialization","1");
			Setattr(yyval.node,"templatetype",nodeType(yyval.node));
			set_nodeType(yyval.node,"template");
			/* Template partial specialization */
			if (tempn && (yyvsp[-3].tparms) && (yyvsp[0].node)) {
			  List   *tlist;
			  String *targs = SwigType_templateargs(tname);
			  tlist = SwigType_parmlist(targs);
			  /*			      Printf(stdout,"targs = '%s'\n", targs); */
			  if (!Getattr(yyval.node,"sym:weak")) {
			    Setattr(yyval.node,"sym:typename","1");
			  }
			  /* Patch the parameter list */
			  if (tempn) {
			    Parm *p,*p1;
			    ParmList *tp = CopyParmList(Getattr(tempn,"templateparms"));
			    p = yyvsp[-3].tparms;
			    p1 = tp;
			    while (p && p1) {
			      String *pn = Getattr(p,"name");
			      if (pn) Setattr(p1,"name",pn);
			      pn = Getattr(p,"type");
			      if (pn) Setattr(p1,"type",pn);
			      p = nextSibling(p);
			      p1 = nextSibling(p1);
			    }
			    Setattr(yyval.node,"templateparms",tp);
			  } else {
			    Setattr(yyval.node,"templateparms",yyvsp[-3].tparms);
			  }
			  Delattr(yyval.node,"specialization");
			  Setattr(yyval.node,"partialspecialization","1");
			  /* Create a specialized name for matching */
			  {
			    Parm *p = yyvsp[-3].tparms;
			    String *fname = NewString(Getattr(yyval.node,"name"));
			    String *ffname = 0;

			    char   tmp[32];
			    int    i;
			    while (p) {
			      String *n = Getattr(p,"name");
			      if (!n) {
				p = nextSibling(p);
				continue;
			      }
			      for (i = 0; i < Len(tlist); i++) {
				if (Strstr(Getitem(tlist,i),n)) {
				  sprintf(tmp,"$%d",i+1);
				  Replaceid(fname,n,tmp);
				}
			      }
			      p = nextSibling(p);
			    }
			    /* Patch argument names with typedef */
			    {
			      SwigType *tt;
			      List *tparms = SwigType_parmlist(fname);
			      ffname = SwigType_templateprefix(fname);
			      Append(ffname,"<(");
			      for (tt = Firstitem(tparms); tt; ) {
				SwigType *ttr = Swig_symbol_typedef_reduce(tt,0);
				ttr = Swig_symbol_type_qualify(ttr,0);
				Append(ffname,ttr);
				tt = Nextitem(tparms);
				if (tt) Putc(',',ffname);
			      }
			      Append(ffname,")>");
			    }
			    {
			      String *partials = Getattr(tempn,"partials");
			      if (!partials) {
				partials = NewList();
				Setattr(tempn,"partials",partials);
			      }
			      /*			      Printf(stdout,"partial: fname = '%s', '%s'\n", fname, Swig_symbol_typedef_reduce(fname,0)); */
			      Append(partials,ffname);
			    }
			    Setattr(yyval.node,"partialargs",ffname);
			    Swig_symbol_cadd(ffname,yyval.node);
			  }
			  Delete(tlist);
			  Delete(targs);
			} else {
			  /* Need to resolve exact specialization name */
			  /* This needs to be rewritten */
			  List *tparms;
			  String *fname;
			  SwigType *tt;
			  fname = SwigType_templateprefix(tname);
			  tparms = SwigType_parmlist(tname);
			  Append(fname,"<(");
			  for (tt = Firstitem(tparms); tt; ) {
			    SwigType *ttr = Swig_symbol_typedef_reduce(tt,0);
			    ttr = Swig_symbol_type_qualify(ttr,0);
			    Append(fname,ttr);
			    tt = Nextitem(tparms);
			    if (tt) Putc(',',fname);
			  }
			  Append(fname,")>");
			  Swig_symbol_cadd(fname,yyval.node);
			}
		      }  else if (yyval.node) {
			Setattr(yyval.node,"templatetype",nodeType(yyvsp[0].node));
			set_nodeType(yyval.node,"template");
			Setattr(yyval.node,"templateparms", yyvsp[-3].tparms);
			if (!Getattr(yyval.node,"sym:weak")) {
			  Setattr(yyval.node,"sym:typename","1");
			}
			add_symbols(yyval.node);
			/* We also place a fully parameterized version in the symbol table */
			{
			  Parm *p;
			  String *fname = NewStringf("%s<(",Getattr(yyval.node,"name"));
			  p = yyvsp[-3].tparms;
			  while (p) {
			    String *n = Getattr(p,"name");
			    if (!n) n = Getattr(p,"type");
			    Printf(fname,"%s", n);
			    p = nextSibling(p);
			    if (p) Putc(',',fname);
			  }
			  Printf(fname,")>");
			  Swig_symbol_cadd(fname,yyval.node);
			}
		      }
		      if (error) yyval.node = 0;
                  ;
    break;}
case 125:
#line 2663 "parser.y"
{
		  yyval.node = yyvsp[0].node;
                ;
    break;}
case 126:
#line 2666 "parser.y"
{
                   yyval.node = yyvsp[0].node;
                ;
    break;}
case 127:
#line 2669 "parser.y"
{
                   yyval.node = yyvsp[0].node;
                ;
    break;}
case 128:
#line 2672 "parser.y"
{
		  yyval.node = 0;
                ;
    break;}
case 129:
#line 2675 "parser.y"
{
                  yyval.node = yyvsp[0].node;
                ;
    break;}
case 130:
#line 2680 "parser.y"
{
		   /* Rip out the parameter names */
		  Parm *p = yyvsp[0].pl;
		  yyval.tparms = yyvsp[0].pl;

		  while (p) {
		    String *name = Getattr(p,"name");
		    if (!name) {
		      /* Hmmm. Maybe it's a 'class T' parameter */
		      char *type = Char(Getattr(p,"type"));
		      /* Template template parameter */
		      if (strncmp(type,"template<class> ",16) == 0) {
			type += 16;
		      }
		      if ((strncmp(type,"class ",6) == 0) || (strncmp(type,"typename ", 9) == 0)) {
			char *t = strchr(type,' ');
			Setattr(p,"name", t+1);
		      } else {
			/*
			 Swig_error(cparse_file, cparse_line, "Missing template parameter name\n");
			 $$.rparms = 0;
			 $$.parms = 0;
			 break; */
		      }
		    }
		    p = nextSibling(p);
		  }
                 ;
    break;}
case 131:
#line 2712 "parser.y"
{
                  yyval.node = new_node("using");
		  Setattr(yyval.node,"uname",yyvsp[-1].str);
		  Setattr(yyval.node,"name", Swig_scopename_last(yyvsp[-1].str));
		  add_symbols(yyval.node);
             ;
    break;}
case 132:
#line 2718 "parser.y"
{
	       Node *n = Swig_symbol_clookup(yyvsp[-1].str,0);
	       if (!n) {
		 Swig_error(cparse_file, cparse_line, "Nothing known about namespace '%s'\n", yyvsp[-1].str);
		 yyval.node = 0;
	       } else {

		 while (Strcmp(nodeType(n),"using") == 0) {
		   n = Getattr(n,"node");
		 }
		 if (n) {
		   if (Strcmp(nodeType(n),"namespace") == 0) {
		     yyval.node = new_node("using");
		     Setattr(yyval.node,"node",n);
		     Setattr(yyval.node,"namespace", yyvsp[-1].str);
		     Swig_symbol_inherit(Getattr(n,"symtab"));
		   } else {
		     Swig_error(cparse_file, cparse_line, "'%s' is not a namespace.\n", yyvsp[-1].str);
		     yyval.node = 0;
		   }
		 } else {
		   yyval.node = 0;
		 }
	       }
             ;
    break;}
case 133:
#line 2745 "parser.y"
{ 
                Hash *h;
                yyvsp[-2].node = Swig_symbol_current();
		h = Swig_symbol_clookup(yyvsp[-1].str,0);
		if (h && (Strcmp(nodeType(h),"namespace") == 0)) {
		  if (Getattr(h,"alias")) {
		    h = Getattr(h,"namespace");
		    Swig_warning(WARN_PARSE_NAMESPACE_ALIAS, cparse_file, cparse_line, "Namespace alias '%s' not allowed here. Assuming '%s'\n",
				 yyvsp[-1].str, Getattr(h,"name"));
		    yyvsp[-1].str = Getattr(h,"name");
		  }
		  Swig_symbol_setscope(Getattr(h,"symtab"));
		} else {
		  Swig_symbol_newscope();
		  Swig_symbol_setscopename(yyvsp[-1].str);
		}
		Namespaceprefix = Swig_symbol_qualifiedscopename(0);
             ;
    break;}
case 134:
#line 2762 "parser.y"
{
                Node *n = yyvsp[-1].node;
		set_nodeType(n,"namespace");
		Setattr(n,"name",yyvsp[-4].str);
                Setattr(n,"symtab", Swig_symbol_popscope());
		Swig_symbol_setscope(yyvsp[-5].node);
		yyval.node = n;
		Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		add_symbols(yyval.node);
             ;
    break;}
case 135:
#line 2772 "parser.y"
{
	       Hash *h;
	       yyvsp[-1].node = Swig_symbol_current();
	       h = Swig_symbol_clookup("",0);
	       if (h && (Strcmp(nodeType(h),"namespace") == 0)) {
		 Swig_symbol_setscope(Getattr(h,"symtab"));
	       } else {
		 Swig_symbol_newscope();
		 Swig_symbol_setscopename("__unnamed__");
	       }
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
             ;
    break;}
case 136:
#line 2783 "parser.y"
{
	       yyval.node = yyvsp[-1].node;
	       set_nodeType(yyval.node,"namespace");
	       Setattr(yyval.node,"unnamed","1");
	       Setattr(yyval.node,"symtab", Swig_symbol_popscope());
	       Swig_symbol_setscope(yyvsp[-4].node);
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	       add_symbols(yyval.node);
             ;
    break;}
case 137:
#line 2792 "parser.y"
{
	       /* Namespace alias */
	       Node *n;
	       yyval.node = new_node("namespace");
	       Setattr(yyval.node,"name",yyvsp[-3].id);
	       Setattr(yyval.node,"alias",yyvsp[-1].str);
	       n = Swig_symbol_clookup(yyvsp[-1].str,0);
	       if (!n) {
		 Swig_error(cparse_file, cparse_line, "Unknown namespace '%s'\n", yyvsp[-1].str);
		 yyval.node = 0;
	       } else {
		 if (Strcmp(nodeType(n),"namespace") != 0) {
		   Swig_error(cparse_file, cparse_line, "'%s' is not a namespace\n",yyvsp[-1].str);
		   yyval.node = 0;
		 } else {
		   while (Getattr(n,"alias")) {
		     n = Getattr(n,"namespace");
		   }
		   Setattr(yyval.node,"namespace",n);
		   add_symbols(yyval.node);
		   /* Set up a scope alias */
		   Swig_symbol_alias(yyvsp[-3].id,Getattr(n,"symtab"));
		 }
	       }
             ;
    break;}
case 138:
#line 2819 "parser.y"
{
                   yyval.node = yyvsp[-1].node;
		   if (yyval.node) {
		     Node *p = yyval.node;
		     Node *pp =0;
		     while (p) {
		       pp = p;
		       p = nextSibling(p);
		     }
		     set_nextSibling(pp,yyvsp[0].node);
		   } else {
		     yyval.node = yyvsp[0].node;
		   }
             ;
    break;}
case 139:
#line 2833 "parser.y"
{ 
                  if (cplus_mode != CPLUS_PUBLIC) {
		     Swig_error(cparse_file,cparse_line,"%%extend can only be used in a public section\n");
		  }
             ;
    break;}
case 140:
#line 2837 "parser.y"
{
	       yyval.node = new_node("extend");
	       Swig_tag_nodes(yyvsp[-2].node,"feature:extend",(char*) "1");
	       appendChild(yyval.node,yyvsp[-2].node);
	       set_nextSibling(yyval.node,yyvsp[0].node);
	     ;
    break;}
case 141:
#line 2843 "parser.y"
{ yyval.node = 0;;
    break;}
case 142:
#line 2844 "parser.y"
{
	       skip_decl();
		   {
		     static int last_error_line = -1;
		     if (last_error_line != cparse_line) {
		       Swig_error(cparse_file, cparse_line,"Syntax error in input.\n");
		       last_error_line = cparse_line;
		     }
		   }
	     ;
    break;}
case 143:
#line 2853 "parser.y"
{ 
                yyval.node = yyvsp[0].node;
             ;
    break;}
case 144:
#line 2864 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 145:
#line 2865 "parser.y"
{ 
                 yyval.node = yyvsp[0].node; 
		 add_symbols(yyval.node);
             ;
    break;}
case 146:
#line 2869 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 147:
#line 2870 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 148:
#line 2871 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 149:
#line 2872 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 150:
#line 2873 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 151:
#line 2874 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 152:
#line 2875 "parser.y"
{ yyval.node = 0; ;
    break;}
case 153:
#line 2876 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 154:
#line 2877 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 155:
#line 2878 "parser.y"
{ yyval.node = 0; ;
    break;}
case 156:
#line 2879 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 157:
#line 2880 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 158:
#line 2881 "parser.y"
{ yyval.node = 0; ;
    break;}
case 159:
#line 2890 "parser.y"
{
              if (Classprefix) {
		 SwigType *decl = NewString("");
		 yyval.node = new_node("constructor");

		 /* Since the parse performs type-corrections in template mode, we
                    have to undo the correction here.  Ugh. */

		 /* Check for template names.  If the class is a template
                    and the constructor is missing the template part, we
                    add it */
		 /*		 {
		   char *c = Strstr(Classprefix,"<");
		   if (c) {
		     if (!Strstr($2,"<")) {
		       Append($2,c);
		     }
		   }
		 }
		 */
		 Setattr(yyval.node,"name",yyvsp[-4].type);
		 Setattr(yyval.node,"parms",yyvsp[-2].pl);
		 SwigType_add_function(decl,yyvsp[-2].pl);
		 Setattr(yyval.node,"decl",decl);
		 Setattr(yyval.node,"throws",yyvsp[0].decl.throws);
		 if (Len(scanner_ccode)) {
		   Setattr(yyval.node,"code",Copy(scanner_ccode));
		 }
		 Setattr(yyval.node,"feature:new","1");
	      } else {
		yyval.node = 0;
              }
              ;
    break;}
case 160:
#line 2927 "parser.y"
{
               yyval.node = new_node("destructor");
	       Setattr(yyval.node,"name",NewStringf("~%s",yyvsp[-4].str));
	       if (Len(scanner_ccode)) {
		 Setattr(yyval.node,"code",Copy(scanner_ccode));
	       }
	       {
		 String *decl = NewString("");
		 SwigType_add_function(decl,yyvsp[-2].pl);
		 Setattr(yyval.node,"decl",decl);
	       }
	       add_symbols(yyval.node);
	      ;
    break;}
case 161:
#line 2943 "parser.y"
{
		yyval.node = new_node("destructor");
	       /* Check for template names.  If the class is a template
		  and the constructor is missing the template part, we
		  add it */
	       {
		 char *c = Strstr(Classprefix,"<");
		 if (c) {
		   if (!Strstr(yyvsp[-4].str,"<")) {
		     yyvsp[-4].str = NewStringf("%s%s",yyvsp[-4].str,c);
		   }
		 }
	       }
		Setattr(yyval.node,"storage","virtual");
		Setattr(yyval.node,"name",NewStringf("~%s",yyvsp[-4].str));
		if (yyvsp[0].dtype.val) {
		  Setattr(yyval.node,"value","0");
		}
		if (Len(scanner_ccode)) {
		  Setattr(yyval.node,"code",Copy(scanner_ccode));
		}
		{
		  String *decl = NewString("");
		  SwigType_add_function(decl,yyvsp[-2].pl);
		  Setattr(yyval.node,"decl",decl);
		}

		add_symbols(yyval.node);
	      ;
    break;}
case 162:
#line 2976 "parser.y"
{
                 yyval.node = new_node("cdecl");
                 Setattr(yyval.node,"type",yyvsp[-5].type);
		 Setattr(yyval.node,"name",yyvsp[-6].str);

		 SwigType_add_function(yyvsp[-4].type,yyvsp[-2].pl);
		 if (yyvsp[0].dtype.qualifier) {
		   SwigType_push(yyvsp[-4].type,yyvsp[0].dtype.qualifier);
		 }
		 Setattr(yyval.node,"decl",yyvsp[-4].type);
		 Setattr(yyval.node,"parms",yyvsp[-2].pl);
		 Setattr(yyval.node,"conversion_operator","1");
		 add_symbols(yyval.node);
              ;
    break;}
case 163:
#line 2990 "parser.y"
{
		 SwigType *decl;
                 yyval.node = new_node("cdecl");
                 Setattr(yyval.node,"type",yyvsp[-5].type);
		 Setattr(yyval.node,"name",yyvsp[-6].str);
		 decl = NewString("");
		 SwigType_add_reference(decl);
		 SwigType_add_function(decl,yyvsp[-2].pl);
		 if (yyvsp[0].dtype.qualifier) {
		   SwigType_push(decl,yyvsp[0].dtype.qualifier);
		 }
		 Setattr(yyval.node,"decl",decl);
		 Setattr(yyval.node,"parms",yyvsp[-2].pl);
		 Setattr(yyval.node,"conversion_operator","1");
		 add_symbols(yyval.node);
	       ;
    break;}
case 164:
#line 3007 "parser.y"
{
		String *t = NewString("");
		yyval.node = new_node("cdecl");
		Setattr(yyval.node,"type",yyvsp[-4].type);
		Setattr(yyval.node,"name",yyvsp[-5].str);
		SwigType_add_function(t,yyvsp[-2].pl);
		if (yyvsp[0].dtype.qualifier) {
		  SwigType_push(t,yyvsp[0].dtype.qualifier);
		}
		Setattr(yyval.node,"decl",t);
		Setattr(yyval.node,"parms",yyvsp[-2].pl);
		Setattr(yyval.node,"conversion_operator","1");
		add_symbols(yyval.node);
              ;
    break;}
case 165:
#line 3025 "parser.y"
{
                 skip_balanced('{','}');
                 yyval.node = 0;
               ;
    break;}
case 166:
#line 3032 "parser.y"
{ 
                yyval.node = new_node("access");
		Setattr(yyval.node,"kind","public");
                cplus_mode = CPLUS_PUBLIC;
              ;
    break;}
case 167:
#line 3039 "parser.y"
{ 
                yyval.node = new_node("access");
                Setattr(yyval.node,"kind","private");
		cplus_mode = CPLUS_PRIVATE;
	      ;
    break;}
case 168:
#line 3047 "parser.y"
{ 
		yyval.node = new_node("access");
		Setattr(yyval.node,"kind","protected");
		cplus_mode = CPLUS_PROTECTED;
	      ;
    break;}
case 169:
#line 3070 "parser.y"
{ cparse_start_line = cparse_line; skip_balanced('{','}');
	      ;
    break;}
case 170:
#line 3071 "parser.y"
{
	        yyval.node = 0;
		if (cplus_mode == CPLUS_PUBLIC) {
		  if (yyvsp[-1].decl.id) {
		    if (strcmp(yyvsp[-5].id,"class") == 0) {
		      Swig_warning(WARN_PARSE_NESTED_CLASS, cparse_file, cparse_line, "Nested classes not currently supported (ignored).\n");
		      /* Generate some code for a new class */
		    } else {
		      Nested *n = (Nested *) malloc(sizeof(Nested));
		      n->code = NewString("");
		      Printv(n->code, "typedef ", yyvsp[-5].id, " ",
			     Char(scanner_ccode), " $classname_", yyvsp[-1].decl.id, ";\n", NIL);

		      n->name = Swig_copy_string(yyvsp[-1].decl.id);
		      n->line = cparse_start_line;
		      n->type = NewString("");
		      n->kind = yyvsp[-5].id;
		      SwigType_push(n->type, yyvsp[-1].decl.type);
		      n->next = 0;
		      add_nested(n);
		    }
		  } else {
		    Swig_warning(WARN_PARSE_NESTED_CLASS, cparse_file, cparse_line, "Nested %s not currently supported (ignored).\n", yyvsp[-5].id);
		  }
		}
	      ;
    break;}
case 171:
#line 3099 "parser.y"
{ cparse_start_line = cparse_line; skip_balanced('{','}');
              ;
    break;}
case 172:
#line 3100 "parser.y"
{
	        yyval.node = 0;
		if (cplus_mode == CPLUS_PUBLIC) {
		  if (strcmp(yyvsp[-4].id,"class") == 0) {
		    Swig_warning(WARN_PARSE_NESTED_CLASS,cparse_file, cparse_line,"Nested class not currently supported (ignored)\n");
		    /* Generate some code for a new class */
		  } else if (yyvsp[-1].decl.id) {
		    /* Generate some code for a new class */
		    Nested *n = (Nested *) malloc(sizeof(Nested));
		    n->code = NewString("");
		    Printv(n->code, "typedef ", yyvsp[-4].id, " " ,
			    Char(scanner_ccode), " $classname_", yyvsp[-1].decl.id, ";\n",NIL);
		    n->name = Swig_copy_string(yyvsp[-1].decl.id);
		    n->line = cparse_start_line;
		    n->type = NewString("");
		    n->kind = yyvsp[-4].id;
		    SwigType_push(n->type,yyvsp[-1].decl.type);
		    n->next = 0;
		    add_nested(n);
		  } else {
		    Swig_warning(WARN_PARSE_NESTED_CLASS, cparse_file, cparse_line, "Nested %s not currently supported (ignored).\n", yyvsp[-4].id);
		  }
		}
	      ;
    break;}
case 173:
#line 3126 "parser.y"
{ yyval.decl = yyvsp[0].decl;;
    break;}
case 174:
#line 3127 "parser.y"
{ yyval.decl.id = 0; ;
    break;}
case 175:
#line 3133 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 176:
#line 3136 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 177:
#line 3140 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 178:
#line 3143 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 179:
#line 3144 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 180:
#line 3145 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 181:
#line 3146 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 182:
#line 3147 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 183:
#line 3148 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 184:
#line 3149 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 185:
#line 3150 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 186:
#line 3153 "parser.y"
{
	            Clear(scanner_ccode);
               ;
    break;}
case 187:
#line 3156 "parser.y"
{ skip_balanced('{','}'); ;
    break;}
case 188:
#line 3159 "parser.y"
{ 
                     Clear(scanner_ccode);
                     yyval.dtype.val = 0;
                     yyval.dtype.qualifier = yyvsp[-1].dtype.qualifier;
                     yyval.dtype.bitfield = 0;
                     yyval.dtype.throws = yyvsp[-1].dtype.throws;
                ;
    break;}
case 189:
#line 3166 "parser.y"
{ 
                     Clear(scanner_ccode);
                     yyval.dtype.val = yyvsp[-1].dtype.val;
                     yyval.dtype.qualifier = yyvsp[-3].dtype.qualifier;
                     yyval.dtype.bitfield = 0;
                     yyval.dtype.throws = yyvsp[-3].dtype.throws; 
               ;
    break;}
case 190:
#line 3173 "parser.y"
{ 
                     skip_balanced('{','}');
                     yyval.dtype.val = 0;
                     yyval.dtype.qualifier = yyvsp[-1].dtype.qualifier;
                     yyval.dtype.bitfield = 0;
                     yyval.dtype.throws = yyvsp[-1].dtype.throws; 
               ;
    break;}
case 191:
#line 3187 "parser.y"
{ yyval.id = "extern"; ;
    break;}
case 192:
#line 3188 "parser.y"
{ 
                   if (strcmp(yyvsp[0].id,"C") == 0) {
		     yyval.id = "externc";
		   } else {
		     Swig_warning(WARN_PARSE_UNDEFINED_EXTERN,cparse_file, cparse_line,"Unrecognized extern type \"%s\" (ignored).\n", yyvsp[0].id);
		     yyval.id = 0;
		   }
               ;
    break;}
case 193:
#line 3196 "parser.y"
{ yyval.id = "static"; ;
    break;}
case 194:
#line 3197 "parser.y"
{ yyval.id = "typedef"; ;
    break;}
case 195:
#line 3198 "parser.y"
{ yyval.id = "virtual"; ;
    break;}
case 196:
#line 3199 "parser.y"
{ yyval.id = "friend"; ;
    break;}
case 197:
#line 3200 "parser.y"
{ yyval.id = 0; ;
    break;}
case 198:
#line 3207 "parser.y"
{
                 Parm *p;
		 yyval.pl = yyvsp[0].pl;
		 p = yyvsp[0].pl;
                 while (p) {
		   Replace(Getattr(p,"type"),"typename ", "", DOH_REPLACE_ANY);
		   p = nextSibling(p);
                 }
               ;
    break;}
case 199:
#line 3218 "parser.y"
{
		  if (1) { 
		    set_nextSibling(yyvsp[-1].p,yyvsp[0].pl);
		    yyval.pl = yyvsp[-1].p;
		  } else {
		    yyval.pl = yyvsp[0].pl;
		  }
		;
    break;}
case 200:
#line 3226 "parser.y"
{ yyval.pl = 0; ;
    break;}
case 201:
#line 3229 "parser.y"
{
                 set_nextSibling(yyvsp[-1].p,yyvsp[0].pl);
		 yyval.pl = yyvsp[-1].p;
                ;
    break;}
case 202:
#line 3233 "parser.y"
{ yyval.pl = 0; ;
    break;}
case 203:
#line 3237 "parser.y"
{
                   SwigType_push(yyvsp[-1].type,yyvsp[0].decl.type);
		   yyval.p = NewParm(yyvsp[-1].type,yyvsp[0].decl.id);
		   Setfile(yyval.p,cparse_file);
		   Setline(yyval.p,cparse_line);
		   if (yyvsp[0].decl.defarg) {
		     Setattr(yyval.p,"value",yyvsp[0].decl.defarg);
		   }
		;
    break;}
case 204:
#line 3247 "parser.y"
{
                  yyval.p = NewParm(NewStringf("template<class> %s %s", yyvsp[-1].id,yyvsp[0].str), 0);
		  Setfile(yyval.p,cparse_file);
		  Setline(yyval.p,cparse_line);
                ;
    break;}
case 205:
#line 3252 "parser.y"
{
		  SwigType *t = NewString("v(...)");
		  yyval.p = NewParm(t, 0);
		  Setfile(yyval.p,cparse_file);
		  Setline(yyval.p,cparse_line);
		;
    break;}
case 206:
#line 3260 "parser.y"
{
                 Parm *p;
		 yyval.p = yyvsp[0].p;
		 p = yyvsp[0].p;
                 while (p) {
		   if (Getattr(p,"type")) {
		     Replace(Getattr(p,"type"),"typename ", "", DOH_REPLACE_ANY);
		   }
		   p = nextSibling(p);
                 }
               ;
    break;}
case 207:
#line 3273 "parser.y"
{
		  if (1) { 
		    set_nextSibling(yyvsp[-1].p,yyvsp[0].p);
		    yyval.p = yyvsp[-1].p;
		  } else {
		    yyval.p = yyvsp[0].p;
		  }
		;
    break;}
case 208:
#line 3281 "parser.y"
{ yyval.p = 0; ;
    break;}
case 209:
#line 3284 "parser.y"
{
                 set_nextSibling(yyvsp[-1].p,yyvsp[0].p);
		 yyval.p = yyvsp[-1].p;
                ;
    break;}
case 210:
#line 3288 "parser.y"
{ yyval.p = 0; ;
    break;}
case 211:
#line 3292 "parser.y"
{
		  yyval.p = yyvsp[0].p;
		  {
		    /* We need to make a possible adjustment for integer parameters. */
		    SwigType *type;
		    Node     *n = 0;

		    while (!n) {
		      type = Getattr(yyvsp[0].p,"type");
		      n = Swig_symbol_clookup(type,0);     /* See if we can find a node that matches the typename */
		      if ((n) && (Strcmp(nodeType(n),"cdecl") == 0)) {
			SwigType *decl = Getattr(n,"decl");
			if (!SwigType_isfunction(decl)) {
			  String *value = Getattr(n,"value");
			  if (value) {
			    Setattr(yyvsp[0].p,"type",Copy(value));
			    n = 0;
			  }
			}
		      } else {
			break;
		      }
		    }
		  }
		  
               ;
    break;}
case 212:
#line 3318 "parser.y"
{
                  yyval.p = NewParm(0,0);
                  Setfile(yyval.p,cparse_file);
		  Setline(yyval.p,cparse_line);
		  Setattr(yyval.p,"value",yyvsp[0].dtype.val);
               ;
    break;}
case 213:
#line 3324 "parser.y"
{
                  yyval.p = NewParm(0,0);
                  Setfile(yyval.p,cparse_file);
		  Setline(yyval.p,cparse_line);
		  Setattr(yyval.p,"value",NewString(yyvsp[0].id));
               ;
    break;}
case 214:
#line 3332 "parser.y"
{ 
                  yyval.dtype = yyvsp[0].dtype; 
		  if (yyvsp[0].dtype.type == T_ERROR) {
		    Swig_warning(WARN_PARSE_BAD_DEFAULT,cparse_file, cparse_line, "Can't set default argument (ignored)\n");
		    yyval.dtype.val = 0;
		    yyval.dtype.rawval = 0;
		    yyval.dtype.bitfield = 0;
		    yyval.dtype.throws = 0;
		  }
               ;
    break;}
case 215:
#line 3342 "parser.y"
{
		 Node *n = Swig_symbol_clookup(yyvsp[0].str,0);
		 if (n) {
		   String *q = Swig_symbol_qualified(n);
		   if (Getattr(n,"access")) {
		     if (cplus_mode == CPLUS_PUBLIC) {
		       Swig_warning(WARN_PARSE_PRIVATE, cparse_file, cparse_line,"'%s' is private in this context.\n", yyvsp[0].str);
		       Swig_warning(WARN_PARSE_BAD_DEFAULT, cparse_file, cparse_line,"Can't set default argument value (ignored)\n");
		     }
		     yyval.dtype.val = 0;
		   } else {
		     if (q) {
		       yyval.dtype.val = NewStringf("&%s::%s", q,Getattr(n,"name"));
		       Delete(q);
		     } else {
		       yyval.dtype.val = NewStringf("&%s", yyvsp[0].str);
		     }
		   }
		 } else {
		   yyval.dtype.val = NewStringf("&%s",yyvsp[0].str);
		 }
		 yyval.dtype.rawval = 0;
		 yyval.dtype.type = T_USER;
		 yyval.dtype.bitfield = 0;
		 yyval.dtype.throws = 0;
	       ;
    break;}
case 216:
#line 3368 "parser.y"
{
		 skip_balanced('{','}');
		 yyval.dtype.val = 0;
		 yyval.dtype.rawval = 0;
                 yyval.dtype.type = T_INT;
		 yyval.dtype.bitfield = 0;
		 yyval.dtype.throws = 0;
	       ;
    break;}
case 217:
#line 3376 "parser.y"
{ 
		 yyval.dtype.val = 0;
		 yyval.dtype.rawval = 0;
		 yyval.dtype.type = 0;
		 yyval.dtype.bitfield = yyvsp[0].dtype.val;
		 yyval.dtype.throws = 0;
	       ;
    break;}
case 218:
#line 3383 "parser.y"
{
                 yyval.dtype.val = 0;
                 yyval.dtype.rawval = 0;
                 yyval.dtype.type = T_INT;
		 yyval.dtype.bitfield = 0;
		 yyval.dtype.throws = 0;
               ;
    break;}
case 219:
#line 3392 "parser.y"
{
                 yyval.decl = yyvsp[-1].decl;
		 yyval.decl.defarg = yyvsp[0].dtype.rawval ? yyvsp[0].dtype.rawval : yyvsp[0].dtype.val;
            ;
    break;}
case 220:
#line 3396 "parser.y"
{
              yyval.decl = yyvsp[-1].decl;
	      yyval.decl.defarg = yyvsp[0].dtype.rawval ? yyvsp[0].dtype.rawval : yyvsp[0].dtype.val;
            ;
    break;}
case 221:
#line 3400 "parser.y"
{
   	      yyval.decl.type = 0;
              yyval.decl.id = 0;
	      yyval.decl.defarg = yyvsp[0].dtype.rawval ? yyvsp[0].dtype.rawval : yyvsp[0].dtype.val;
            ;
    break;}
case 222:
#line 3407 "parser.y"
{
                 yyval.decl = yyvsp[0].decl;
		 if (SwigType_isfunction(yyvsp[0].decl.type)) {
		   Delete(SwigType_pop_function(yyvsp[0].decl.type));
		 } else if (SwigType_isarray(yyvsp[0].decl.type)) {
		   SwigType *ta = SwigType_pop_arrays(yyvsp[0].decl.type);
		   if (SwigType_isfunction(yyvsp[0].decl.type)) {
		     Delete(SwigType_pop_function(yyvsp[0].decl.type));
		   } else {
		     yyval.decl.parms = 0;
		   }
		   SwigType_push(yyvsp[0].decl.type,ta);
		   Delete(ta);
		 } else {
		   yyval.decl.parms = 0;
		 }
            ;
    break;}
case 223:
#line 3424 "parser.y"
{
              yyval.decl = yyvsp[0].decl;
	      if (SwigType_isfunction(yyvsp[0].decl.type)) {
		Delete(SwigType_pop_function(yyvsp[0].decl.type));
	      } else if (SwigType_isarray(yyvsp[0].decl.type)) {
		SwigType *ta = SwigType_pop_arrays(yyvsp[0].decl.type);
		if (SwigType_isfunction(yyvsp[0].decl.type)) {
		  Delete(SwigType_pop_function(yyvsp[0].decl.type));
		} else {
		  yyval.decl.parms = 0;
		}
		SwigType_push(yyvsp[0].decl.type,ta);
		Delete(ta);
	      } else {
		yyval.decl.parms = 0;
	      }
            ;
    break;}
case 224:
#line 3441 "parser.y"
{
   	      yyval.decl.type = 0;
              yyval.decl.id = 0;
	      yyval.decl.parms = 0;
	      ;
    break;}
case 225:
#line 3449 "parser.y"
{
              yyval.decl = yyvsp[0].decl;
	      if (yyval.decl.type) {
		SwigType_push(yyvsp[-1].type,yyval.decl.type);
		Delete(yyval.decl.type);
	      }
	      yyval.decl.type = yyvsp[-1].type;
           ;
    break;}
case 226:
#line 3457 "parser.y"
{
              yyval.decl = yyvsp[0].decl;
	      SwigType_add_reference(yyvsp[-2].type);
              if (yyval.decl.type) {
		SwigType_push(yyvsp[-2].type,yyval.decl.type);
		Delete(yyval.decl.type);
	      }
	      yyval.decl.type = yyvsp[-2].type;
           ;
    break;}
case 227:
#line 3466 "parser.y"
{
              yyval.decl = yyvsp[0].decl;
	      if (!yyval.decl.type) yyval.decl.type = NewString("");
           ;
    break;}
case 228:
#line 3470 "parser.y"
{ 
	     yyval.decl = yyvsp[0].decl;
	     yyval.decl.type = NewString("");
	     SwigType_add_reference(yyval.decl.type);
	     if (yyvsp[0].decl.type) {
	       SwigType_push(yyval.decl.type,yyvsp[0].decl.type);
	       Delete(yyvsp[0].decl.type);
	     }
           ;
    break;}
case 229:
#line 3479 "parser.y"
{ 
	     SwigType *t = NewString("");

	     yyval.decl = yyvsp[0].decl;
	     SwigType_add_memberpointer(t,yyvsp[-2].str);
	     if (yyval.decl.type) {
	       SwigType_push(t,yyval.decl.type);
	       Delete(yyval.decl.type);
	     }
	     yyval.decl.type = t;
	     ;
    break;}
case 230:
#line 3490 "parser.y"
{ 
	     SwigType *t = NewString("");
	     yyval.decl = yyvsp[0].decl;
	     SwigType_add_memberpointer(t,yyvsp[-2].str);
	     SwigType_push(yyvsp[-3].type,t);
	     if (yyval.decl.type) {
	       SwigType_push(yyvsp[-3].type,yyval.decl.type);
	       Delete(yyval.decl.type);
	     }
	     yyval.decl.type = yyvsp[-3].type;
	     Delete(t);
	   ;
    break;}
case 231:
#line 3502 "parser.y"
{ 
	     yyval.decl = yyvsp[0].decl;
	     SwigType_add_memberpointer(yyvsp[-4].type,yyvsp[-3].str);
	     SwigType_add_reference(yyvsp[-4].type);
	     if (yyval.decl.type) {
	       SwigType_push(yyvsp[-4].type,yyval.decl.type);
	       Delete(yyval.decl.type);
	     }
	     yyval.decl.type = yyvsp[-4].type;
	   ;
    break;}
case 232:
#line 3512 "parser.y"
{ 
	     SwigType *t = NewString("");
	     yyval.decl = yyvsp[0].decl;
	     SwigType_add_memberpointer(t,yyvsp[-3].str);
	     SwigType_add_reference(t);
	     if (yyval.decl.type) {
	       SwigType_push(t,yyval.decl.type);
	       Delete(yyval.decl.type);
	     } 
	     yyval.decl.type = t;
	   ;
    break;}
case 233:
#line 3525 "parser.y"
{
                /* Note: This is non-standard C.  Template declarator is allowed to follow an identifier */
                 yyval.decl.id = Char(yyvsp[0].str);
		 yyval.decl.type = 0;
		 yyval.decl.parms = 0;
		 yyval.decl.have_parms = 0;
                  ;
    break;}
case 234:
#line 3533 "parser.y"
{
                  yyval.decl.id = Char(NewStringf("~%s",yyvsp[0].str));
                  yyval.decl.type = 0;
                  yyval.decl.parms = 0;
                  yyval.decl.have_parms = 0;
                  ;
    break;}
case 235:
#line 3541 "parser.y"
{
                  yyval.decl.id = Char(yyvsp[-1].str);
                  yyval.decl.type = 0;
                  yyval.decl.parms = 0;
                  yyval.decl.have_parms = 0;
                  ;
    break;}
case 236:
#line 3549 "parser.y"
{
		    yyval.decl = yyvsp[-1].decl;
		    if (yyval.decl.type) {
		      SwigType_push(yyvsp[-2].type,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = yyvsp[-2].type;
                  ;
    break;}
case 237:
#line 3557 "parser.y"
{
		    SwigType *t;
		    yyval.decl = yyvsp[-1].decl;
		    t = NewString("");
		    SwigType_add_memberpointer(t,yyvsp[-3].str);
		    if (yyval.decl.type) {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = t;
		    ;
    break;}
case 238:
#line 3568 "parser.y"
{ 
		    SwigType *t;
		    yyval.decl = yyvsp[-2].decl;
		    t = NewString("");
		    SwigType_add_array(t,(char*)"");
		    if (yyval.decl.type) {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = t;
                  ;
    break;}
case 239:
#line 3579 "parser.y"
{ 
		    SwigType *t;
		    yyval.decl = yyvsp[-3].decl;
		    t = NewString("");
		    SwigType_add_array(t,yyvsp[-1].dtype.val);
		    if (yyval.decl.type) {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = t;
                  ;
    break;}
case 240:
#line 3590 "parser.y"
{
		    SwigType *t;
                    yyval.decl = yyvsp[-3].decl;
		    t = NewString("");
		    SwigType_add_function(t,yyvsp[-1].pl);
		    if (!yyval.decl.have_parms) {
		      yyval.decl.parms = yyvsp[-1].pl;
		      yyval.decl.have_parms = 1;
		    }
		    if (!yyval.decl.type) {
		      yyval.decl.type = t;
		    } else {
		      SwigType_push(t, yyval.decl.type);
		      Delete(yyval.decl.type);
		      yyval.decl.type = t;
		    }
		  ;
    break;}
case 241:
#line 3609 "parser.y"
{
                /* Note: This is non-standard C.  Template declarator is allowed to follow an identifier */
                 yyval.decl.id = Char(yyvsp[0].str);
		 yyval.decl.type = 0;
		 yyval.decl.parms = 0;
		 yyval.decl.have_parms = 0;
                  ;
    break;}
case 242:
#line 3617 "parser.y"
{
                  yyval.decl.id = Char(NewStringf("~%s",yyvsp[0].str));
                  yyval.decl.type = 0;
                  yyval.decl.parms = 0;
                  yyval.decl.have_parms = 0;
                  ;
    break;}
case 243:
#line 3634 "parser.y"
{
		    yyval.decl = yyvsp[-1].decl;
		    if (yyval.decl.type) {
		      SwigType_push(yyvsp[-2].type,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = yyvsp[-2].type;
                  ;
    break;}
case 244:
#line 3642 "parser.y"
{
		    SwigType *t;
		    yyval.decl = yyvsp[-1].decl;
		    t = NewString("");
		    SwigType_add_memberpointer(t,yyvsp[-3].str);
		    if (yyval.decl.type) {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = t;
		    ;
    break;}
case 245:
#line 3653 "parser.y"
{ 
		    SwigType *t;
		    yyval.decl = yyvsp[-2].decl;
		    t = NewString("");
		    SwigType_add_array(t,(char*)"");
		    if (yyval.decl.type) {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = t;
                  ;
    break;}
case 246:
#line 3664 "parser.y"
{ 
		    SwigType *t;
		    yyval.decl = yyvsp[-3].decl;
		    t = NewString("");
		    SwigType_add_array(t,yyvsp[-1].dtype.val);
		    if (yyval.decl.type) {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = t;
                  ;
    break;}
case 247:
#line 3675 "parser.y"
{
		    SwigType *t;
                    yyval.decl = yyvsp[-3].decl;
		    t = NewString("");
		    SwigType_add_function(t,yyvsp[-1].pl);
		    if (!yyval.decl.have_parms) {
		      yyval.decl.parms = yyvsp[-1].pl;
		      yyval.decl.have_parms = 1;
		    }
		    if (!yyval.decl.type) {
		      yyval.decl.type = t;
		    } else {
		      SwigType_push(t, yyval.decl.type);
		      Delete(yyval.decl.type);
		      yyval.decl.type = t;
		    }
		  ;
    break;}
case 248:
#line 3694 "parser.y"
{
		    yyval.decl.type = yyvsp[0].type;
                    yyval.decl.id = 0;
		    yyval.decl.parms = 0;
		    yyval.decl.have_parms = 0;
                  ;
    break;}
case 249:
#line 3700 "parser.y"
{ 
                     yyval.decl = yyvsp[0].decl;
                     SwigType_push(yyvsp[-1].type,yyvsp[0].decl.type);
		     yyval.decl.type = yyvsp[-1].type;
		     Delete(yyvsp[0].decl.type);
                  ;
    break;}
case 250:
#line 3706 "parser.y"
{
		    yyval.decl.type = yyvsp[-1].type;
		    SwigType_add_reference(yyval.decl.type);
		    yyval.decl.id = 0;
		    yyval.decl.parms = 0;
		    yyval.decl.have_parms = 0;
		  ;
    break;}
case 251:
#line 3713 "parser.y"
{
		    yyval.decl = yyvsp[0].decl;
		    SwigType_add_reference(yyvsp[-2].type);
		    if (yyval.decl.type) {
		      SwigType_push(yyvsp[-2].type,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = yyvsp[-2].type;
                  ;
    break;}
case 252:
#line 3722 "parser.y"
{
		    yyval.decl = yyvsp[0].decl;
                  ;
    break;}
case 253:
#line 3725 "parser.y"
{
		    yyval.decl = yyvsp[0].decl;
		    yyval.decl.type = NewString("");
		    SwigType_add_reference(yyval.decl.type);
		    if (yyvsp[0].decl.type) {
		      SwigType_push(yyval.decl.type,yyvsp[0].decl.type);
		      Delete(yyvsp[0].decl.type);
		    }
                  ;
    break;}
case 254:
#line 3734 "parser.y"
{ 
                    yyval.decl.id = 0;
                    yyval.decl.parms = 0;
		    yyval.decl.have_parms = 0;
                    yyval.decl.type = NewString("");
		    SwigType_add_reference(yyval.decl.type);
                  ;
    break;}
case 255:
#line 3741 "parser.y"
{ 
		    yyval.decl.type = NewString("");
                    SwigType_add_memberpointer(yyval.decl.type,yyvsp[-1].str);
                    yyval.decl.id = 0;
                    yyval.decl.parms = 0;
		    yyval.decl.have_parms = 0;
      	          ;
    break;}
case 256:
#line 3748 "parser.y"
{ 
		    SwigType *t = NewString("");
                    yyval.decl.type = yyvsp[-2].type;
		    yyval.decl.id = 0;
		    yyval.decl.parms = 0;
		    yyval.decl.have_parms = 0;
		    SwigType_add_memberpointer(t,yyvsp[-1].str);
		    SwigType_push(yyval.decl.type,t);
		    Delete(t);
                  ;
    break;}
case 257:
#line 3758 "parser.y"
{ 
		    yyval.decl = yyvsp[0].decl;
		    SwigType_add_memberpointer(yyvsp[-3].type,yyvsp[-2].str);
		    if (yyval.decl.type) {
		      SwigType_push(yyvsp[-3].type,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = yyvsp[-3].type;
                  ;
    break;}
case 258:
#line 3769 "parser.y"
{ 
		    SwigType *t;
		    yyval.decl = yyvsp[-2].decl;
		    t = NewString("");
		    SwigType_add_array(t,(char*)"");
		    if (yyval.decl.type) {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = t;
                  ;
    break;}
case 259:
#line 3780 "parser.y"
{ 
		    SwigType *t;
		    yyval.decl = yyvsp[-3].decl;
		    t = NewString("");
		    SwigType_add_array(t,yyvsp[-1].dtype.val);
		    if (yyval.decl.type) {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = t;
                  ;
    break;}
case 260:
#line 3791 "parser.y"
{ 
		    yyval.decl.type = NewString("");
		    yyval.decl.id = 0;
		    yyval.decl.parms = 0;
		    yyval.decl.have_parms = 0;
		    SwigType_add_array(yyval.decl.type,(char*)"");
                  ;
    break;}
case 261:
#line 3798 "parser.y"
{ 
		    yyval.decl.type = NewString("");
		    yyval.decl.id = 0;
		    yyval.decl.parms = 0;
		    yyval.decl.have_parms = 0;
		    SwigType_add_array(yyval.decl.type,yyvsp[-1].dtype.val);
		  ;
    break;}
case 262:
#line 3805 "parser.y"
{
                    yyval.decl = yyvsp[-1].decl;
		  ;
    break;}
case 263:
#line 3808 "parser.y"
{
		    SwigType *t;
                    yyval.decl = yyvsp[-3].decl;
		    t = NewString("");
                    SwigType_add_function(t,yyvsp[-1].pl);
		    if (!yyval.decl.type) {
		      yyval.decl.type = t;
		    } else {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		      yyval.decl.type = t;
		    }
		    if (!yyval.decl.have_parms) {
		      yyval.decl.parms = yyvsp[-1].pl;
		      yyval.decl.have_parms = 1;
		    }
		  ;
    break;}
case 264:
#line 3825 "parser.y"
{
                    yyval.decl.type = NewString("");
                    SwigType_add_function(yyval.decl.type,yyvsp[-1].pl);
		    yyval.decl.parms = yyvsp[-1].pl;
		    yyval.decl.have_parms = 1;
		    yyval.decl.id = 0;
                  ;
    break;}
case 265:
#line 3835 "parser.y"
{ 
               yyval.type = NewString("");
               SwigType_add_pointer(yyval.type);
	       SwigType_push(yyval.type,yyvsp[-1].str);
	       SwigType_push(yyval.type,yyvsp[0].type);
	       Delete(yyvsp[0].type);
           ;
    break;}
case 266:
#line 3842 "parser.y"
{
	     yyval.type = NewString("");
	     SwigType_add_pointer(yyval.type);
	     SwigType_push(yyval.type,yyvsp[0].type);
	     Delete(yyvsp[0].type);
	     ;
    break;}
case 267:
#line 3848 "parser.y"
{ 
	     	yyval.type = NewString("");	
		SwigType_add_pointer(yyval.type);
	        SwigType_push(yyval.type,yyvsp[0].str);
           ;
    break;}
case 268:
#line 3853 "parser.y"
{
	      yyval.type = NewString("");
	      SwigType_add_pointer(yyval.type);
           ;
    break;}
case 269:
#line 3859 "parser.y"
{ 
                  yyval.str = NewString("");
	          SwigType_add_qualifier(yyval.str,yyvsp[0].id);
               ;
    break;}
case 270:
#line 3863 "parser.y"
{ 
                  yyval.str = yyvsp[0].str; 
                  SwigType_add_qualifier(yyval.str,yyvsp[-1].id);
               ;
    break;}
case 271:
#line 3869 "parser.y"
{ yyval.id = "const"; ;
    break;}
case 272:
#line 3870 "parser.y"
{ yyval.id = "volatile"; ;
    break;}
case 273:
#line 3876 "parser.y"
{
                   yyval.type = yyvsp[0].type;
                   Replace(yyval.type,"typename ","", DOH_REPLACE_ANY);
                ;
    break;}
case 274:
#line 3882 "parser.y"
{
                   yyval.type = yyvsp[0].type;
	           SwigType_push(yyval.type,yyvsp[-1].str);
               ;
    break;}
case 275:
#line 3886 "parser.y"
{ yyval.type = yyvsp[0].type; ;
    break;}
case 276:
#line 3889 "parser.y"
{ yyval.type = yyvsp[0].type;
                  /* Printf(stdout,"primitive = '%s'\n", $$);*/
                ;
    break;}
case 277:
#line 3892 "parser.y"
{ yyval.type = yyvsp[0].type; ;
    break;}
case 278:
#line 3893 "parser.y"
{ yyval.type = yyvsp[0].type; ;
    break;}
case 279:
#line 3894 "parser.y"
{ yyval.type = NewStringf("%s%s",yyvsp[-1].type,yyvsp[0].id); ;
    break;}
case 280:
#line 3895 "parser.y"
{ yyval.type = NewStringf("enum %s", yyvsp[0].str); ;
    break;}
case 281:
#line 3896 "parser.y"
{ yyval.type = yyvsp[0].type; ;
    break;}
case 282:
#line 3897 "parser.y"
{
		  yyval.type = yyvsp[-1].type;
	          SwigType_push(yyval.type,yyvsp[0].str);
     	       ;
    break;}
case 283:
#line 3902 "parser.y"
{
		  yyval.type = yyvsp[0].str;
               ;
    break;}
case 284:
#line 3905 "parser.y"
{ 
		 yyval.type = NewStringf("%s %s", yyvsp[-1].id, yyvsp[0].str);
               ;
    break;}
case 285:
#line 3910 "parser.y"
{
		 if (!yyvsp[0].ptype.type) yyvsp[0].ptype.type = NewString("int");
		 if (yyvsp[0].ptype.us) {
		   yyval.type = NewStringf("%s %s", yyvsp[0].ptype.us, yyvsp[0].ptype.type);
		   Delete(yyvsp[0].ptype.us);
                   Delete(yyvsp[0].ptype.type);
		 } else {
                   yyval.type = yyvsp[0].ptype.type;
		 }
		 if (Cmp(yyval.type,"signed int") == 0) {
		   Delete(yyval.type);
		   yyval.type = NewString("int");
                 } else if (Cmp(yyval.type,"signed long") == 0) {
		   Delete(yyval.type);
                   yyval.type = NewString("long");
                 } else if (Cmp(yyval.type,"signed short") == 0) {
		   Delete(yyval.type);
		   yyval.type = NewString("short");
		 } else if (Cmp(yyval.type,"signed long long") == 0) {
		   Delete(yyval.type);
		   yyval.type = NewString("long long");
		 }
               ;
    break;}
case 286:
#line 3935 "parser.y"
{ 
                 yyval.ptype = yyvsp[0].ptype;
               ;
    break;}
case 287:
#line 3938 "parser.y"
{
                    if (yyvsp[-1].ptype.us && yyvsp[0].ptype.us) {
		      Swig_error(cparse_file, cparse_line, "Extra %s specifier.\n", yyvsp[0].ptype.us);
		    }
                    yyval.ptype = yyvsp[0].ptype;
                    if (yyvsp[-1].ptype.us) yyval.ptype.us = yyvsp[-1].ptype.us;
		    if (yyvsp[-1].ptype.type) {
		      if (!yyvsp[0].ptype.type) yyval.ptype.type = yyvsp[-1].ptype.type;
		      else {
			int err = 0;
			if ((Cmp(yyvsp[-1].ptype.type,"long") == 0)) {
			  if ((Cmp(yyvsp[0].ptype.type,"long") == 0) || (Cmp(yyvsp[0].ptype.type,"double") == 0)) {
			    yyval.ptype.type = NewStringf("long %s", yyvsp[0].ptype.type);
			  } else if (Cmp(yyvsp[0].ptype.type,"int") == 0) {
			    yyval.ptype.type = yyvsp[-1].ptype.type;
			  } else {
			    err = 1;
			  }
			} else if ((Cmp(yyvsp[-1].ptype.type,"short")) == 0) {
			  if (Cmp(yyvsp[0].ptype.type,"int") == 0) {
			    yyval.ptype.type = yyvsp[-1].ptype.type;
			  } else {
			    err = 1;
			  }
			} else if (Cmp(yyvsp[-1].ptype.type,"int") == 0) {
			  yyval.ptype.type = yyvsp[0].ptype.type;
			} else if (Cmp(yyvsp[-1].ptype.type,"double") == 0) {
			  if (Cmp(yyvsp[0].ptype.type,"long") == 0) {
			    yyval.ptype.type = NewString("long double");
			  } else {
			    err = 1;
			  }
			}
			if (err) {
			  Swig_error(cparse_file, cparse_line, "Extra %s specifier.\n", yyvsp[-1].ptype.type);
			}
		      }
		    }
               ;
    break;}
case 288:
#line 3980 "parser.y"
{ 
		    yyval.ptype.type = NewString("int");
                    yyval.ptype.us = 0;
               ;
    break;}
case 289:
#line 3984 "parser.y"
{ 
                    yyval.ptype.type = NewString("short");
                    yyval.ptype.us = 0;
                ;
    break;}
case 290:
#line 3988 "parser.y"
{ 
                    yyval.ptype.type = NewString("long");
                    yyval.ptype.us = 0;
                ;
    break;}
case 291:
#line 3992 "parser.y"
{ 
                    yyval.ptype.type = NewString("char");
                    yyval.ptype.us = 0;
                ;
    break;}
case 292:
#line 3996 "parser.y"
{ 
                    yyval.ptype.type = NewString("float");
                    yyval.ptype.us = 0;
                ;
    break;}
case 293:
#line 4000 "parser.y"
{ 
                    yyval.ptype.type = NewString("double");
                    yyval.ptype.us = 0;
                ;
    break;}
case 294:
#line 4004 "parser.y"
{ 
                    yyval.ptype.us = NewString("signed");
                    yyval.ptype.type = 0;
                ;
    break;}
case 295:
#line 4008 "parser.y"
{ 
                    yyval.ptype.us = NewString("unsigned");
                    yyval.ptype.type = 0;
                ;
    break;}
case 296:
#line 4014 "parser.y"
{ /* scanner_check_typedef(); */ ;
    break;}
case 297:
#line 4014 "parser.y"
{
                   yyval.dtype = yyvsp[0].dtype;
		   yyval.dtype.rawval = 0;
		   yyval.dtype.bitfield = 0;
		   yyval.dtype.throws = 0;
		   scanner_ignore_typedef();
                ;
    break;}
case 298:
#line 4021 "parser.y"
{
                   yyval.dtype.val = NewString(yyvsp[0].id);
		   yyval.dtype.rawval = NewStringf("\"%(escape)s\"",yyval.dtype.val);
                   yyval.dtype.type = T_STRING;
		   yyval.dtype.bitfield = 0;
		   yyval.dtype.throws = 0;
		;
    break;}
case 299:
#line 4028 "parser.y"
{
                   yyval.dtype.val = NewString(yyvsp[0].str);
		   if (Len(yyval.dtype.val)) {
		     yyval.dtype.rawval = NewStringf("\'%(escape)s\'",yyval.dtype.val);
		   } else {
		     yyval.dtype.rawval = NewString("\'\\0'");
		   }
		   yyval.dtype.type = T_CHAR;
		   yyval.dtype.bitfield = 0;
		   yyval.dtype.throws;
		 ;
    break;}
case 300:
#line 4043 "parser.y"
{ yyval.id = yyvsp[0].id; ;
    break;}
case 301:
#line 4044 "parser.y"
{ yyval.id = (char *) 0;;
    break;}
case 302:
#line 4049 "parser.y"
{ 
                   Node *n = Getattr(yyvsp[-2].node,"_last");
		   if (!n) {
		     set_nextSibling(yyvsp[-2].node,yyvsp[0].node);
		     Setattr(yyvsp[-2].node,"_last",yyvsp[0].node);
		   } else {
		     set_nextSibling(n,yyvsp[0].node);
		     Setattr(yyvsp[-2].node,"_last",yyvsp[0].node);
		   }
		   yyval.node = yyvsp[-2].node;
               ;
    break;}
case 303:
#line 4060 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 304:
#line 4063 "parser.y"
{
		   yyval.node = new_node("enumitem");
		   Setattr(yyval.node,"name",yyvsp[0].id);
		   Setattr(yyval.node,"type",NewSwigType(T_INT));
		   Setattr(yyval.node,"feature:immutable","1");
		 ;
    break;}
case 305:
#line 4069 "parser.y"
{
		   yyval.node = new_node("enumitem");
		   Setattr(yyval.node,"name",yyvsp[-2].id);
		   Setattr(yyval.node,"enumvalue", yyvsp[0].dtype.val);
	           if (yyvsp[0].dtype.type == T_CHAR) {
		     Setattr(yyval.node,"value",yyvsp[0].dtype.val);
		     Setattr(yyval.node,"type",NewSwigType(T_CHAR));
		   } else {
		     Setattr(yyval.node,"value",yyvsp[-2].id);
		     Setattr(yyval.node,"type",NewSwigType(T_INT));
		   }
		   Setattr(yyval.node,"feature:immutable","1");
                 ;
    break;}
case 306:
#line 4082 "parser.y"
{ yyval.node = 0; ;
    break;}
case 307:
#line 4085 "parser.y"
{
                   yyval.dtype = yyvsp[0].dtype;
		   if ((yyval.dtype.type != T_INT) && (yyval.dtype.type != T_UINT) &&
		       (yyval.dtype.type != T_LONG) && (yyval.dtype.type != T_ULONG) &&
		       (yyval.dtype.type != T_SHORT) && (yyval.dtype.type != T_USHORT) &&
		       (yyval.dtype.type != T_SCHAR) && (yyval.dtype.type != T_UCHAR)) {
		     Swig_error(cparse_file,cparse_line,"Type error. Expecting an int\n");
		   }
                ;
    break;}
case 308:
#line 4094 "parser.y"
{
                   yyval.dtype.val  = NewString(yyvsp[0].str);
		   yyval.dtype.type = T_INT;
		 ;
    break;}
case 309:
#line 4105 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 310:
#line 4106 "parser.y"
{
  		  SwigType_push(yyvsp[-2].type,yyvsp[-1].decl.type);
		  yyval.dtype.val = NewStringf("sizeof(%s)",SwigType_str(yyvsp[-2].type,0));
		  yyval.dtype.type = T_INT;
               ;
    break;}
case 311:
#line 4111 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 312:
#line 4112 "parser.y"
{
		 Node *n;
		 yyval.dtype.val = yyvsp[0].type;
		 yyval.dtype.type = T_INT;
		 /* Check if value is in scope */
		 n = Swig_symbol_clookup(yyvsp[0].type,0);
		 if (n) {
		   if (Getattr(n,"access")) {
		     if (cplus_mode == CPLUS_PUBLIC) {
		       Swig_warning(WARN_PARSE_PRIVATE,cparse_file, cparse_line, "'%s' is private in this context.\n", yyvsp[0].type);
		       yyval.dtype.type = T_ERROR;
		     }

		   }
		 }
               ;
    break;}
case 313:
#line 4130 "parser.y"
{
   	            yyval.dtype.val = NewStringf("(%s)",yyvsp[-1].dtype.val);
		    yyval.dtype.type = yyvsp[-1].dtype.type;
   	       ;
    break;}
case 314:
#line 4137 "parser.y"
{
                 yyval.dtype = yyvsp[0].dtype;
		 yyval.dtype.val = NewStringf("(%s) %s", SwigType_str(yyvsp[-2].dtype.val,0), yyvsp[0].dtype.val);
 	       ;
    break;}
case 315:
#line 4141 "parser.y"
{
                 yyval.dtype = yyvsp[0].dtype;
		 SwigType_push(yyvsp[-3].dtype.val,yyvsp[-2].type);
		 yyval.dtype.val = NewStringf("(%s) %s", SwigType_str(yyvsp[-3].dtype.val,0), yyvsp[0].dtype.val);
 	       ;
    break;}
case 316:
#line 4146 "parser.y"
{
                 yyval.dtype = yyvsp[0].dtype;
		 SwigType_add_reference(yyvsp[-3].dtype.val);
		 yyval.dtype.val = NewStringf("(%s) %s", SwigType_str(yyvsp[-3].dtype.val,0), yyvsp[0].dtype.val);
 	       ;
    break;}
case 317:
#line 4151 "parser.y"
{
                 yyval.dtype = yyvsp[0].dtype;
		 SwigType_push(yyvsp[-4].dtype.val,yyvsp[-3].type);
		 SwigType_add_reference(yyvsp[-4].dtype.val);
		 yyval.dtype.val = NewStringf("(%s) %s", SwigType_str(yyvsp[-4].dtype.val,0), yyvsp[0].dtype.val);
 	       ;
    break;}
case 318:
#line 4159 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 319:
#line 4160 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 320:
#line 4161 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 321:
#line 4162 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 322:
#line 4163 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 323:
#line 4164 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 324:
#line 4165 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 325:
#line 4168 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s+%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 326:
#line 4172 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s-%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 327:
#line 4176 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s*%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 328:
#line 4180 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s/%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 329:
#line 4184 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s&%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 330:
#line 4188 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s|%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 331:
#line 4192 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s^%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 332:
#line 4196 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s<<%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 333:
#line 4200 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s>>%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 334:
#line 4204 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s&&%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = T_INT;
	       ;
    break;}
case 335:
#line 4208 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s||%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = T_INT;
	       ;
    break;}
case 336:
#line 4212 "parser.y"
{
		 yyval.dtype.val = NewStringf("-%s",yyvsp[0].dtype.val);
		 yyval.dtype.type = yyvsp[0].dtype.type;
	       ;
    break;}
case 337:
#line 4216 "parser.y"
{
		 yyval.dtype.val = NewStringf("~%s",yyvsp[0].dtype.val);
		 yyval.dtype.type = yyvsp[0].dtype.type;
	       ;
    break;}
case 338:
#line 4220 "parser.y"
{
                 yyval.dtype.val = NewStringf("!%s",yyvsp[0].dtype.val);
		 yyval.dtype.type = T_INT;
	       ;
    break;}
case 339:
#line 4224 "parser.y"
{
                 skip_balanced('(',')');
		 if (SwigType_istemplate(yyvsp[-1].type)) {
		   yyvsp[-1].type = SwigType_namestr(yyvsp[-1].type);
		 }
		 yyval.dtype.val = NewStringf("%s%s",yyvsp[-1].type,scanner_ccode);
		 Clear(scanner_ccode);
		 yyval.dtype.type = T_INT;
               ;
    break;}
case 340:
#line 4235 "parser.y"
{
		 yyval.bases = yyvsp[0].bases;
               ;
    break;}
case 341:
#line 4240 "parser.y"
{ inherit_list = 1; ;
    break;}
case 342:
#line 4240 "parser.y"
{ yyval.bases = yyvsp[0].bases; inherit_list = 0; ;
    break;}
case 343:
#line 4241 "parser.y"
{ yyval.bases = 0; ;
    break;}
case 344:
#line 4244 "parser.y"
{
	           yyval.bases = NewList();
	           if (yyvsp[0].node) Append(yyval.bases,yyvsp[0].node);
               ;
    break;}
case 345:
#line 4249 "parser.y"
{
                   yyval.bases = yyvsp[-2].bases;
                   if (yyvsp[0].node) Append(yyval.bases,yyvsp[0].node);
               ;
    break;}
case 346:
#line 4255 "parser.y"
{
                 if (last_cpptype && (Strcmp(last_cpptype,"struct") != 0)) {
                     Swig_warning(WARN_PARSE_NO_ACCESS,cparse_file, cparse_line,"No access specifier given for base class %s (ignored).\n",yyvsp[0].str);
   		     yyval.node = (char *) 0;
                 } else {
		   yyval.node = yyvsp[0].str;
		   Setfile(yyval.node,cparse_file);
		   Setline(yyval.node,cparse_line);
		 }
               ;
    break;}
case 347:
#line 4265 "parser.y"
{
		 yyval.node = 0;
	         if (strcmp(yyvsp[-2].id,"public") == 0) {
		   yyval.node = yyvsp[0].str;
		   Setfile(yyval.node, cparse_file);
		   Setline(yyval.node, cparse_line);
		 } else {
		   Swig_warning(WARN_PARSE_PRIVATE_INHERIT, cparse_file, cparse_line, "%s inheritance ignored.\n", yyvsp[-2].id);
		 }
               ;
    break;}
case 348:
#line 4277 "parser.y"
{ yyval.id = (char*)"public"; ;
    break;}
case 349:
#line 4278 "parser.y"
{ yyval.id = (char*)"private"; ;
    break;}
case 350:
#line 4279 "parser.y"
{ yyval.id = (char*)"protected"; ;
    break;}
case 351:
#line 4283 "parser.y"
{ 
                   yyval.id = (char*)"class"; 
		   if (!inherit_list) last_cpptype = yyval.id;
               ;
    break;}
case 352:
#line 4287 "parser.y"
{ 
                   yyval.id = (char*)"struct"; 
		   if (!inherit_list) last_cpptype = yyval.id;
               ;
    break;}
case 353:
#line 4291 "parser.y"
{
                   yyval.id = (char*)"union"; 
		   if (!inherit_list) last_cpptype = yyval.id;
               ;
    break;}
case 354:
#line 4295 "parser.y"
{ 
                   yyval.id = (char *)"typename"; 
		   if (!inherit_list) last_cpptype = yyval.id;
               ;
    break;}
case 357:
#line 4305 "parser.y"
{
                    yyval.dtype.qualifier = yyvsp[0].str;
                    yyval.dtype.throws = 0;
               ;
    break;}
case 358:
#line 4309 "parser.y"
{
                    yyval.dtype.qualifier = 0;
                    yyval.dtype.throws = yyvsp[-1].pl;
               ;
    break;}
case 359:
#line 4313 "parser.y"
{
                    yyval.dtype.qualifier = yyvsp[-4].str;
                    yyval.dtype.throws = yyvsp[-1].pl;
               ;
    break;}
case 360:
#line 4317 "parser.y"
{ 
                    yyval.dtype.qualifier = 0; 
                    yyval.dtype.throws = 0;
               ;
    break;}
case 361:
#line 4323 "parser.y"
{ 
                    Clear(scanner_ccode); 
                    yyval.decl.have_parms = 0; 
                    yyval.decl.defarg = 0; 
		    yyval.decl.throws = yyvsp[-2].dtype.throws;
               ;
    break;}
case 362:
#line 4329 "parser.y"
{ 
                    skip_balanced('{','}'); 
                    yyval.decl.have_parms = 0; 
                    yyval.decl.defarg = 0; 
                    yyval.decl.throws = yyvsp[-2].dtype.throws;
               ;
    break;}
case 363:
#line 4335 "parser.y"
{ 
                    Clear(scanner_ccode); 
                    yyval.decl.parms = yyvsp[-2].pl; 
                    yyval.decl.have_parms = 1; 
                    yyval.decl.defarg = 0; 
		    yyval.decl.throws = 0;
               ;
    break;}
case 364:
#line 4342 "parser.y"
{
                    skip_balanced('{','}'); 
                    yyval.decl.parms = yyvsp[-2].pl; 
                    yyval.decl.have_parms = 1; 
                    yyval.decl.defarg = 0; 
                    yyval.decl.throws = 0;
               ;
    break;}
case 365:
#line 4349 "parser.y"
{ 
                    yyval.decl.have_parms = 0; 
                    yyval.decl.defarg = yyvsp[-1].dtype.val; 
                    yyval.decl.throws = 0;
               ;
    break;}
case 370:
#line 4364 "parser.y"
{
	            skip_balanced('(',')');
                    Clear(scanner_ccode);
            	;
    break;}
case 371:
#line 4370 "parser.y"
{ 
                     String *s = NewString("");
                     SwigType_add_template(s,yyvsp[-1].p);
                     yyval.id = Char(s);
                 ;
    break;}
case 372:
#line 4375 "parser.y"
{ yyval.id = (char*)"";  ;
    break;}
case 373:
#line 4378 "parser.y"
{ yyval.id = yyvsp[0].id; ;
    break;}
case 374:
#line 4379 "parser.y"
{ yyval.id = yyvsp[0].id; ;
    break;}
case 375:
#line 4382 "parser.y"
{ yyval.id = yyvsp[0].id; ;
    break;}
case 376:
#line 4383 "parser.y"
{ yyval.id = 0; ;
    break;}
case 377:
#line 4386 "parser.y"
{ 
                  yyval.str = 0;
		  if (!yyval.str) yyval.str = NewStringf("%s%s", yyvsp[-1].str,yyvsp[0].str);
      	          Delete(yyvsp[0].str);
               ;
    break;}
case 378:
#line 4391 "parser.y"
{ 
		 yyval.str = NewStringf("::%s%s",yyvsp[-1].str,yyvsp[0].str);
                 Delete(yyvsp[0].str);
               ;
    break;}
case 379:
#line 4395 "parser.y"
{
		 yyval.str = NewString(yyvsp[0].str);
   	       ;
    break;}
case 380:
#line 4398 "parser.y"
{
		 yyval.str = NewStringf("::%s",yyvsp[0].str);
               ;
    break;}
case 381:
#line 4401 "parser.y"
{
                 yyval.str = NewString(yyvsp[0].str);
	       ;
    break;}
case 382:
#line 4404 "parser.y"
{
                 yyval.str = NewStringf("::%s",yyvsp[0].str);
               ;
    break;}
case 383:
#line 4409 "parser.y"
{
                   yyval.str = NewStringf("::%s%s",yyvsp[-1].str,yyvsp[0].str);
		   Delete(yyvsp[0].str);
               ;
    break;}
case 384:
#line 4413 "parser.y"
{
                   yyval.str = NewStringf("::%s",yyvsp[0].str);
               ;
    break;}
case 385:
#line 4416 "parser.y"
{
                   yyval.str = NewStringf("::%s",yyvsp[0].str);
               ;
    break;}
case 386:
#line 4419 "parser.y"
{
		 yyval.str = NewStringf("::~%s",yyvsp[0].str);
               ;
    break;}
case 387:
#line 4425 "parser.y"
{
                  yyval.str = NewStringf("%s%s",yyvsp[-1].id,yyvsp[0].id);
                  scanner_last_id(1);
              ;
    break;}
case 388:
#line 4432 "parser.y"
{ 
                  yyval.str = 0;
		  if (!yyval.str) yyval.str = NewStringf("%s%s", yyvsp[-1].id,yyvsp[0].str);
      	          Delete(yyvsp[0].str);
               ;
    break;}
case 389:
#line 4437 "parser.y"
{ 
		 yyval.str = NewStringf("::%s%s",yyvsp[-1].id,yyvsp[0].str);
                 Delete(yyvsp[0].str);
               ;
    break;}
case 390:
#line 4441 "parser.y"
{
		 yyval.str = NewString(yyvsp[0].id);
   	       ;
    break;}
case 391:
#line 4444 "parser.y"
{
		 yyval.str = NewStringf("::%s",yyvsp[0].id);
               ;
    break;}
case 392:
#line 4447 "parser.y"
{
                 yyval.str = NewString(yyvsp[0].str);
	       ;
    break;}
case 393:
#line 4450 "parser.y"
{
                 yyval.str = NewStringf("::%s",yyvsp[0].str);
               ;
    break;}
case 394:
#line 4455 "parser.y"
{
                   yyval.str = NewStringf("::%s%s",yyvsp[-1].id,yyvsp[0].str);
		   Delete(yyvsp[0].str);
               ;
    break;}
case 395:
#line 4459 "parser.y"
{
                   yyval.str = NewStringf("::%s",yyvsp[0].id);
               ;
    break;}
case 396:
#line 4462 "parser.y"
{
                   yyval.str = NewStringf("::%s",yyvsp[0].str);
               ;
    break;}
case 397:
#line 4465 "parser.y"
{
		 yyval.str = NewStringf("::~%s",yyvsp[0].id);
               ;
    break;}
case 398:
#line 4471 "parser.y"
{ 
                   yyval.id = (char *) malloc(strlen(yyvsp[-1].id)+strlen(yyvsp[0].id)+1);
                   strcpy(yyval.id,yyvsp[-1].id);
                   strcat(yyval.id,yyvsp[0].id);
               ;
    break;}
case 399:
#line 4476 "parser.y"
{ yyval.id = yyvsp[0].id;;
    break;}
case 400:
#line 4479 "parser.y"
{
		 yyval.str = NewString(yyvsp[0].id);
               ;
    break;}
case 401:
#line 4482 "parser.y"
{
                  skip_balanced('{','}');
		  yyval.str = NewString(scanner_ccode);
               ;
    break;}
case 402:
#line 4486 "parser.y"
{
		 yyval.str = yyvsp[0].str;
              ;
    break;}
case 403:
#line 4491 "parser.y"
{
                  Hash *n;
                  yyval.node = NewHash();
                  n = yyvsp[-1].node;
                  while(n) {
                     String *name, *value;
                     name = Getattr(n,"name");
                     value = Getattr(n,"value");
		     if (!value) value = (String *) "1";
                     Setattr(yyval.node,name, value);
		     n = nextSibling(n);
		  }
               ;
    break;}
case 404:
#line 4504 "parser.y"
{ yyval.node = 0; ;
    break;}
case 405:
#line 4508 "parser.y"
{
		 yyval.node = NewHash();
		 Setattr(yyval.node,"name",yyvsp[-2].id);
		 Setattr(yyval.node,"value",yyvsp[0].id);
               ;
    break;}
case 406:
#line 4513 "parser.y"
{
		 yyval.node = NewHash();
		 Setattr(yyval.node,"name",yyvsp[-4].id);
		 Setattr(yyval.node,"value",yyvsp[-2].id);
		 set_nextSibling(yyval.node,yyvsp[0].node);
               ;
    break;}
case 407:
#line 4519 "parser.y"
{
                 yyval.node = NewHash();
                 Setattr(yyval.node,"name",yyvsp[0].id);
	       ;
    break;}
case 408:
#line 4523 "parser.y"
{
                 yyval.node = NewHash();
                 Setattr(yyval.node,"name",yyvsp[-2].id);
                 set_nextSibling(yyval.node,yyvsp[0].node);
               ;
    break;}
case 409:
#line 4530 "parser.y"
{
		 yyval.id = yyvsp[0].id;
               ;
    break;}
case 410:
#line 4533 "parser.y"
{
                 yyval.id = yyvsp[0].dtype.val;
               ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "/usr/local/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 4540 "parser.y"


/* Called by the parser (yyparse) when an error is found.*/
void yyerror (const char *e) {
}

SwigType *Swig_cparse_type(String *s) {
   String *ns;
   extern void scanner_file(File *);
   extern int yyparse();
   extern void scanner_next_token(int);
   ns = NewStringf("%s;",s);
   Seek(ns,0,SEEK_SET);
   scanner_file(ns);
   top = 0;
   scanner_next_token(PARSETYPE);
   yyparse();
   /*   Printf(stdout,"typeparse: '%s' ---> '%s'\n", s, top); */
   return top;
}









