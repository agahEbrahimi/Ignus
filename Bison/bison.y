%{
    #include "node.h"
    EnhancedBlock *programBlock;

    extern int yylex();
    void yyerror(const char *s) { printf("ERROR: %sn", s); }
%}

%union {
      Node *node;
      Modifier *modif; 
      BaseStatement *basestmt;
      Statement *stmt;
      Expression *expr;
      ProgramBlock  *prgmblock;
      Program *program;
      Block *block;
      EnhancedBlock *eblock;
      Class *cls;
      DeclGroup *declGroup;
      Declaration *var_decl;
      DeclWAssign *var_declWAssign;
      Assignment *assign;
      Identifier *ident;
      Control *control;
      ControlStatements *controlStmt;
      ControlLoop *controlLoop;
      SecControlStatements *seccontrolstmt;
      ExpressionStatement *exprstmt;
      EmpMethodDecl *emthddecl;
      MethodDecl *mthddecl;
      MethodCall *mthdcall;
      EmpMethodCall *emthdcalls
      typedef std::vector<Statement*> stmtVec;
      typedef std::vector<Class*> clssVec;
      typedef std::vector<BaseStatement*> baseStmtVec;
      typedef std::vector<Expression*> expVec;
      typedef std::vector<Declaration*> declVec;
      std::string *string;
      int *Integer;
      double *Double
      char *char; 
      boolean *Bool;
      int token;
}


%token <string> TIDENTIFIER TINTEGER TDOUBLE TSTRING TTRUE TFALSE
%token <token> TCEQUAL TCNEQUAL TCLESS TCLESSE TCGREATER TCGREATERE TIOF TNIOF
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TLBRACKET TRBRACKET TCOMMA TDOT TCOLON
%token <token>  TASSIGN TMODA TADDA TMINUSA TMULTIA TPOWERA TDIVA 
%token <token> TPLUS TMINUS TMUL TDIV TPOWER  TMOD 
%token <token> TNOT TAND TOR
%token <token> TINC TDEC TLBW TRBW
%token <token> TBI TII TFI TDI TLI TSHI TSI TVI TOI TCI TVOID TBOOL
%token <token> TCI TENUM TIMPORT TRET TEXTE TIMP TPACKAGE
%token <token> TPUI TPRI TPOI TSTAT TFIN TSYNCH TABS TINTER
%token <token> TTHIS TSUPER TNEW TTHR 
%token <token> TFOR TWHILE TDO TCASE TSWITCH TIF TCONT TBREAK TTRY TCATCH TELSE TELSEIF

%type <ident> ident
%type <expr> value expr 
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> stmts block
%type <eblock> program class method eblock
%type <stmt> stmt var_decl func_decl
%type <token> comparison

%left TPLUS TMINUS
%left TMUL TDIV
%left TPOWER TMOD


%start program

%%
program : prgmblock {programBlock = new Program($1);};

prgmblock : basestmt {$$ = new ProgramBlock(); $$->baseStatements.push_back($<basestmt>1);}
      | prgmblock basestmt {$1->baseStatements.push_back($<basestmt>2);}

basestmt : cls 
      |    mthd
      | stmts;

cls : TCI ident eblock {$$ = new Class(*$3, *$2);} 
      | modif TCI ident eblock {$$ = new Class(*$4, *$3, $1);};

ebstmt : mthd {$$ = new EnhancedBlock(); $$ -> statements.push_back($<mthd>1);} ##make this with the vector only and not the EnhancedBlock itself
      |  stmts {$$ = new EnhancedBlock(); $$ -> statements.push_back($<stmts>1);}
      | ebstmt ebstmt {$1->statements.push_back($<ebstmt>2)''};


mthd : ident ident TLPAREN TRPAREN block {$$ = new EmpMethodDecl(*$5, *$1, *$2);}
      | modif ident ident TLPAREN TRPAREN block {$$ = new EmpMethodDecl($1, *$6, *$2, *$3);}
      | ident ident TLPAREN var_group TRPAREN block {$$ = new MethodDecl(*$6, *$1, *$2, $4);}
      | modif ident ident TLPAREN var_group TRPAREN block {$$ = new MethodDecl($1, *$7, *$2, *$3, $5);};

stmts : stmt { $$ = new Block(); $$->statements.push_back($<stmt>1); }
      | stmts stmt { $1->statements.push_back($<stmt>2); }
      ;

