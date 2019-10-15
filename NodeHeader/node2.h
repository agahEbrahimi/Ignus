#include <iostream>
#include <vector>
#include <string>

using std::vector; 
using std::string;

class Statement;
class BaseStatement;
class Class;
class Expression;
class Declaration; 

class ProgramBlock; //CHANGED: added this and following definitions
class Identifier;
class Modifier;

enum Boolean {
    False,
    True
};
enum CTRLStmt {
    IF,
    ELIF,
    ELSE,
    SWITCH,
    CASE,
    FOR,
    WHILE
};

enum binOper{
    checkEqual,
    checkNotEqual,
    lessThan,
    lessEqThan,
    greaterThan, 
    greaterEqThan,
    typeOf,
    notTypeOf,
    plus,
    minus, 
    multi,
    divide, 
    power, 
    mod
};

enum uniOper{
    lBitwise,
    rBitwise,
    increment,
    decrement
};

enum modifiers{
    Public, 
    Private, 
    Protected,
    Static, 
    Final, 
    Abstract,
    Interface
};

typedef vector<Statement*> stmtList;
typedef vector<Class*> clssList;
typedef vector<BaseStatement*> baseStmtList;
typedef vector<Expression*> expList;
typedef vector<Declaration*> declGroup;

class Node{
    public: 
        virtual ~Node() {};
};

//CHANGED: Moved Modifier to here
class Modifier : public Node {
    public:
        vector<modifiers> modif; //CHANGED: from int to modifiers
        Modifier() {}
};

class BaseStatement : public Node {};

class Statement : public BaseStatement {};

class Expression : public Statement {};

//CHANGED: Order of block and programBlock
class ProgramBlock : public BaseStatement {
    public:
        baseStmtList baseStatements;
        ProgramBlock() {}
};

class Program : public Node{
    public:
        ProgramBlock& programBlock;
        Program(ProgramBlock& programBlock) : programBlock(programBlock) {} //CHANGED: made constructor have pointer to match variable
};


class Block : public Expression {
    public:
        stmtList statements; 
        Block() {}
};

class EnhancedBlock : public BaseStatement {
    public:
        baseStmtList statements;
        EnhancedBlock() {}
};

class Class : public BaseStatement {
    public: 
        EnhancedBlock& block;
        Identifier& id;
        Modifier mod;

        Class(EnhancedBlock& block, Identifier& id): block(block) , id(id) {}
        Class(EnhancedBlock& block, Identifier& id, Modifier mod): block(block) , id(id), mod(mod) {}
};

class Integer : public Expression {
    public:
        int num; 
        Integer(int num) : num(num) {}
};

class Double : public Expression {
    public:
        double num; 
        Double(double num) : num(num) {}
};

class String : public Expression {
    public:
        string str; 
        String(string str) : str(str) {}
};

class Char : public Expression {
    public:
        char ch; 
        Char(char ch) : ch(ch) {}
};

class Bool : public Expression {
    public:
        bool boolean; 
        Bool(bool boolean) : boolean(boolean) {} 
};

class UnaryOperator : public Expression {
    public:
        Expression& operant;
        int oper;
        UnaryOperator(Expression& operant, int oper) : operant(operant), oper(oper){}       
};

class BinaryOperator : public Expression {
    public:
        Expression& rhs;
        Expression& lhs;
        int oper;
        BinaryOperator(Expression& rhs, Expression& lhs, int oper) : rhs(rhs), lhs(lhs), oper(oper) {}
};

class DeclGroup : public Expression {
    public: 
        declGroup arguments; 
        DeclGroup() {}
};

class Declaration : public Statement {
    public:
        const Identifier& type;
        Identifier& id; 
        Modifier mod;

        Declaration(Modifier mod, const Identifier& type, Identifier& id) : mod(mod), type(type), id(id) {} 
        Declaration(const Identifier& type, Identifier& id) :  type(type), id(id) {} 
};

class DeclWAssign : public Statement {
    public:
        const Identifier& type;
        Identifier& id; 
        Modifier mod;
        Expression& val;
        DeclWAssign(Modifier mod, const Identifier& type, Identifier& id, Expression& val) : mod(mod), type(type), id(id), val(val) {}    
        DeclWAssign(const Identifier& type, Identifier& id, Expression& val) : type(type), id(id), val(val) {}   
};

class Assignment : public Expression {
    public:
        Expression& lhs;
        Expression& rhs;
        Assignment(Expression& lhs, Expression& rhs) : lhs(lhs), rhs(rhs) {}
};

class Identifier : public Expression {
    public:
        string name; 
        Identifier(const string& name) : name(name) {}
};

class Control: public Expression {
    public:
        Control() {}
};

class ControlStatements : public Control {
    public: 
        Expression& condition;
        CTRLStmt type; //CHANGED: from int to CTRLStmt
        Block& block; 
        ControlStatements(Expression& condition, CTRLStmt type, Block& block) : condition(condition), type(type), block(block) {} 

};

class ControlLoop : public Control {
    public: 
        Statement& preCond;
        Expression& condition;
        CTRLStmt type; //CHANGED:
        Block& block; 
        Expression& postAct;
        ControlLoop(CTRLStmt type, Statement& preCond, Expression& condition, Block& block, Expression& postAct) : preCond(preCond), postAct(postAct), condition(condition), type(type), block(block) {} 

};

class SecControlStatements : public Control {
    public: 
        Expression* condition; //CHANGED: to other pointer
        CTRLStmt type; //CHANGED:
        Block& block; 
        Control& proceeding;
        SecControlStatements(Expression* condition, CTRLStmt type, Block& block, Control& proceeding) : condition(condition), type(type), block(block), proceeding(proceeding) {} 
};

class ExpressionStatement : public Statement {
    public:
        Expression& expression;
        ExpressionStatement(Expression& expression) : expression(expression) { }
};


class EmpMethodDecl : public BaseStatement {
    public:
        Block& block;
        const Identifier& type; 
        Identifier& id; 
        Modifier mod;
        DeclGroup arguments;
        EmpMethodDecl(Block& block, const Identifier& type, Identifier& id, DeclGroup arguments) : block(block), id(id), type(type), arguments(arguments) {}
        EmpMethodDecl(Modifier mod, Block& block, const Identifier& type, Identifier& id, DeclGroup arguments) : mod(mod), block(block), id(id), type(type), arguments(arguments) {}
};

class MethodDecl : public BaseStatement {
    public:
        Block& block;
        const Identifier& type; 
        Identifier& id; 
        Modifier mod; 

        MethodDecl(Block& block, const Identifier& type, Identifier& id) : block(block), id(id), type(type) {}
        MethodDecl(Modifier mod, Block& block, const Identifier& type, Identifier& id) : mod(mod), block(block), id(id), type(type) {}
    
};

class MethodCall : public Expression {
    public:
        const Identifier& id;
        DeclGroup arguments;
        expList superExpression;
        MethodCall(const Identifier& id, DeclGroup arguments) : id(id), arguments(arguments) {}
};

class EmpMethodCall : public Expression {
    public:
        const Identifier& id;
        expList superExpression;
        EmpMethodCall(const Identifier& id) : id(id) {}
};