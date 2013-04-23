/**
  @file ast.c
  @author Matthew Hammer
  @date 


*/

#include "ast.h"
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include "ol_yacc.h"

extern void ol_errorline(const char *format, ...); 

int ol_validate_ast(ol_link_t* ast) {
  ol_link_t* n = ast;

  while(n) {
    if(n->type == OL_AST_DEF &&
       (n->u.def->parent /* template name */ )) {
      ol_template_t* templ = ol_find_template(ast, n->u.def->parent);
      if(templ) {
        int status = ol_validate_def_on_template(n->u.def, templ);
        if (!status) return status;
      }
      else {
        ol_push_error("no template named %s was found in AST", n->u.def->parent);
        return 0;
      }
    }
    n = n->next;
  }
  return 1;
}

// find a template by name
ol_template_t* ol_find_template(const ol_link_t* ast, const char* name) {
  const ol_link_t* n = ast;

  while(n) {
    if(n->type == OL_AST_TEMPLATE &&
       (!strcmp(n->u.templat->name, name)))
      return n->u.templat;
    n = n->next;
  }
  return NULL;
}

// find a keyval or inner def within a def by name
const ol_link_t* ol_def_find(const ol_def_t* def, const char* name) {
  ol_link_t* n = def->bodylist;
  while(n) {
    if(n->type == OL_AST_KEYVAL
       && !strcmp(n->u.keyval->name, name))
      return n;
    else if(n->type == OL_AST_DEF &&
            !strcmp(n->u.def->name, name))
      return n;
    n = n->next;
  }
  return NULL;
}

// nonzero if def validates against template, zero otherwise
int ol_validate_def_on_template(const ol_def_t* def, const ol_template_t* templ) {

  ol_link_t* n = templ->bodylist;

  fprintf(stderr, "checking: %s : %s \n", def->name, templ->name);
  
  while(n) {
    const ol_templateitem_t *titem = n->u.templateitem;    
    const ol_link_t* defnode = ol_def_find(def, titem->name);

    // check requirement failure
    if(!defnode && (titem->modifiers & OL_TMOD_REQUIRED)) {      
      ol_push_error("template %s, def %s: required item `%s' is not present in def",
                    templ->name, def->name, titem->name);
      return 0;
    }
    // check component success
    else if(titem->type == OL_VALX_COMPONENT) {
      if(n->type != OL_AST_DEF) {
        ol_push_error("template %s, def %s: wanted a component `%s',"
                      " found something else", templ->name, def->name, titem->name);
        return 0;
      }
      else {
        // we dont recursive into component (?)
        continue;
      }
    }    
    // check keyval validation
    else if(defnode && defnode->type == OL_AST_KEYVAL) {      
      fprintf(stderr, "  checking: %s \n", n->u.keyval->name);          

      {
        int valid = ol_validate_value_on_templateitem(defnode->u.keyval->value, titem);      
        if(!valid) {
          ol_push_error("template %s, def %s: key `%s' has wrong type"
                        " or doesn't meet constraints",
                        templ->name, def->name, titem->name);
          return 0;
        }}
    }    
    n = n->next; 
  }
  return 1;
}


// nonzero if value is valid against template item, 0 otherwise 
int ol_validate_value_on_templateitem(const ol_val_t* val,
                                      const ol_templateitem_t* templ) {
  int list_mod_set = (templ->modifiers & OL_TMOD_LIST);
      
  // handle case where value is a list
  if(val->type == OL_VAL_LIST) {
    int list_type = -1;
    
    if(!ol_validate_list(val, &list_type))
      return 0;

    {
      int list_type_match = (list_type < 0) || (templ->type == list_type);
            
      if(list_type_match && list_mod_set) {        
        int status = ol_validate_value_on_constr(val, templ->constr);
        if (!status)
          ol_push_error("contraints for %s unmet", templ->name);
        return status;
      }
      else if(!list_type_match) {
        ol_push_error("list type mismatch, (%d != %d)", list_type, templ->type);
        return 0;
      }
      else if(!list_mod_set) {
        ol_push_error("template doesnt type value as a list");
        return 0;
      }
    }      
  }
  // handle scalar cases
  {
    if((val->type != templ->type) || list_mod_set) {
      if(list_mod_set)
        ol_push_error("template requires a list for item `%s'", templ->name);
      else
        ol_push_error("scalar type mismatch on item `%s'", templ->name);
      return 0;
    } else
      return ol_validate_value_on_constr(val, templ->constr);      
  }
  return -1;
}

