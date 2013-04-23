/**
  @file ol_yacc.y
  @author Matthew Hammer
  @date Wed Jan 12 07:13:20 CST 2005

  Copyright (C) 2005 Stolen Notebook
*/

%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>
#include <malloc.h>
#include "OL/ast.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef DEBUG
#define LOG(msg) printf(msg)
#else
#define LOG(msg)
#endif

extern int ol_lex();

extern void ol_set_lexer_source(char *src, int size);
extern void ol_close_lexer();

void ol_error(const char *format, ...);
void ol_errorline(const char *format, ...); 
int ol_lineno;

extern char* ol_text;
ol_link_t* ol_root = 0;

ol_link_t* olAstParse(const char *src, int size)
{
  ol_root = 0;
  ol_set_lexer_source((char*)src, size);
  ol_parse();
  ol_close_lexer();
  return ol_root;
}

%}



%union
{
  float floatlit;
  int   intlit;
  char* stringlit;
  char* name;
  void* astnode;
  int   intval;  // is this necessary?
};


%start file

%token T_LBRACKET
%token T_RBRACKET
%token T_LABRACKET
%token T_RABRACKET
%token T_LBRACE
%token T_RBRACE
%token T_COLON
%token T_COMMA
%token T_NEWLINE
%token T_EQUALS
%token T_SEMI
%token T_PIPE
%token T_STAR
%token T_LPAREN
%token T_RPAREN

%token KW_OBJECT
%token KW_COMPONENT
%token KW_TEMPLATE
%token KW_FLOAT
%token KW_INT
%token KW_STRING
%token KW_LIST
%token KW_IN
%token KW_BOOL
%token KW_TRUE
%token KW_FALSE

%token <stringlit> T_STRING
%token <name>      T_NAME
%token <intlit>    T_INT
%token <floatlit>  T_FLOAT

%type <astnode> file file_body defval defval_body
%type <astnode> componentval
%type <astnode> optional_name optional_parent keyval value_list value_list_body0 value_list_body1 value

%type <astnode> templat template_body template_item
%type <astnode> optional_template_item_default optional_template_item_constr template_item_constr
%type <intval> optional_list
%type <intval> optional_required
%type <intval> template_item_type

%left T_PIPE
%left T_COMMA

%%

file: file_body { ol_root = $1; }

file_body:
  defval file_body { $$ = ol_ast_node(OL_AST_LINK, OL_AST_DEF, $1, $2); }
| templat file_body   { $$ = ol_ast_node(OL_AST_LINK, OL_AST_TEMPLATE, $1, $2); }
| { $$ = NULL; }
;


/* TEMPLATE */
templat:
  T_LABRACKET KW_TEMPLATE T_NAME T_RABRACKET 
  T_LBRACE template_body T_RBRACE
  { $$ = ol_ast_node(OL_AST_TEMPLATE, $3, $6); }
;

template_body:
  template_item template_body { $$ = ol_ast_node(OL_AST_LINK, OL_AST_TEMPLATEITEM, $1, $2); }
| { $$ = NULL; }
;

template_item:
  optional_required T_NAME T_COLON
  template_item_type optional_list
  optional_template_item_default
  optional_template_item_constr T_SEMI
{ $$ = ol_ast_node(OL_AST_TEMPLATEITEM, $2, $4, ($1 | $5), $6, $7); }
;

template_item_type:
  KW_INT { $$ = OL_VAL_INT; }
| KW_FLOAT { $$ = OL_VAL_FLOAT; }
| KW_STRING { $$ = OL_VAL_STRING; }
| KW_BOOL { $$ = OL_VAL_BOOL; }
| KW_COMPONENT { $$ = OL_VALX_COMPONENT; }
;

optional_required:
  T_STAR { $$ = OL_TMOD_REQUIRED; }
| { $$ = 0; }
;

optional_list:
  KW_LIST { $$ = OL_TMOD_LIST; }
| { $$ = 0; }
;

optional_template_item_default:
{ $$ = NULL; }
| T_EQUALS value
{ $$ = $2; }
;

optional_template_item_constr:
  KW_IN template_item_constr { $$ = $2; }
| { $$ = NULL; }
;

template_item_constr:
 template_item_constr T_COMMA template_item_constr
{ $$ = ol_ast_node(OL_AST_TEMPLATECONSTR, OL_TCONSTR_RANGE, $1, $3); }

| T_LPAREN template_item_constr T_RPAREN { $$ = $2; }

