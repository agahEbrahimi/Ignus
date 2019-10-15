/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TIDENTIFIER = 258,
    TINTEGER = 259,
    TDOUBLE = 260,
    TSTRING = 261,
    TTRUE = 262,
    TFALSE = 263,
    TCHAR = 264,
    TENDL = 265,
    TCEQUAL = 266,
    TCNEQUAL = 267,
    TCLESS = 268,
    TCLESSE = 269,
    TCGREATER = 270,
    TCGREATERE = 271,
    TIOF = 272,
    TNIOF = 273,
    TLPAREN = 274,
    TRPAREN = 275,
    TLBRACE = 276,
    TRBRACE = 277,
    TLBRACKET = 278,
    TRBRACKET = 279,
    TCOMMA = 280,
    TDOT = 281,
    TCOLON = 282,
    TASSIGN = 283,
    TMODA = 284,
    TADDA = 285,
    TMINUSA = 286,
    TMULTIA = 287,
    TPOWERA = 288,
    TDIVA = 289,
    TPLUS = 290,
    TMINUS = 291,
    TMUL = 292,
    TDIV = 293,
    TPOWER = 294,
    TMOD = 295,
    TNOT = 296,
    TAND = 297,
    TOR = 298,
    TINC = 299,
    TDEC = 300,
    TLBW = 301,
    TRBW = 302,
    TBI = 303,
    TII = 304,
    TFI = 305,
    TDI = 306,
    TLI = 307,
    TSHI = 308,
    TSI = 309,
    TVI = 310,
    TOI = 311,
    TCI = 312,
    TVOID = 313,
    TBOOL = 314,
    TENUM = 315,
    TIMPORT = 316,
    TRET = 317,
    TEXTE = 318,
    TIMP = 319,
    TPACKAGE = 320,
    TPUI = 321,
    TPRI = 322,
    TPOI = 323,
    TSTAT = 324,
    TFIN = 325,
    TSYNCH = 326,
    TABS = 327,
    TINTER = 328,
    TTHIS = 329,
    TSUPER = 330,
    TNEW = 331,
    TTHR = 332,
    TFOR = 333,
    TWHILE = 334,
    TDO = 335,
    TCASE = 336,
    TSWITCH = 337,
    TIF = 338,
    TCONT = 339,
    TBREAK = 340,
    TTRY = 341,
    TCATCH = 342,
    TELSE = 343,
    TELSEIF = 344
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 9 "parser.y" /* yacc.c:1909  */

      Node *node;
      Modifier modif; 
      BaseStatement *basestmt;
      Statement *stmt;
      Expression *expr;
      ProgramBlock  *prgmblock;
      Program *program;
      Block *block;
      EnhancedBlock *eblock;
      Class *cls;
      DeclGroup declGroup;
      Declaration *var_decl;
      DeclWAssign *var_declWAssign;
      Assignment *assign;
      Identifier *ident;
      Control *control;
      ControlStatements *controlStmt;
      ControlLoop *controlLoop;
      SecControlStatements *seccontrolstmt;
      ExpressionStatement *exprstmt;
      MethodStatement *mthd;
      Block *stmts;
      std::vector<BaseStatement*> *baseStmtVec;
      std::vector<Expression*> *expVec;
      std::vector<Declaration*> *declVec;
      std::string *string;
      int *Integer;
      double *Double;
      char *Char; 
      bool *Bool;
      int token;


#line 179 "parser.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
