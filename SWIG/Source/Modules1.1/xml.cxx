/* ----------------------------------------------------------------------------- 
 * Xml.cxx
 *
 *     A web-base parse tree Xml using SWILL.   This is an optional
 *     feature that's normally disabled.
 * 
 * Author(s) : David Beazley (beazley@cs.uchicago.edu)
 *
 * Copyright (C) 2002.  The University of Chicago
 * See the file LICENSE for information on usage and redistribution.	
 * ----------------------------------------------------------------------------- */

char cvsroot_xml_cxx[] = "Header";
static const char *usage = "\
XML Options (available with -xml)\n\
     -xml output.xml - Use output.xml as output file (extension .xml mandatory)\n\
     -xmllang lang   - Typedef language.\n\n";

#include "swigmod.h"

//static Node *view_top = 0;
static File *out = 0;
      
class XML 
: public Language 
{



public:

	int indent_level;
	long id;
	XML()
	: indent_level( 0 )
	, id( 0 )
	{
	}

	virtual ~XML()
	{
	}

	virtual void main(int argc, char *argv[])
	{
	    SWIG_typemap_lang("xml");
		for( int iX = 0; iX < argc; iX++ )
		{
			if( strcmp( argv[iX], "-xml" ) == 0 )
			{
				char * extension = argv[iX+1]+strlen(argv[iX+1])-4;
				if( strcmp( extension, ".xml" ) )
					continue;
				iX++;
				Swig_mark_arg (iX);
				String * outfile = NewString( argv[iX] );
				out = NewFile(outfile,"w");
				if (!out) 
				{
					Printf(stderr,"*** Can't open '%s'\n", outfile);
					SWIG_exit(EXIT_FAILURE);
				}
				continue;
			}
			if( strcmp( argv[iX], "-xmllang" ) == 0 )
			{
				Swig_mark_arg (iX);
				iX++;
			    SWIG_typemap_lang(argv[iX]);
				Swig_mark_arg (iX);
				continue;
			}
			if( strcmp( argv[iX], "-help" ) == 0 )
			{
				fputs( usage, stderr );
			}
		}
	}	

  /* Top of the parse tree */

	virtual int top(Node *n) 
	{
		if( out == 0 )
		{
			String *outfile = Getattr(n,"outfile");
			Replaceall(outfile,"_wrap.cxx", ".xml");
			out = NewFile(outfile,"w");
			if (!out) 
			{
				Printf(stderr,"*** Can't open '%s'\n", outfile);
				SWIG_exit(EXIT_FAILURE);
			}
		}
		Printf( out, "<?xml version=\"1.0\" ?> \n" );
		Xml_print_tree(n);
    	return SWIG_OK;
  	}



	void print_indent(int l) 
	{
		int i;
		for (i = 0; i < indent_level; i++) 
		{
			Printf(out, " ");
		}
		if (l) 
		{
			Printf(out, " ");
		}
	}

	void Xml_print_tree(DOH *obj) 
	{
		while (obj) 
		{
			Xml_print_node(obj);
			obj = nextSibling(obj);
		}
	}

	void Xml_print_attributes(Node * obj)
	{
		String *k;
		indent_level += 4;
		print_indent(0);
		Printf( out, "<attributelist id=\"%ld\" addr=\"%x\" >\n", ++id, obj );
		indent_level += 4;		

		k = Firstkey(obj);
		while (k) 
		{
			if ((Cmp(k,"nodeType") == 0) 
			|| (Cmp(k,"firstChild") == 0) 
			|| (Cmp(k,"lastChild") == 0) 
			|| (Cmp(k,"parentNode") == 0) 
			|| (Cmp(k,"nextSibling") == 0) 
			|| (Cmp(k,"previousSibling") == 0) 
			|| (*(Char(k)) == '$')) 
			{
				/* Do nothing */
			} 
			else if (Cmp(k,"module") == 0) 
			{
				Xml_print_module( Getattr(obj,k) );
			} 
			else if (Cmp(k,"baselist") == 0) 
			{
				Xml_print_baselist( Getattr(obj,k) );
			} 
			else if (Cmp(k,"typescope") == 0) 
			{
				Xml_print_typescope( Getattr(obj,k) );
			} 
			else if (Cmp(k,"typetab") == 0) 
			{
				Xml_print_typetab( Getattr(obj,k) );
			} 
			else if (Cmp(k,"kwargs") == 0) 
			{
				Xml_print_kwargs( Getattr(obj,k) );
			} 
			else if (Cmp(k,"parms") == 0 || Cmp(k, "pattern") == 0 ) 
			{
				Xml_print_parmlist( Getattr(obj,k) );
			} 
			else 
			{
				DOH *o;
				print_indent(0);
				if (DohIsString(Getattr(obj,k))) 
				{
					o = Str(Getattr(obj,k));
					Replaceall( k, ":", "_" );
					Replaceall( o, "<", "&lt;" );
					Replaceall( o, "&", "&amp;" );
					Replaceall( o, "\"", "&quot;" );
					Replaceall( o, "\\", "\\\\" );
					Printf(out,"<attribute name=\"%s\" value=\"%s\" id=\"%ld\" addr=\"%x\" />\n", k, o, ++id, o );
					Delete(o);
				} 
				else 
				{
					o = Getattr(obj,k);
					Replaceall( k, ":", "_" );
					Printf(out,"<attribute name=\"%s\" value=\"%x\" id=\"%ld\" addr=\"%x\" />\n", k, o, ++id, o );
				}
			}
			k = Nextkey(obj);
		}
		indent_level -= 4;
		print_indent(0);
		Printf( out, "</attributelist >\n" );
		indent_level -= 4;
	}

	void Xml_print_node(Node *obj) 
	{
		Node   *cobj;
  
		print_indent(0);
		Printf(out,"<%s id=\"%ld\" addr=\"%x\" >\n", nodeType(obj), ++id, obj);
		Xml_print_attributes( obj );
		cobj = firstChild(obj);
		if (cobj) 
		{
			indent_level += 4;
			Printf(out,"\n");
			Xml_print_tree(cobj);
			indent_level -= 4;
		} 
		else 
		{
			print_indent(1);
			Printf(out,"\n");
		}
		print_indent(0);
		Printf(out,"</%s >\n", nodeType(obj));
	}


	void Xml_print_parmlist(ParmList *p) 
	{

		print_indent(0);
		Printf( out, "<parmlist id=\"%ld\" addr=\"%x\" >\n", ++id, p );
		indent_level += 4;		
		while(p) 
		{
			print_indent(0);
			Printf( out, "<parm id=\"%ld\">\n", ++id );
			Xml_print_attributes( p );
			print_indent(0);
			Printf( out, "</parm >\n" );
			p = nextSibling(p);
		}
		indent_level -= 4;		
		print_indent(0);
		Printf( out, "</parmlist >\n" );
	}

	void Xml_print_baselist(List *p) 
	{

		print_indent(0);
		Printf( out, "<baselist id=\"%ld\" addr=\"%x\" >\n", ++id, p );
		indent_level += 4;		
		String *s;
		for (s = Firstitem(p); s; s = Nextitem(p)) 
		{
			print_indent(0);
			Printf( out, "<base name=\"%s\" id=\"%ld\" addr=\"%x\" />\n", s, ++id, s );
		}
		indent_level -= 4;		
		print_indent(0);
		Printf( out, "</baselist >\n" );
	}

	void Xml_print_module(Node *p) 
	{

		print_indent(0);
		Printf( out, "<attribute name=\"module\" value=\"%s\" id=\"%ld\" addr=\"%x\" />\n", Getattr( p, "name"), ++id, p );
	}

	void Xml_print_kwargs(Hash *p) 
	{
		Xml_print_hash( p, "kwargs" );
	}

	void Xml_print_typescope(Hash *p) 
	{

		Xml_print_hash( p, "typescope" );
	}

	void Xml_print_typetab(Hash *p) 
	{

		Xml_print_hash( p, "typetab" );
	}


	void Xml_print_hash(Hash *p, const char * markup) 
	{

		print_indent(0);
		Printf( out, "<%s id=\"%ld\" addr=\"%x\" >\n", markup, ++id, p );
		Xml_print_attributes( p );
		indent_level += 4;		
		Node * n = Firstitem( p );
		while(n) 
		{
			print_indent(0);
			Printf( out, "<%ssitem id=\"%ld\" addr=\"%x\" >\n", markup, ++id, n );
			Xml_print_attributes( n );
			Printf( out, "</%ssitem >\n", markup );
			print_indent(0);
			Printf( out, " />\n" );
			n = Nextkey(p);
		}
		indent_level -= 4;		
		print_indent(0);
		Printf( out, "</%s >\n", markup );
	}

};


extern "C"
{
	Language * swig_xml( void )
	{
		return new XML();
	}
}