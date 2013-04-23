/**
  @file ast.h
  @author Matthew Hammer
  @date Wed Jan 12 07:13:20 CST 2005


*/

#ifndef __OL_AST__
#define __OL_AST__

#include <stdio.h>

#define DECLARE_STRUCT(sname, tname) \
  struct sname; typedef struct sname tname;

DECLARE_STRUCT(ol_def_s, ol_def_t)
DECLARE_STRUCT(ol_template_s, ol_template_t)
DECLARE_STRUCT(ol_templateitem_s, ol_templateitem_t)
DECLARE_STRUCT(ol_templateconstr_s, ol_templateconstr_t)
DECLARE_STRUCT(ol_keyval_s, ol_keyval_t)
DECLARE_STRUCT(ol_val_s, ol_val_t)
DECLARE_STRUCT(ol_link_s, ol_link_t)

typedef enum ol_asttype_e {
  OL_AST_LINK,
  OL_AST_DEF,
  OL_AST_KEYVAL,
  OL_AST_VALUE,
  OL_AST_TEMPLATE,
  OL_AST_TEMPLATEITEM,
  OL_AST_TEMPLATECONSTR
} ol_asttype_t;

//
// VALUE
//
struct ol_val_s {
  enum { OL_VAL_STRING,
         OL_VAL_INT,
         OL_VAL_FLOAT,
         OL_VAL_BOOL,
         OL_VAL_LIST,
         /* vector types */
         OL_VAL_VEC2,
         OL_VAL_VEC3,
         OL_VAL_VEC4,
         /* extra value type(s) for template items, not used in usual
            ol_val_s structs: */
         OL_VALX_COMPONENT
  } type;

  union {
    int   val_int;
    float val_float;
    char* val_string;
    int   val_bool;
    float* val_vec;
    ol_link_t* val_list;
  } u;
};

//
// KEYVAL
//
struct ol_keyval_s {
  char* name;
  ol_val_t* value;
};

//
// LINK
//
struct ol_link_s {
  ol_asttype_t type;  

  union {
    ol_def_t* def;
    ol_template_t* templat;
    ol_templateitem_t* templateitem;
    ol_keyval_t* keyval;
    ol_val_t* value;
  } u;
  
  ol_link_t* next;
};

//
// DEF
//
struct ol_def_s {
  char* type;
  char* name;
  char* parent;
  ol_link_t* bodylist;
};

//
// TEMPLATE
//
struct ol_template_s {
  char* name;
  ol_link_t* bodylist;
};

//
// TEMPLATE ITEM
//
struct ol_templateitem_s {
  char *name;
  int type;

  enum {
    OL_TMOD_LIST  = 0x01,
    OL_TMOD_REQUIRED = 0x02
  } modifiers;
  
  ol_val_t* def_val;
  ol_templateconstr_t* constr;  
};

//
// TEMPLATE CONSTRAINT
//
struct ol_templateconstr_s {
  enum {
    OL_TCONSTR_ALT,
    OL_TCONSTR_RANGE,
    OL_TCONSTR_LIT
  } type;

  union {

    // ALTERNATIVE
    struct {
      ol_templateconstr_t* a;
      ol_templateconstr_t* b;
    } alt;

    // RANGE
    struct {
      ol_templateconstr_t* lower;
      ol_templateconstr_t* upper;
    } range;

    // LITERAL
    ol_val_t* lit_val;
  } u;
};


#define VAR_DECL(t, v) t* v = malloc(sizeof(t)); memset(v, 0, sizeof(t))

#ifdef __cplusplus
extern "C"
{
#endif
void* ol_ast_node(int type, ...);
void ol_ast_print(FILE* file, int type, void* node);
void ol_ast_printi(FILE* file, int type, void* node, int ind);

ol_template_t* ol_find_template(const ol_link_t* ast, const char* name);
  
// typechecking / templatechecking API
int ol_validate_ast(ol_link_t* ast);
int ol_validate_def_on_template(const ol_def_t* def, const ol_template_t* templ);
int ol_validate_value_on_templateitem(const ol_val_t* val, const ol_templateitem_t* templ);
int ol_validate_value_on_constr(const ol_val_t* val, const ol_templateconstr_t* constr);
int ol_validate_list(const ol_val_t* list, int* list_type); 

// value_cmp : compare values much like strcmp(3)
#define TYPE_MISMATCH -2
#define OTHER_ERROR -3
int ol_value_cmp(const ol_val_t* v1, const ol_val_t* v2);

// error reporting
void ol_push_error(const char* fmt, ...);
char* ol_pop_error();
const char* ol_last_error();
void ol_print_error_trace(FILE* out);
  
// outter API
ol_link_t* olParse(const char *src, int size);
#ifdef __cplusplus
}
#endif


#endif
