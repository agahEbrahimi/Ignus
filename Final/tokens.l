%{
#include <string>
#include "node.h"
#include "parser.hpp"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
extern "C" int yywrap() { }
%}

%%

[ \t\n]                 ;
[a-zA-Z_][a-zA-Z0-9_]*  SAVE_TOKEN; return TIDENTIFIER;
[0-9]+.[0-9]*           SAVE_TOKEN; return TDOUBLE;
[0-9]+                  SAVE_TOKEN; return TINTEGER;
"true"                  SAVE_TOKEN; return TTRUE;
"false"                 SAVE_TOKEN; return TFALSE;
"null"                  SAVE_TOKEN; return TNULL;
";"$                    return TOKEN(TENDL);
":"                     return TOKEN(TCOLON);
"="                     return TOKEN(TASSIGN);
"=="                    return TOKEN(TCEQUAL);
"!="                    return TOKEN(TCNEQUAL);
"<"                     return TOKEN(TCLESS);
"<="                    return TOKEN(TCLESSE);
">"                     return TOKEN(TCGREATER);
">="                    return TOKEN(TCGREATERE);
"("                     return TOKEN(TLPAREN);
")"                     return TOKEN(TRPAREN);
"{"                     return TOKEN(TLBRACE);
"}"                     return TOKEN(TRBRACE);
"["                     return TOKEN(TLBRACKET);
"]"                     return TOKEN(TRBRACKET);
"."                     return TOKEN(TDOT);
","                     return TOKEN(TCOMMA);
"+"                     return TOKEN(TPLUS);
"-"                     return TOKEN(TMINUS);
"*"                     return TOKEN(TMUL);
"/"                     return TOKEN(TDIV);
"^"                     return TOKEN(TPOWER);
"!"                     return TOKEN(TNOT);
"%"                     return TOKEN(TMOD);
"%="                    return TOKEN(TMODA);
"+="                    return TOKEN(TADDA);
"-="                    return TOKEN(TMINUSA);
"*="                    return TOKEN(TMULTIA);
"^="                    return TOKEN(TPOWERA);
"/="                    return TOKEN(TDIVA);
">>"                    return TOKEN(TLBW);
"<<"                    return TOKEN(TRBW);
"++"                    return TOKEN(TINC);
"--"                    return TOKEN(TDEC);
"&&"                    return TOKEN(TAND);
"||"                    return TOKEN(TOR);
"==="                   return TOKEN(TIOF);
"!=="                   return TOKEN(TNIOF)
"byte"                  return TOKEN(TBI);
"int"                   return TOKEN(TII);
"float"                 return TOKEN(TFI);
"double"                return TOKEN(TDI);
"long"                  return TOKEN(TLI);
"short"                 return TOKEN(TSHI);
"String"                return TOKEN(TSI);
"var"                   return TOKEN(TVI);
"Object"                return TOKEN(TOI);
"class"                 return TOKEN(TCI);
"public"                return TOKEN(TPUI);
"private"               return TOKEN(TPRI);
"protected"             return TOKEN(TPOI);
"static"                return TOKEN(TSTAT);
"final"                 return TOKEN(TFIN); 
"this"                  return TOKEN(TTHIS);
"super"                 return TOKEN(TSUPER);
"new"                   return TOKEN(TNEW);
"char"                  return TOKEN(TCI);
"for"                   return TOKEN(TFOR);
"while"                 return TOKEN(TWHILE);
"do"                    return TOKEN(TDO);
"case"                  return TOKEN(TCASE);
"switch"                return TOKEN(TSWITCH);
"enum"                  return TOKEN(TENUM);
"void"                  return TOKEN(TVOID);
"boolean"               return TOKEN(TBOOL);    
"import"                return TOKEN(TIMPORT);   
"if"                    return TOKEN(TIF);
"continue"              return TOKEN(TCONT);     
"break"                 return TOKEN(TBREAK);  
"return"                return TOKEN(TRET);  
"extends"               return TOKEN(TEXTE);
"implements"            return TOKEN(TIMP);
"throw"                 return TOKEN(TTHR);
"package"               return TOKEN(TPACKAGE);
"synchronized"          return TOKEN(TSYNCH);
"try"                   return TOKEN(TTRY);
"catch"                 return TOKEN(TCATCH);
"abstract"              return TOKEN(TABS);
"interface"             return TOKEN(TINTER);
"else"                  return TOKEN(TELSE);
"elseif"                return TOKEN(TELSEIF);

<INITIAL>"/*"           {yymore(); BEGIN(COMMENT_MULTI);}

<INITIAL>["]            {yymore(); BEGIN(STRING_MULTI);}

<STRING_MULTI>[^"]+     {yymore(); }

<STRING_MULTI>["]       {SAVE_TOKEN; return TSTRING; BEGIN(INITIAL); }

<STRING_MULTI><<EOF>>   {printf("Unterminated String: %s\n",yytext); yyterminate();}


<CHAR_MULTI>[^']+     {yymore(); }

<CHAR_MULTI>[']       {SAVE_TOKEN; return TCHAR; BEGIN(INITIAL); }

<CHAR_MULTI><<EOF>>   {printf("Unterminated String: %s\n",yytext); yyterminate();}


<COMMENT_MULTI>"*/"     {BEGIN(INITIAL); }

<COMMENT_MULTI>.        {yymore();} 

<COMMENT_MULTI>\n       {yymore();}

<COMMENT_MULTI><<EOF>>  {printf("Unterminated Comment: %s\n", yytext); yyterminate();}

.                       printf("Unknown token!n"); yyterminate();

%%




