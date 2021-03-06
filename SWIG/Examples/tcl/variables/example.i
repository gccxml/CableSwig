/* File : example.i */
%module example
%{
#include "example.h"
%}

/* Some global variable declarations */
extern int              ivar;
extern short            svar;
extern long             lvar;
extern unsigned int     uivar;
extern unsigned short   usvar;
extern unsigned long    ulvar;
extern signed char      scvar;
extern unsigned char    ucvar;
extern char             cvar;
extern float            fvar;
extern double           dvar;
extern char            *strvar;
extern const char      *cstrvar;
extern int             *iptrvar;
extern char             name[256];

extern Point           *ptptr;
extern Point            pt;


/* Some read-only variables */

%immutable;
extern int  status;
extern char path[256];
%mutable;

/* Some helper functions to make it easier to test */
extern void  print_vars();
extern int  *new_int(int value);
extern Point *new_Point(int x, int y);
extern char  *Point_print(Point *p);
extern void  pt_print();