| template_item_constr T_PIPE template_item_constr
{ $$ = ol_ast_node(OL_AST_TEMPLATECONSTR, OL_TCONSTR_ALT, $1, $3); }

| value
{ $$ = ol_ast_node(OL_AST_TEMPLATECONSTR, OL_TCONSTR_LIT, $1); }
;


/* DEF */
defval:
  T_LABRACKET T_NAME T_NAME T_RABRACKET optional_parent
  T_LBRACE defval_body T_RBRACE
  { $$ = ol_ast_node(OL_AST_DEF, $2, $3, $5, $7); }
;

defval_body:
  componentval defval_body
{ $$ = ol_ast_node(OL_AST_LINK, OL_AST_DEF, $1, $2); }

| keyval defval_body
{ $$ = ol_ast_node(OL_AST_LINK, OL_AST_KEYVAL, $1, $2); }

| { $$ = NULL; }
;

componentval:
  T_LBRACKET T_NAME optional_name T_RBRACKET 
  T_LBRACE defval_body T_RBRACE
  { $$ = ol_ast_node(OL_AST_DEF, $2, $3, 0, $6); }

| T_LBRACKET T_NAME optional_name T_RBRACKET
  { $$ = ol_ast_node(OL_AST_DEF, $2, $3, 0, 0); }
;

optional_parent:
  T_COLON T_NAME { $$ = $2; }
| { $$ = NULL; }
;

optional_name:
  T_NAME { $$ = $1; }
| { $$ = NULL; }
;

keyval:
  T_NAME T_EQUALS value T_SEMI
{ $$ = ol_ast_node(OL_AST_KEYVAL, $1, $3); }
;

value_list:
  T_LBRACKET value_list_body0 T_RBRACKET
{ $$ = $2; }
| T_LBRACKET T_RBRACKET
{ $$ = NULL; }
;

value_list_body0:
  value value_list_body1
{ $$ = ol_ast_node(OL_AST_LINK, OL_AST_VALUE, $1, $2); }
;

value_list_body1:
  T_COMMA value_list_body0
{ $$ = $2; }
| { $$ = NULL; }
;

value:
  T_STRING   { $$ = ol_ast_node(OL_AST_VALUE, OL_VAL_STRING, $1); }
| T_INT      { $$ = ol_ast_node(OL_AST_VALUE, OL_VAL_INT, $1); }
| T_FLOAT    { $$ = ol_ast_node(OL_AST_VALUE, OL_VAL_FLOAT, $1); }
| T_NAME     { $$ = ol_ast_node(OL_AST_VALUE, OL_VAL_STRING, $1); }
| value_list
{ {
  void* list = ol_ast_node(OL_AST_VALUE, OL_VAL_LIST, $1);
  if(!ol_validate_list(list, NULL))
    YYABORT;
  $$ = list;
  }
} 
| KW_TRUE    { $$ = ol_ast_node(OL_AST_VALUE, OL_VAL_BOOL, -1); }
| KW_FALSE   { $$ = ol_ast_node(OL_AST_VALUE, OL_VAL_BOOL, 0); }

/* vec2 */
| T_LABRACKET T_FLOAT T_COMMA T_FLOAT T_RABRACKET
{ $$ = ol_ast_node(OL_AST_VALUE, OL_VAL_VEC2, $2, $4); }

/* vec3 */
| T_LABRACKET T_FLOAT T_COMMA T_FLOAT T_COMMA T_FLOAT T_RABRACKET
{ $$ = ol_ast_node(OL_AST_VALUE, OL_VAL_VEC3, $2, $4, $6); }

/* vec4 */
| T_LABRACKET T_FLOAT T_COMMA T_FLOAT T_COMMA T_FLOAT T_COMMA T_FLOAT T_RABRACKET
{ $$ = ol_ast_node(OL_AST_VALUE, OL_VAL_VEC4, $2, $4, $6, $8); }
%%

void ol_errorline(const char *format, ...)
{
#define buff_size 256
  char buff[buff_size];

	va_list ap;

	va_start(ap, format);
	vsprintf(buff, format, ap);
	va_end(ap);

	fprintf(stderr, "error, line %d: %s\n", ol_lineno,buff);  
}

void ol_error(const char *format, ...)
{
#define buff_size 256
  char buff[buff_size];

	va_list ap;

	va_start(ap, format);
	vsprintf(buff, format, ap);
	va_end(ap);

	fprintf(stderr, "%s: %d: \'%s\'\n", buff, ol_lineno, ol_text);  
}


