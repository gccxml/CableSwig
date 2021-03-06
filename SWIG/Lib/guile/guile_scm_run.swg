/* -*- c -*-
 * -----------------------------------------------------------------------
 * swig_lib/guile/guile_scm_run.swg
 *
 * Author: John Lenz <jelenz@wisc.edu>
 * ----------------------------------------------------------------------- */

#include <libguile.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef SCM (*swig_guile_proc)();
typedef SCM (*guile_destructor)(SCM);

typedef struct swig_guile_clientdata {
  guile_destructor destroy;
  SCM goops_class;
} swig_guile_clientdata;

#define SWIG_scm2str(s) \
  SWIG_Guile_scm2newstr(s, NULL)
#define SWIG_malloc(size) \
  SCM_MUST_MALLOC(size)
#define SWIG_free(mem) \
  scm_must_free(mem)
#define SWIG_ConvertPtr(s, result, type, flags) \
  SWIG_Guile_ConvertPtr(s, result, type, flags)
#define SWIG_MustGetPtr(s, type, argnum, flags) \
  SWIG_Guile_MustGetPtr(s, type, argnum, flags, FUNC_NAME)
#define SWIG_NewPointerObj(ptr, type, owner) \
  SWIG_Guile_NewPointerObj((void*)ptr, type, owner)
#define SWIG_PropagateClientData(type) \
  SWIG_Guile_PropagateClientData(type)
#define SWIG_contract_assert(expr, msg)				\
  if (!(expr))							\
    scm_error(scm_str2symbol("swig-contract-assertion-failed"),	\
	      (char *) FUNC_NAME, (char *) msg,			\
	      SCM_EOL, SCM_BOOL_F); else
  
#ifdef SWIG_NOINCLUDE

/* Interface helper function */
SWIGIMPORT(char *) SWIG_Guile_scm2newstr(SCM str, size_t *len);

/* Register SWIG smobs with Guile.  */
SWIGIMPORT(void) SWIG_Guile_Init();
/* Get a pointer value from a smob.  If there is a type-mismatch,
   return nonzero; on success, return 0.  */
SWIGIMPORT(int) SWIG_Guile_ConvertPtr(SCM s, void **result, swig_type_info *type, int flags);
/* Get a pointer value from a smob.  If there is a type-mismatch,
   signal a wrong-type-arg error for the given argument number. */
SWIGIMPORT(void *) SWIG_Guile_MustGetPtr(SCM s, swig_type_info *type, int argnum, int flags, const char *func_name);
/* Make a smob from a pointer and typeinfo.  */
SWIGIMPORT(SCM) SWIG_Guile_NewPointerObj(void *ptr, swig_type_info *type, int owner);
/* Get arguments from an argument list */
SWIGIMPORT(int) SWIG_Guile_GetArgs(SCM *dest, SCM rest, int reqargs, int optargs, const char *procname);
/* Propagate client data to equivalent types */
SWIGIMPORT(void) 
SWIG_Guile_PropagateClientData(swig_type_info *type);
/* Make a pointer object non-collectable */
SWIGIMPORT(void)
SWIG_Guile_MarkPointerNoncollectable(SCM s);
/* Mark a pointer object destroyed */
SWIGIMPORT(void)
SWIG_Guile_MarkPointerDestroyed(SCM s);
       
#else

SWIGRUNTIME(char *) 
SWIG_Guile_scm2newstr(SCM str, size_t *len) {
#define FUNC_NAME "SWIG_Guile_scm2newstr"
  char *ret;
  size_t l;

  l = SCM_STRING_LENGTH(str);
  ret = (char *) SWIG_malloc( (l + 1) * sizeof(char));
  if (!ret) return NULL;

  memcpy(ret, SCM_STRING_CHARS(str), l);
  ret[l] = '\0';
  if (len) *len = l;
  return ret;
#undef FUNC_NAME
}

static scm_t_bits swig_tag = 0;
static scm_t_bits swig_collectable_tag = 0;
static scm_t_bits swig_destroyed_tag = 0;
static SCM swig_make_func = SCM_EOL;
static SCM swig_keyword = SCM_EOL;
static SCM swig_symbol = SCM_EOL;

#define SWIG_Guile_GetSmob(x) \
  ( SCM_NNULLP(x) && SCM_INSTANCEP(x) && SCM_NFALSEP(scm_slot_exists_p(x, swig_symbol)) \
      ? scm_slot_ref(x, swig_symbol) : (x) )

