/**
  @file ast.c
  @author Matthew Hammer
  @date Wed Jan 12 07:13:20 CST 2005


*/

#include "ast.h"
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include "ol_yacc.h"

void* olAstParse(const char* buffer, int size);

ol_link_t* olParse(const char* buffer, int size)
{
  return (ol_link_t*) olAstParse(buffer, size);
}

#define XTOA_BUFF_SZ 32


static char* ftoa(double f) {
  char* s = (char*) malloc(XTOA_BUFF_SZ);
  sprintf(s, "%f", f);
  return s; }

static char* itoa(int i) {
  char* s = (char*) malloc(XTOA_BUFF_SZ);
  sprintf(s, "%d", i);
  return s; }

static int vec_size(int type) {
  switch(type){
  case OL_VAL_VEC2: return 2;
  case OL_VAL_VEC3: return 3;
  case OL_VAL_VEC4: return 4;
  default:
    return 0;
  }
}

void* ol_ast_node(int type, ...) {
  va_list ap;
  void *return_val = 0;
  va_start(ap, type);
  
#define RETURN(val) {return_val=val; goto done;}  

  /* LINK */
  if(type == OL_AST_LINK) {
    VAR_DECL(ol_link_t, o);
    o->type = va_arg(ap, int);
        
    if(o->type == OL_AST_DEF) {
      o->u.def = va_arg(ap, ol_def_t*);
    }

    else if(o->type == OL_AST_TEMPLATE) {
      o->u.templat = va_arg(ap, ol_template_t*);
    }

    else if(o->type == OL_AST_TEMPLATEITEM) {
      o->u.templateitem = va_arg(ap, ol_templateitem_t*);
    }

    else if(o->type == OL_AST_KEYVAL) {
      o->u.keyval = va_arg(ap, ol_keyval_t*);
    }

    else if(o->type == OL_AST_VALUE) {
      o->u.value = va_arg(ap, ol_val_t*);      
    }

    o->next = va_arg(ap, ol_link_t*);
    RETURN(o);    
  }
  
  /* DEF */
  else if(type == OL_AST_DEF) {
    VAR_DECL(ol_def_t, o);
    o->type = va_arg(ap, char*);
    o->name = va_arg(ap, char*);
    o->parent = va_arg(ap, char*);
    
    o->bodylist = va_arg(ap, ol_link_t*);    
    RETURN(o);    
  }

  /* KEYVAL */
  else if(type == OL_AST_KEYVAL) {
    VAR_DECL(ol_keyval_t, o);    

    o->name = va_arg(ap, char*);
    o->value = va_arg(ap, ol_val_t*);
    RETURN(o);
  }

  /* VALUE */
  else if(type == OL_AST_VALUE) {
    VAR_DECL(ol_val_t, o);
    o->type = va_arg(ap, int);
          
    if(o->type == OL_VAL_STRING) {
      o->u.val_string = va_arg(ap, char*);
      
    } else if(o->type == OL_VAL_INT) {
      o->u.val_int = va_arg(ap, int);      
      
    } else if(o->type == OL_VAL_FLOAT) {
      o->u.val_float = (float)va_arg(ap, double);
    }        
    else if(o->type == OL_VAL_BOOL) {
      o->u.val_bool = va_arg(ap, int);
    }
    else if(o->type == OL_VAL_LIST) {
      o->u.val_list = va_arg(ap, ol_link_t*);
    }
    else if((o->type >= OL_VAL_VEC2) &&
            (o->type <= OL_VAL_VEC4))
    {
      int size = vec_size(o->type);        
      float *v = malloc(sizeof(float) * size);
      {int i = 0; for(; i < size; i++) {
        v[i] = (float) va_arg(ap, double);
      }}
      o->u.val_vec = v;
    }
    RETURN(o);
  }
  
  /* TEMPLATE */
  else if(type == OL_AST_TEMPLATE) {
    VAR_DECL(ol_template_t, o);
    o->name = va_arg(ap, char*);
    o->bodylist = va_arg(ap, ol_link_t*);
    RETURN(o);
  }

  /* ITEM */
  else if(type == OL_AST_TEMPLATEITEM) {
    VAR_DECL(ol_templateitem_t, o);
    o->name = va_arg(ap,char*);
    o->type = va_arg(ap,int);
    o->modifiers = va_arg(ap,int);

    if(o->type == OL_VALX_COMPONENT) {
      ol_val_t* v = va_arg(ap, ol_val_t*);
      o->def_val->type = OL_VALX_COMPONENT;
      o->def_val->u.val_string = v->u.val_string;
      /** todo: free v */
    }
    else {
      ol_val_t* v = va_arg(ap, ol_val_t*);      
      o->def_val = v;

      if(!v) {
        if(o->type == OL_VAL_STRING)
          o->def_val = ol_ast_node(OL_AST_VALUE, OL_VAL_STRING, strdup("(empty)"));

        else if(o->type == OL_VAL_INT)
          o->def_val = ol_ast_node(OL_AST_VALUE, OL_VAL_INT, 0);

        else if(o->type == OL_VAL_FLOAT)
          o->def_val = ol_ast_node(OL_AST_VALUE, OL_VAL_FLOAT, 0.0f);

        else if(o->type == OL_VAL_BOOL)
          o->def_val = ol_ast_node(OL_AST_VALUE, OL_VAL_BOOL, 0);
      }
      
      o->constr = va_arg(ap, ol_templateconstr_t*);    
      RETURN(o);
    }
  }

 
  /* CONSTRAINT */
  else if(type == OL_AST_TEMPLATECONSTR) {
    VAR_DECL(ol_templateconstr_t, o);
    o->type = va_arg(ap,int);
    
    if(o->type == OL_TCONSTR_ALT) {
      o->u.alt.a = va_arg(ap,ol_templateconstr_t*);
      o->u.alt.b = va_arg(ap,ol_templateconstr_t*);
    }
    else if(o->type == OL_TCONSTR_RANGE) {
      o->u.range.lower = va_arg(ap,ol_templateconstr_t*);
      o->u.range.upper = va_arg(ap,ol_templateconstr_t*);
    }
    else if(o->type == OL_TCONSTR_LIT) {
      o->u.lit_val = va_arg(ap,ol_val_t*);
    }  
    RETURN(o);
  }

  
 done:
  va_end(ap);
  return return_val;
}
