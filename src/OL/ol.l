/**
  @file ol_lex.l
  @author Matthew Hammer
  @date Wed Jan 12 07:13:20 CST 2005

  Copyright (C) 2005 Stolen Notebook
*/

%{
#include "OL/ast.h"
#include "OL/ol_yacc.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define YY_SKIP_YYWRAP
int yywrap() { return 1; }
extern int ol_lineno;
char* ol_text;

// exports for parser to parse from memory
YY_BUFFER_STATE buf_state;
void ol_set_lexer_source(char *src, int size) { buf_state = yy_scan_bytes(src, size); }
void ol_close_lexer() { yy_delete_buffer(buf_state); }
%}



LETTER    [a-zA-Z_]
DIGIT     [0-9]
IDENT     {LETTER}({LETTER}|{DIGIT})*
STRING    \"[^\"]*\"
WSPACE    [ \t\r]+
SECTION   "["{IDENT}"]"
FLOAT     [\-]?{DIGIT}+((["."]{DIGIT}+)|((["."]{DIGIT}+)?f))
INT       [\-]?{DIGIT}+
NAME      ({LETTER}|{DIGIT}|"/"|"."|"-")*


%%
"{"         { return T_LBRACE; }
"}"         { return T_RBRACE; }
"["         { return T_LBRACKET; }
"]"         { return T_RBRACKET; }
"<"         { return T_LABRACKET; }
">"         { return T_RABRACKET; }
":"         { return T_COLON; }
";"         { return T_SEMI; }
","         { return T_COMMA; }
"="         { return T_EQUALS; }
"|"         { return T_PIPE; }
"*"         { return T_STAR; }
"("         { return T_LPAREN; }
")"         { return T_RPAREN; }

#"object"    { return KW_OBJECT; }
"component" { return KW_COMPONENT; }
"template"  { return KW_TEMPLATE; }
"float"     { return KW_FLOAT; }
"int"       { return KW_INT; }
"string"    { return KW_STRING; }
"list"      { return KW_LIST; }
"in"        { return KW_IN; }
"true"      { return KW_TRUE; }
"false"     { return KW_FALSE; }
"bool"      { return KW_BOOL; }



{FLOAT}     {
              ol_lval.floatlit = atof(yytext);
              return T_FLOAT;
            }

{INT}       {
              ol_lval.intlit = atoi(yytext);
              return T_INT;
            }


{NAME}      {
              ol_lval.stringlit = (char*)malloc(strlen(ol_text)+1);
              memset(ol_lval.name, 0, strlen(ol_text)+1);
              strcpy(ol_lval.name, ol_text);
              return T_NAME;
            }

{STRING}    { ol_lval.stringlit = (char*)malloc(strlen(ol_text)+1-2);
              memset(ol_lval.stringlit, 0, strlen(ol_text)+1-2);
              strncpy(ol_lval.stringlit, ol_text+1, strlen(ol_text)-2);
              return T_STRING;
            }


"#"         {
              while(input() != '\n') {}
              ol_lineno++;
            }


\n          {
              ol_lineno++;
            }

\r          {
            }


{WSPACE}    {
            }


.           {
              FILE* file = fopen("error.log", "w");
              fprintf(file, "Lexical Error on line %d:\n\'%s'\n",
                            ol_lineno+1, ol_text);
              fclose(file);
              exit(1);
            }

%%
