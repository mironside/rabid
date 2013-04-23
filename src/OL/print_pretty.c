#include "ast.h"
#include <assert.h>

// indent some ammount on FILE out
static void indent(FILE* out, int amt) {
  int i=0; for(; i < amt; i++) {
    fprintf(out, "  ");
  }}

// print the AST at indent 0
void ol_ast_print(FILE* out, int type, void* node) {
  ol_ast_printi(out, type, node, 0); }

// print an AST node with indent on FILE out
void ol_ast_printi(FILE* out, int type, void* node, int ind) {    

  if(node == NULL)
    return;

  else if(type == OL_AST_LINK) {
    ol_link_t* l = (ol_link_t*) node;

    while(l != NULL) {      
      if(l->type == OL_AST_DEF)
        ol_ast_printi(out, OL_AST_DEF, l->u.def, ind);

      else if(l->type == OL_AST_TEMPLATE)      
        ol_ast_printi(out, OL_AST_TEMPLATE, l->u.templat, ind);

      else if(l->type == OL_AST_TEMPLATEITEM)      
        ol_ast_printi(out, OL_AST_TEMPLATEITEM, l->u.templateitem, ind);

      else if(l->type == OL_AST_KEYVAL)      
        ol_ast_printi(out, OL_AST_KEYVAL, l->u.keyval, ind);

      else if(l->type == OL_AST_VALUE) {
        fprintf(out, "[");
        while(l != NULL) {
          ol_ast_printi(out, OL_AST_VALUE, l->u.value, ind);
          fprintf(out, "%s", l->next != NULL ? ", " : "");
          l = l->next;          
        }
        fprintf(out, "]");
        break;
      }
      
      l = l->next;
    }
  }

  else if(type == OL_AST_DEF) {
    ol_def_t* o = (ol_def_t*) node;
    
    indent(out, ind);
    if(ind == 0)
    {
      fprintf(out, "<%s %s>", o->type, o->name);
      if(o->parent)
        fprintf(out, " : %s\n", o->parent);
    }
    else
    {
      fprintf(out, "[%s", o->type);
      if(o->name)
        fprintf(out, " %s", o->name);
      fprintf(out, "]\n");
    }
    indent(out, ind);

    if(o->bodylist) {
      fprintf(out, "{\n");

      ol_ast_printi(out, OL_AST_LINK, o->bodylist, ind+1);
    
      indent(out, ind);
      fprintf(out, "}\n\n");
    }
  }

  /*
  else if(type == OL_AST_COMPONENT) {
    ol_component_t* c = (ol_component_t*) node;

    indent(out, ind);
    fprintf(out, "[%s]", c->name);

    if(c->initializer)
      fprintf(out, " : %s", c->initializer);

    if(c->bodylist) {
      fprintf(out, " {\n");
      ol_ast_printi(out, OL_AST_LINK, c->bodylist, ind+1);      
      indent(out, ind);
      fprintf(out, "}\n");
    }
  }
  */
  else if(type == OL_AST_KEYVAL) {
    ol_keyval_t* k = (ol_keyval_t*) node;
    
    indent(out, ind);
    
    fprintf(out, "%s = ", k->name);
    ol_ast_printi(out, OL_AST_VALUE, k->value, ind);
    fprintf(out, ";\n");
  }

  else if(type == OL_AST_TEMPLATE) {
    ol_template_t* o = (ol_template_t*) node;
    
    indent(out, ind);
    fprintf(out, "<template %s> {\n", o->name);
    
    ol_ast_printi(out, OL_AST_LINK, o->bodylist, ind+1);    

    indent(out, ind);
    fprintf(out, "}\n\n");
  }
  else if(type == OL_AST_TEMPLATEITEM) {
    ol_templateitem_t* o = (ol_templateitem_t*) node;
    
    if(o->modifiers & OL_TMOD_REQUIRED) {
      indent(out, ind-1);
      fprintf(out, " *");
    } else
      indent(out, ind);

    fprintf(out, "%s", o->name);

    if(o->type == OL_VALX_COMPONENT) {
      fprintf(out, ": component;\n");
    }
    else {
      fprintf(out, " : ");

      //
      // print type
      //
      if(o->type == OL_VAL_STRING)
        fprintf(out, "string");
      else if(o->type == OL_VAL_INT)
        fprintf(out, "int");        
      else if(o->type == OL_VAL_FLOAT)
        fprintf(out, "float");
      else if(o->type == OL_VAL_BOOL)
        fprintf(out, "bool");

      if(o->modifiers & OL_TMOD_LIST)
        fprintf(out, " list");      
      
      //
      // print default value
      //
      if(o->def_val) {
        fprintf(out, " = ");      
        ol_ast_printi(out, OL_AST_VALUE, o->def_val, ind);
      }
      
      //
      // print constraints
      //
      if(o->constr) {
        fprintf(out, " in ");
        ol_ast_printi(out, OL_AST_TEMPLATECONSTR, o->constr, 0);
      }
      
      fprintf(out, ";\n");      
    }
  }

  else if(type == OL_AST_TEMPLATECONSTR) {
    ol_templateconstr_t* o = (ol_templateconstr_t*) node;

    if(o->type == OL_TCONSTR_ALT) {
      ol_ast_printi(out, OL_AST_TEMPLATECONSTR, o->u.alt.a, 0);      
      fprintf(out, " | ");
      ol_ast_printi(out, OL_AST_TEMPLATECONSTR, o->u.alt.b, 0);
    }
    else if(o->type == OL_TCONSTR_RANGE) {
      fprintf(out, "(");
      ol_ast_printi(out, OL_AST_TEMPLATECONSTR, o->u.range.lower, 0);
      fprintf(out, ", ");
      ol_ast_printi(out, OL_AST_TEMPLATECONSTR, o->u.range.upper, 0);
      fprintf(out, ")");
    }
    else if(o->type == OL_TCONSTR_LIT) {
      ol_ast_printi(out, OL_AST_VALUE, o->u.lit_val, ind);
    }    
  }

  else if(type == OL_AST_VALUE) {
    ol_val_t* o = (ol_val_t*) node;

    //fprintf(out, "~value~ ");
    
    switch(o->type) {
    case OL_VAL_STRING:
      fprintf(out, "%s", o->u.val_string);
      break;
    case OL_VAL_INT:
      fprintf(out, "%d", o->u.val_int);
      break;
    case OL_VAL_FLOAT:
      fprintf(out, "%f", o->u.val_float);
      break;
    case OL_VAL_LIST:
      if(o->u.val_list)
        ol_ast_printi(out, OL_AST_LINK, o->u.val_list, ind);
      else
        fprintf(out, "[]");
      break;
    case OL_VAL_BOOL:
      fprintf(out, "%s", o->u.val_bool ? "true" : "false");
      break;
    case OL_VAL_VEC2:
      fprintf(out, "<%f, %f>",
              o->u.val_vec[0],
              o->u.val_vec[1]);
      break;
    case OL_VAL_VEC3:
      fprintf(out, "<%f, %f, %f>",
              o->u.val_vec[0],
              o->u.val_vec[1],
              o->u.val_vec[2]);
      break;
    case OL_VAL_VEC4:
      fprintf(out, "<%f, %f, %f, %f>",
              o->u.val_vec[0],
              o->u.val_vec[1],
              o->u.val_vec[2],
              o->u.val_vec[3]);
      break;
    case OL_VALX_COMPONENT:
      fprintf(out, "%s", o->u.val_string);
      break;
    default:
      assert(!"invalid value type");
    }    
  }
}
  

