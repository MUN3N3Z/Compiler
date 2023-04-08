/* A Bison parser, made by GNU Bison 3.7.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */

  typedef void* yyscan_t;


/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TOKEN_TYPE_LONG = 258,         /* "long"  */
    TOKEN_RETURN = 259,            /* "return"  */
    TOKEN_SEMI = 260,              /* ";"  */
    TOKEN_COMMA = 261,             /* ","  */
    TOKEN_LPAREN = 262,            /* "("  */
    TOKEN_RPAREN = 263,            /* ")"  */
    TOKEN_LBRACE = 264,            /* "{"  */
    TOKEN_RBRACE = 265,            /* "}"  */
    TOKEN_PLUS = 266,              /* "+"  */
    TOKEN_MINUS = 267,             /* "-"  */
    TOKEN_STAR = 268,              /* "*"  */
    TOKEN_DIV = 269,               /* "/"  */
    TOKEN_ASSIGN = 270,            /* "="  */
    TOKEN_BOR = 271,               /* "|"  */
    TOKEN_BAND = 272,              /* "&"  */
    TOKEN_BXOR = 273,              /* "^"  */
    TOKEN_SHR = 274,               /* ">>"  */
    TOKEN_SHL = 275,               /* "<<"  */
    TOKEN_NUMBER = 276,            /* "constant value"  */
    TOKEN_ID = 277                 /* "variable name"  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{

    long value;
    char* str;
    Node* node;
    NodeList* nodelist;
    OpType opcode;
    list_t* symel;


};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (NodeList **funcdecls, int* errorCount, SymTable* symtable, yyscan_t scanner);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