SWIGRUNTIME(SCM)
SWIG_Guile_NewPointerObj(void *ptr, swig_type_info *type, int owner)
{
  if (ptr == NULL)
    return SCM_EOL;
  else {
    SCM smob;
    swig_guile_clientdata *cdata = (swig_guile_clientdata *) type->clientdata;
    if (owner)
      SCM_NEWSMOB2(smob, swig_collectable_tag, ptr, (void *) type);
    else
      SCM_NEWSMOB2(smob, swig_tag, ptr, (void *) type);

    if (!cdata || SCM_NULLP(cdata->goops_class) || swig_make_func == SCM_EOL ) {
      return smob;
    } else {
      /* the scm_make() C function only handles the creation of gf,
	 methods and classes (no instances) the (make ...) function is
	 later redefined in goops.scm.  So we need to call that
	 Scheme function. */
      return scm_apply(swig_make_func,
		       scm_list_3(cdata->goops_class,
				  swig_keyword,
				  smob),
		       SCM_EOL);
    }
  }
}

/* Return 0 if successful. */
SWIGRUNTIME(int)
SWIG_Guile_ConvertPtr(SCM s, void **result, swig_type_info *type, int flags)
{
  swig_type_info *cast;
  swig_type_info *from;
  SCM smob = SWIG_Guile_GetSmob(s);

  if (SCM_NULLP(smob)) {
    *result = NULL;
    return 0;
  } else if (SCM_SMOB_PREDICATE(swig_tag, smob) || SCM_SMOB_PREDICATE(swig_collectable_tag, smob)) {
    /* we do not accept smobs representing destroyed pointers */
    from = (swig_type_info *) SCM_CELL_WORD_2(smob);
    if (!from) return 1;
    if (type) {
      cast = SWIG_TypeCheck((char*)from->name, type);
      if (cast) {
        *result = SWIG_TypeCast(cast, (void *) SCM_CELL_WORD_1(smob));
        return 0;
      } else {
        return 1;
      }
    } else {
      *result = (void *) SCM_CELL_WORD_1(smob);
      return 0;
    }
  }
  return 1;
}

SWIGRUNTIME(void *)
SWIG_Guile_MustGetPtr (SCM s, swig_type_info *type,
		       int argnum, int flags, const char *func_name)
{
  void *result;
  if (SWIG_Guile_ConvertPtr(s, &result, type, flags)) {
    /* type mismatch */
    scm_wrong_type_arg((char *) func_name, argnum, s);
  }
  return result;
}

/* Mark a pointer object non-collectable */
SWIGRUNTIME(void)
SWIG_Guile_MarkPointerNoncollectable(SCM s)
{
  SCM smob = SWIG_Guile_GetSmob(s);
  if (!SCM_NULLP(smob)) {
    if (SCM_SMOB_PREDICATE(swig_tag, smob) || SCM_SMOB_PREDICATE(swig_collectable_tag, smob)) {
      SCM_SET_CELL_TYPE(smob, swig_tag);
    }
    else scm_wrong_type_arg(NULL, 0, s);
  }
}

/* Mark a pointer object destroyed */
SWIGIMPORT(void)
SWIG_Guile_MarkPointerDestroyed(SCM s)
{
  SCM smob = SWIG_Guile_GetSmob(s);
  if (!SCM_NULLP(smob)) {
    if (SCM_SMOB_PREDICATE(swig_tag, smob) || SCM_SMOB_PREDICATE(swig_collectable_tag, smob)) {
      SCM_SET_CELL_TYPE(smob, swig_destroyed_tag);
    }
    else scm_wrong_type_arg(NULL, 0, s);
  }
}

/* Init */

static int
print_swig_aux (SCM swig_smob, SCM port, scm_print_state *pstate, const char *attribute)
{
  swig_type_info *type;
  
  type = (swig_type_info *) SCM_CELL_WORD_2(swig_smob);
  if (type) {
    scm_puts((char *) "#<", port);
    scm_puts(attribute, port);
    scm_puts("swig-pointer ", port);
    if (type->str != NULL)
      scm_puts(type->str, port);
    else 
      scm_puts(type->name, port);
    scm_puts((char *) " ", port);
    scm_intprint((long) SCM_CELL_WORD_1(swig_smob), 16, port);
    scm_puts((char *) ">", port);
    /* non-zero means success */
    return 1;
  } else {
    return 0;
  }
}

  
static int
print_swig (SCM swig_smob, SCM port, scm_print_state *pstate)
{
  return print_swig_aux(swig_smob, port, pstate, "");
}