stmt : var_decl
     | expr { $$ = new ExpressionStatement(*$1); }
     | ctrlstmt;  

eblock : TLBRACE ebstmt TRBRACE {$$=$2;}
      |  TLBRACE TRBRACE { $$ = new EnhancedBlock(); };

block : TLBRACE stmts TRBRACE { $$ = $2; }
      | TLBRACE TRBRACE { $$ = new Block(); };

var_group : ident ident { $$ = new DeclGroup(); $$->arguments.push_back(new Declaration(*$1, *$2)); }
      | var_group TCOMMA ident ident {$1->arguments.push_back(new Declaration(*$3, *$4));};

var_decl : ident ident TENDL{ $$ = new Declaration(*$1, *$2); }
      | modif ident ident TENDL{$$ = new Declaration($1, *$2, *$3); } ;

var_decl_assign : ident dent TASSIGN expr TENDL {$$ = new DeclWAssign(*$1, *2$, *$4);}
      | modif ident ident TASSIGN expr TENDL {$$ = new DeclWAssign($1, *$2, *$3, *$5);};

var_ctrl = var_decl
      |    var_decl_assign
      | expr TENDL;

ident : TIDENTIFIER { $$ = new Identifier(*$1); delete $1; };

conditionalStmt : TIF TLPAREN expr TRPAREN block {$$ = new ControlStatements(*$3, $1, *$5);};

conditionalElif : conditionalStmt
      |     conditionalElif TELSEIF TLPAREN expr TRPAREN block {$$ = new SecControlStatements(*$4, $1, *$6, *$1)};

conditionalElse : conditionalElif
      |     conditionalElif TELSE TLPAREN expr TRPAREN block {$$ = new SecControlStatements(*$4, $1, *$6, *$1)};   

ctrlstmt : conditionalElse    
      |    TFOR TLPAREN var_ctrl expr expr TRPAREN block {$$ = new ControlLoop($1, *$2, *$3, *$7, *$5);}
      |    TWHILE TLPAREN expr TRPAREN block{$$ = new ControlStatements(*$3, $1, *$5);};

modif : modi {$$ = $1}
      | modif modi {$$->modif.push_back($2);};


modi :    TPUI {$$ = new Modifier{}; $$->modif.push_back($1);} 
      |    TPRI {$$ = new Modifier{}; $$->modif.push_back($1);} 
      |    TPOI {$$ = new Modifier{}; $$->modif.push_back($1);} 
      |    TSTAT {$$ = new Modifier{}; $$->modif.push_back($1);} 
      |    TFIN {$$ = new Modifier{}; $$->modif.push_back($1);} 
      |    TABS {$$ = new Modifier{}; $$->modif.push_back($1);} 
      |    TINTER {$$ = new Modifier{}; $$->modif.push_back($1);} 
      ;

numeric : TINTEGER { $$ = new Integer(atol($1->c_str())); delete $1; }
      |   TDOUBLE { $$ = new Double(atof($1->c_str())); delete $1; }
      ;

char : TCHAR {$$ = new Char($1->c_str()); delete $1;}
      ;

str : TSTRING {$$ = new String($1->c_str()); delete $1;}
      ;
    

expr_callable : ident { $<ident>$ = $1; }
     | ident TLPAREN var_group TRPAREN{ $$ = new MethodCall(*$1, *$3); delete $3; }
     | ident TLPAREN { $$ = new EmpMethodCall(*$1);}
     | expr_callable TDOT expr_callable {$3->superExpression.push_back($1); delete $1;}
     ;

expr : ident TEQUAL expr { $$ = new Assignment(*$<ident>1, *$3); }
     | TLPAREN expr TRPAREN {$$ = $2;}
     | expr_callable
     | numeric
     | str
     | char
     | expr operator expr { $$ = new BinaryOperator(*$1, *$3, $2); }
     | expr uni_operator {$$ = UnaryOperator(*$1, $2); }
     | TNOT expr {$$ = UnaryOperator(*$2, $1); }
     ;

operator : TCEQ | TCNE | TCLT | TCLE | TCGT | TCGE | TIOF | TNIOF;
           | TPLUS | TMINUS | TMUL | TDIV | TPOWER | TMOD;

uni_operator : TLBW |  TRBW | TINC | TDEC

%%