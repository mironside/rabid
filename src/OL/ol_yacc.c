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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* If NAME_PREFIX is specified substitute the variables and functions
   names.  */
#define yyparse ol_parse
#define yylex   ol_lex
#define yyerror ol_error
#define yylval  ol_lval
#define yychar  ol_char
#define yydebug ol_debug
#define yynerrs ol_nerrs


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




/* Copy the first part of user declarations.  */
#line 9 "OL/ol.y"

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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

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
/* Line 191 of yacc.c.  */
#line 200 "OL/ol_yacc.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 212 "OL/ol_yacc.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   95

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  33
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  22
/* YYNRULES -- Number of rules. */
#define YYNRULES  52
/* YYNRULES -- Number of states. */
#define YYNSTATES  95

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   287

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     5,     8,    11,    12,    20,    23,    24,
      33,    35,    37,    39,    41,    43,    45,    46,    48,    49,
      50,    53,    56,    57,    61,    65,    69,    71,    80,    83,
      86,    87,    95,   100,   103,   104,   106,   107,   112,   116,
     119,   122,   125,   126,   128,   130,   132,   134,   136,   138,
     140,   146,   154
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      34,     0,    -1,    35,    -1,    45,    35,    -1,    36,    35,
      -1,    -1,     5,    20,    30,     6,     7,    37,     8,    -1,
      38,    37,    -1,    -1,    40,    30,     9,    39,    41,    42,
      43,    13,    -1,    22,    -1,    21,    -1,    23,    -1,    26,
      -1,    19,    -1,    15,    -1,    -1,    24,    -1,    -1,    -1,
      12,    54,    -1,    25,    44,    -1,    -1,    44,    10,    44,
      -1,    16,    44,    17,    -1,    44,    14,    44,    -1,    54,
      -1,     5,    30,    30,     6,    48,     7,    46,     8,    -1,
      47,    46,    -1,    50,    46,    -1,    -1,     3,    30,    49,
       4,     7,    46,     8,    -1,     3,    30,    49,     4,    -1,
       9,    30,    -1,    -1,    30,    -1,    -1,    30,    12,    54,
      13,    -1,     3,    52,     4,    -1,     3,     4,    -1,    54,
      53,    -1,    10,    52,    -1,    -1,    29,    -1,    31,    -1,
      32,    -1,    30,    -1,    51,    -1,    27,    -1,    28,    -1,
       5,    32,    10,    32,     6,    -1,     5,    32,    10,    32,
      10,    32,     6,    -1,     5,    32,    10,    32,    10,    32,
      10,    32,     6,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   115,   115,   118,   119,   120,   126,   132,   133,   137,
     145,   146,   147,   148,   149,   153,   154,   158,   159,   163,
     164,   169,   170,   174,   177,   179,   182,   189,   195,   198,
     201,   205,   209,   214,   215,   219,   220,   224,   229,   231,
     236,   241,   243,   247,   248,   249,   250,   251,   259,   260,
     263,   267,   271
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_LBRACKET", "T_RBRACKET", "T_LABRACKET", 
  "T_RABRACKET", "T_LBRACE", "T_RBRACE", "T_COLON", "T_COMMA", 
  "T_NEWLINE", "T_EQUALS", "T_SEMI", "T_PIPE", "T_STAR", "T_LPAREN", 
  "T_RPAREN", "KW_OBJECT", "KW_COMPONENT", "KW_TEMPLATE", "KW_FLOAT", 
  "KW_INT", "KW_STRING", "KW_LIST", "KW_IN", "KW_BOOL", "KW_TRUE", 
  "KW_FALSE", "T_STRING", "T_NAME", "T_INT", "T_FLOAT", "$accept", "file", 
  "file_body", "templat", "template_body", "template_item", 
  "template_item_type", "optional_required", "optional_list", 
  "optional_template_item_default", "optional_template_item_constr", 
  "template_item_constr", "defval", "defval_body", "componentval", 
  "optional_parent", "optional_name", "keyval", "value_list", 
  "value_list_body0", "value_list_body1", "value", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    33,    34,    35,    35,    35,    36,    37,    37,    38,
      39,    39,    39,    39,    39,    40,    40,    41,    41,    42,
      42,    43,    43,    44,    44,    44,    44,    45,    46,    46,
      46,    47,    47,    48,    48,    49,    49,    50,    51,    51,
      52,    53,    53,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     2,     0,     7,     2,     0,     8,
       1,     1,     1,     1,     1,     1,     0,     1,     0,     0,
       2,     2,     0,     3,     3,     3,     1,     8,     2,     2,
       0,     7,     4,     2,     0,     1,     0,     4,     3,     2,
       2,     2,     0,     1,     1,     1,     1,     1,     1,     1,
       5,     7,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       5,     0,     0,     2,     5,     5,     0,     0,     1,     4,
       3,     0,     0,     0,    34,     8,     0,     0,    15,     0,
       8,     0,    33,    30,     6,     7,     0,     0,     0,     0,
      30,    30,     0,    36,     0,    27,    28,    29,    14,    11,
      10,    12,    13,    18,    35,     0,     0,     0,    48,    49,
      43,    46,    44,    45,    47,     0,    17,    19,    32,    39,
       0,    42,     0,    37,     0,    22,    30,    38,     0,    40,
       0,    20,     0,     0,     0,    41,     0,     0,    21,    26,
       9,    31,    50,     0,     0,     0,     0,     0,    24,    23,
      25,    51,     0,     0,    52
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     2,     3,     4,    19,    20,    43,    21,    57,    65,
      73,    78,     5,    29,    30,    17,    45,    31,    54,    60,
      69,    79
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -32
static const yysigned_char yypact[] =
{
       5,   -11,    20,   -32,     5,     5,   -17,    -7,   -32,   -32,
     -32,    33,    50,    18,    55,    -9,    35,    59,   -32,    60,
      -9,    39,   -32,     2,   -32,   -32,    61,    41,    62,    64,
       2,     2,    36,    43,    -1,   -32,   -32,   -32,   -32,   -32,
     -32,   -32,   -32,    51,   -32,    72,    13,    45,   -32,   -32,
     -32,   -32,   -32,   -32,   -32,    65,   -32,    67,    73,   -32,
      77,    74,    75,   -32,    -1,    58,     2,   -32,    -1,   -32,
      54,   -32,    19,    69,    79,   -32,    28,    19,    -2,   -32,
     -32,   -32,   -32,    56,    -3,    19,    19,    57,   -32,   -32,
      80,   -32,    63,    83,   -32
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -32,   -32,    49,   -32,    71,   -32,   -32,   -32,   -32,   -32,
     -32,   -25,   -32,   -30,   -32,   -32,   -32,   -32,   -32,    24,
     -32,   -31
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -17
static const yysigned_char yytable[] =
{
      36,    37,    46,    55,    47,    27,    18,    85,    85,     6,
       1,    86,    86,    11,    88,    61,    46,    59,    47,     7,
       8,   -16,    46,    12,    47,    15,    48,    49,    50,    51,
      52,    53,    28,    71,    82,    77,    74,    61,    83,    13,
      48,    49,    50,    51,    52,    53,    48,    49,    50,    51,
      52,    53,    84,     9,    10,    38,    14,    39,    40,    41,
      89,    90,    42,    91,    16,    22,    23,    92,    24,    26,
      32,    33,    35,    44,    34,    56,    58,    62,    63,    64,
      66,    67,    80,    72,    68,    70,    76,    81,    87,    94,
      85,    25,    75,     0,     0,    93
};

static const yysigned_char yycheck[] =
{
      30,    31,     3,    34,     5,     3,    15,    10,    10,    20,
       5,    14,    14,    30,    17,    46,     3,     4,     5,    30,
       0,    30,     3,    30,     5,     7,    27,    28,    29,    30,
      31,    32,    30,    64,     6,    16,    66,    68,    10,     6,
      27,    28,    29,    30,    31,    32,    27,    28,    29,    30,
      31,    32,    77,     4,     5,    19,     6,    21,    22,    23,
      85,    86,    26,     6,     9,    30,     7,    10,     8,    30,
       9,    30,     8,    30,    12,    24,     4,    32,    13,    12,
       7,     4,    13,    25,    10,    10,    32,     8,    32,     6,
      10,    20,    68,    -1,    -1,    32
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     5,    34,    35,    36,    45,    20,    30,     0,    35,
      35,    30,    30,     6,     6,     7,     9,    48,    15,    37,
      38,    40,    30,     7,     8,    37,    30,     3,    30,    46,
      47,    50,     9,    30,    12,     8,    46,    46,    19,    21,
      22,    23,    26,    39,    30,    49,     3,     5,    27,    28,
      29,    30,    31,    32,    51,    54,    24,    41,     4,     4,
      52,    54,    32,    13,    12,    42,     7,     4,    10,    53,
      10,    54,    25,    43,    46,    52,    32,    16,    44,    54,
      13,     8,     6,    10,    44,    10,    14,    32,    17,    44,
      44,     6,    10,    32,     6
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 115 "OL/ol.y"
    { ol_root = yyvsp[0].astnode; ;}
    break;

  case 3:
#line 118 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_LINK, OL_AST_DEF, yyvsp[-1].astnode, yyvsp[0].astnode); ;}
    break;

  case 4:
#line 119 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_LINK, OL_AST_TEMPLATE, yyvsp[-1].astnode, yyvsp[0].astnode); ;}
    break;

  case 5:
#line 120 "OL/ol.y"
    { yyval.astnode = NULL; ;}
    break;

  case 6:
#line 128 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_TEMPLATE, yyvsp[-4].name, yyvsp[-1].astnode); ;}
    break;

  case 7:
#line 132 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_LINK, OL_AST_TEMPLATEITEM, yyvsp[-1].astnode, yyvsp[0].astnode); ;}
    break;

  case 8:
#line 133 "OL/ol.y"
    { yyval.astnode = NULL; ;}
    break;

  case 9:
#line 141 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_TEMPLATEITEM, yyvsp[-6].name, yyvsp[-4].intval, (yyvsp[-7].intval | yyvsp[-3].intval), yyvsp[-2].astnode, yyvsp[-1].astnode); ;}
    break;

  case 10:
#line 145 "OL/ol.y"
    { yyval.intval = OL_VAL_INT; ;}
    break;

  case 11:
#line 146 "OL/ol.y"
    { yyval.intval = OL_VAL_FLOAT; ;}
    break;

  case 12:
#line 147 "OL/ol.y"
    { yyval.intval = OL_VAL_STRING; ;}
    break;

  case 13:
#line 148 "OL/ol.y"
    { yyval.intval = OL_VAL_BOOL; ;}
    break;

  case 14:
#line 149 "OL/ol.y"
    { yyval.intval = OL_VALX_COMPONENT; ;}
    break;

  case 15:
#line 153 "OL/ol.y"
    { yyval.intval = OL_TMOD_REQUIRED; ;}
    break;

  case 16:
#line 154 "OL/ol.y"
    { yyval.intval = 0; ;}
    break;

  case 17:
#line 158 "OL/ol.y"
    { yyval.intval = OL_TMOD_LIST; ;}
    break;

  case 18:
#line 159 "OL/ol.y"
    { yyval.intval = 0; ;}
    break;

  case 19:
#line 163 "OL/ol.y"
    { yyval.astnode = NULL; ;}
    break;

  case 20:
#line 165 "OL/ol.y"
    { yyval.astnode = yyvsp[0].astnode; ;}
    break;

  case 21:
#line 169 "OL/ol.y"
    { yyval.astnode = yyvsp[0].astnode; ;}
    break;

  case 22:
#line 170 "OL/ol.y"
    { yyval.astnode = NULL; ;}
    break;

  case 23:
#line 175 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_TEMPLATECONSTR, OL_TCONSTR_RANGE, yyvsp[-2].astnode, yyvsp[0].astnode); ;}
    break;

  case 24:
#line 177 "OL/ol.y"
    { yyval.astnode = yyvsp[-1].astnode; ;}
    break;

  case 25:
#line 180 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_TEMPLATECONSTR, OL_TCONSTR_ALT, yyvsp[-2].astnode, yyvsp[0].astnode); ;}
    break;

  case 26:
#line 183 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_TEMPLATECONSTR, OL_TCONSTR_LIT, yyvsp[0].astnode); ;}
    break;

  case 27:
#line 191 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_DEF, yyvsp[-6].name, yyvsp[-5].name, yyvsp[-3].astnode, yyvsp[-1].astnode); ;}
    break;

  case 28:
#line 196 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_LINK, OL_AST_DEF, yyvsp[-1].astnode, yyvsp[0].astnode); ;}
    break;

  case 29:
#line 199 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_LINK, OL_AST_KEYVAL, yyvsp[-1].astnode, yyvsp[0].astnode); ;}
    break;

  case 30:
#line 201 "OL/ol.y"
    { yyval.astnode = NULL; ;}
    break;

  case 31:
#line 207 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_DEF, yyvsp[-5].name, yyvsp[-4].astnode, 0, yyvsp[-1].astnode); ;}
    break;

  case 32:
#line 210 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_DEF, yyvsp[-2].name, yyvsp[-1].astnode, 0, 0); ;}
    break;

  case 33:
#line 214 "OL/ol.y"
    { yyval.astnode = yyvsp[0].name; ;}
    break;

  case 34:
#line 215 "OL/ol.y"
    { yyval.astnode = NULL; ;}
    break;

  case 35:
#line 219 "OL/ol.y"
    { yyval.astnode = yyvsp[0].name; ;}
    break;

  case 36:
#line 220 "OL/ol.y"
    { yyval.astnode = NULL; ;}
    break;

  case 37:
#line 225 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_KEYVAL, yyvsp[-3].name, yyvsp[-1].astnode); ;}
    break;

  case 38:
#line 230 "OL/ol.y"
    { yyval.astnode = yyvsp[-1].astnode; ;}
    break;

  case 39:
#line 232 "OL/ol.y"
    { yyval.astnode = NULL; ;}
    break;

  case 40:
#line 237 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_LINK, OL_AST_VALUE, yyvsp[-1].astnode, yyvsp[0].astnode); ;}
    break;

  case 41:
#line 242 "OL/ol.y"
    { yyval.astnode = yyvsp[0].astnode; ;}
    break;

  case 42:
#line 243 "OL/ol.y"
    { yyval.astnode = NULL; ;}
    break;

  case 43:
#line 247 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_VALUE, OL_VAL_STRING, yyvsp[0].stringlit); ;}
    break;

  case 44:
#line 248 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_VALUE, OL_VAL_INT, yyvsp[0].intlit); ;}
    break;

  case 45:
#line 249 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_VALUE, OL_VAL_FLOAT, yyvsp[0].floatlit); ;}
    break;

  case 46:
#line 250 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_VALUE, OL_VAL_STRING, yyvsp[0].name); ;}
    break;

  case 47:
#line 252 "OL/ol.y"
    { {
  void* list = ol_ast_node(OL_AST_VALUE, OL_VAL_LIST, yyvsp[0].astnode);
  if(!ol_validate_list(list, NULL))
    YYABORT;
  yyval.astnode = list;
  }
;}
    break;

  case 48:
#line 259 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_VALUE, OL_VAL_BOOL, -1); ;}
    break;

  case 49:
#line 260 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_VALUE, OL_VAL_BOOL, 0); ;}
    break;

  case 50:
#line 264 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_VALUE, OL_VAL_VEC2, yyvsp[-3].floatlit, yyvsp[-1].floatlit); ;}
    break;

  case 51:
#line 268 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_VALUE, OL_VAL_VEC3, yyvsp[-5].floatlit, yyvsp[-3].floatlit, yyvsp[-1].floatlit); ;}
    break;

  case 52:
#line 272 "OL/ol.y"
    { yyval.astnode = ol_ast_node(OL_AST_VALUE, OL_VAL_VEC4, yyvsp[-7].floatlit, yyvsp[-5].floatlit, yyvsp[-3].floatlit, yyvsp[-1].floatlit); ;}
    break;


    }

/* Line 999 of yacc.c.  */
#line 1435 "OL/ol_yacc.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 115 "OL/ol.y"


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



