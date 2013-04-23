/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_LBRACKET = 258,
     T_RBRACKET = 259,
     T_LABRACKET = 260,
     T_RABRACKET = 261,
     T_LBRACE = 262,
     T_RBRACE = 263,
     T_COLON = 264,
     T_COMMA = 265,
     T_NEWLINE = 266,
     T_EQUALS = 267,
     T_SEMI = 268,
     T_PIPE = 269,
     T_STAR = 270,
     T_LPAREN = 271,
     T_RPAREN = 272,
     KW_OBJECT = 273,
     KW_COMPONENT = 274,
     KW_TEMPLATE = 275,
     KW_FLOAT = 276,
     KW_INT = 277,
     KW_STRING = 278,
     KW_LIST = 279,
     KW_IN = 280,
     KW_BOOL = 281,
     KW_TRUE = 282,
     KW_FALSE = 283,
     T_STRING = 284,
     T_NAME = 285,
     T_INT = 286,
     T_FLOAT = 287
   };
#endif
#define T_LBRACKET 258
#define T_RBRACKET 259
#define T_LABRACKET 260
#define T_RABRACKET 261
#define T_LBRACE 262
#define T_RBRACE 263
#define T_COLON 264
#define T_COMMA 265
#define T_NEWLINE 266
#define T_EQUALS 267
#define T_SEMI 268
#define T_PIPE 269
#define T_STAR 270
#define T_LPAREN 271
#define T_RPAREN 272
#define KW_OBJECT 273
#define KW_COMPONENT 274
#define KW_TEMPLATE 275
#define KW_FLOAT 276
#define KW_INT 277
#define KW_STRING 278
#define KW_LIST 279
#define KW_IN 280
#define KW_BOOL 281
#define KW_TRUE 282
#define KW_FALSE 283
#define T_STRING 284
#define T_NAME 285
#define T_INT 286
#define T_FLOAT 287




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 55 "OL/ol.y"
typedef union YYSTYPE {
  float floatlit;
  int   intlit;
  char* stringlit;
  char* name;
  void* astnode;
  int   intval;  // is this necessary?
} YYSTYPE;
/* Line 1240 of yacc.c.  */
#line 109 "OL/ol_yacc.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE ol_lval;