// nonzero if value is valid against template item, 0 otherwise 
int ol_validate_value_on_constr(const ol_val_t* val,
                                const ol_templateconstr_t* constr) {
  if(constr->type == OL_TCONSTR_ALT) {
    return (ol_validate_value_on_constr(val, constr->u.alt.a) ||
            ol_validate_value_on_constr(val, constr->u.alt.b));      
  }
  else if(constr->type == OL_TCONSTR_RANGE) {
    assert(constr->u.range.lower->type == OL_TCONSTR_LIT);
    assert(constr->u.range.upper->type == OL_TCONSTR_LIT);
    {
      int cmp1 = ol_value_cmp(val, constr->u.range.lower->u.lit_val);
      int cmp2 = ol_value_cmp(val, constr->u.range.upper->u.lit_val);

      if(cmp1 == TYPE_MISMATCH || cmp2 == TYPE_MISMATCH) {
        ol_push_error("type mismatch while applying constraint");
        return 0;
      }
      if(cmp1 == OTHER_ERROR) {
        ol_push_error("internal error while applying constraint");
        return 0;
      }
      if(cmp1 >= 0 && cmp2 <= 0)
        return 1;
      else {
        ol_push_error("range constraint not met");
        return 0;
      }
    }
  }
  else if(constr->type == OL_TCONSTR_LIT) {
    return ol_value_cmp(val, constr->u.lit_val);
  }  
  return -1;
}

int ol_value_cmp(const ol_val_t* v1, const ol_val_t* v2) {
  // v1 and v2 must be same type to compare
  if(v1->type != v2->type)
    return TYPE_MISMATCH;

  // int
  if(v1->type == OL_VAL_INT) {
    if(v1->u.val_int > v2->u.val_int) return 1;
    else if(v1->u.val_int == v2->u.val_int) return 0;
    else return -1;
  }

  // float
  else if(v1->type == OL_VAL_FLOAT) {
    if(v1->u.val_float > v2->u.val_float) return 1;
    else if(v1->u.val_float == v2->u.val_float) return 0;
    else return -1;
  }
  
  // bool
  else if(v1->type == OL_VAL_BOOL) {
    if(v1->u.val_bool == v2->u.val_bool) return 0;
    return -1;
  }

  // string
  else if(v1->type == OL_VAL_STRING)
    return strcmp(v1->u.val_string, v2->u.val_string);

  // list
  else if(v1->type == OL_VAL_LIST) {
    int v1t, v2t;
    if(ol_validate_list(v1, &v1t) && ol_validate_list(v1, &v2t)) {
      if(v1t != v2t)
        return TYPE_MISMATCH;
      else {      
        const ol_link_t *n1 = v1->u.val_list;
        const ol_link_t *n2 = v2->u.val_list;
        while(n1 && n2) {
          int cmp = ol_value_cmp(n1->u.value, n2->u.value);
          if(cmp != 0)
            return cmp;
          n1 = n1->next;
          n2 = n2->next;
        }
        if(n1 == NULL && n2 != NULL) return -1;
        else if(n2 == NULL && n1 != NULL) return 1;
        else return 0;
      }
    }
    else
      return TYPE_MISMATCH;
  }
  else return OTHER_ERROR;
}

// check value list
int ol_validate_list(const ol_val_t* val, int* list_type) {
  if(val->type == OL_VAL_LIST &&
     (val->u.val_list))
  {
    const ol_link_t* l = val->u.val_list;
    const ol_val_t* v = NULL;
    
    if(l && l->type == OL_AST_VALUE) {
      v = l->u.value;
      l = l->next;
    }
    else
      return 1;
    
    while(l) {
      if(v->type != l->u.value->type) {
        ol_push_error("list with mixed types");
        return 0;
      }          
      l = l->next;
    }
    
    if(list_type)
      *list_type = v->type;
  }    
  return 1;
}