static int
print_collectable_swig (SCM swig_smob, SCM port, scm_print_state *pstate)
{
  return print_swig_aux(swig_smob, port, pstate, "collectable-");
}

static int
print_destroyed_swig (SCM swig_smob, SCM port, scm_print_state *pstate)
{
  return print_swig_aux(swig_smob, port, pstate, "destroyed-");
}

static SCM
equalp_swig (SCM A, SCM B)
{
  if (SCM_CELL_WORD_0(A) == SCM_CELL_WORD_0(B) && SCM_CELL_WORD_1(A) == SCM_CELL_WORD_1(B) 
      && SCM_CELL_WORD_2(A) == SCM_CELL_WORD_2(B))
    return SCM_BOOL_T;
  else return SCM_BOOL_F;
}

static size_t
free_swig(SCM A)
{
  swig_type_info *type = (swig_type_info *) SCM_CELL_WORD_2(A);
  if (type) {
    if (type->clientdata)
      ((swig_guile_clientdata *)type->clientdata)->destroy(A);
  } 
  return 0;
}

SWIGRUNTIME(void)
SWIG_Guile_Init ()
{
  if (!swig_tag) {
    swig_tag = scm_make_smob_type((char*)"swig-pointer", 0);
    scm_set_smob_print(swig_tag, print_swig);
    scm_set_smob_equalp(swig_tag, equalp_swig);
  }

  if (!swig_collectable_tag) {
    swig_collectable_tag = scm_make_smob_type((char*)"collectable-swig-pointer", 0);
    scm_set_smob_print(swig_collectable_tag, print_collectable_swig);
    scm_set_smob_equalp(swig_collectable_tag, equalp_swig);
    scm_set_smob_free(swig_collectable_tag, free_swig);
  }

  if (!swig_destroyed_tag) {
    swig_destroyed_tag = scm_make_smob_type((char*)"destroyed-swig-pointer", 0);
    scm_set_smob_print(swig_destroyed_tag, print_destroyed_swig);
    scm_set_smob_equalp(swig_destroyed_tag, equalp_swig);
  }
      
  swig_make_func = scm_permanent_object(
  scm_variable_ref(scm_c_module_lookup(scm_c_resolve_module("oop goops"), "make")));
  swig_keyword = scm_permanent_object(scm_c_make_keyword((char*) "init-smob"));
  swig_symbol = scm_permanent_object(scm_str2symbol("swig-smob"));
}

SWIGRUNTIME(int)
SWIG_Guile_GetArgs (SCM *dest, SCM rest,
		    int reqargs, int optargs,
		    const char *procname)
{
  int i;
  int num_args_passed = 0;
  for (i = 0; i<reqargs; i++) {
    if (!SCM_CONSP(rest))
      scm_wrong_num_args(scm_makfrom0str((char *) procname));
    *dest++ = SCM_CAR(rest);
    rest = SCM_CDR(rest);
    num_args_passed++;
  }
  for (i = 0; i<optargs && SCM_CONSP(rest); i++) {
    *dest++ = SCM_CAR(rest);
    rest = SCM_CDR(rest);
    num_args_passed++;
  }
  for (; i<optargs; i++)
    *dest++ = SCM_UNDEFINED;
  if (!SCM_NULLP(rest))
    scm_wrong_num_args(scm_makfrom0str((char *) procname));
  return num_args_passed;
}

/* This function will propagate the clientdata field of type to
* any new swig_type_info structures that have been added into the list
* of equivalent types.  It is like calling
* SWIG_TypeClientData(type, clientdata) a second time.
*/
SWIGRUNTIME(void) 
SWIG_Guile_PropagateClientData(swig_type_info *type) {
  swig_type_info *equiv = type->next;
  swig_type_info *tc;
  if (!type->clientdata) return;
  while (equiv) {
    if (!equiv->converter) {
      tc = swig_type_list;
      while (tc) {
        if ((strcmp(tc->name, equiv->name) == 0) && !tc->clientdata)
          SWIG_TypeClientData(tc, type->clientdata);
        tc = tc->prev;
      }
    }
    equiv = equiv->next;
  }
}

#endif

#ifdef __cplusplus
}
#endif
