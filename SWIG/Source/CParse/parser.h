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


extern YYSTYPE yylval;
