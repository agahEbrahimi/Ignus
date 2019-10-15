//#pragma once
#include <iostream>
#include <vector>
#include <string>

using std::vector; 
using std::string;

class Statement;
class BaseStatement;
class Expression;
class VariableDecl; 
class Identifier; //Added
class Modifier; //Added

enum Boolean {
    False,
    True
};

typedef vector<Statement*> stmtList;
typedef vector<BaseStatement*> baseStmtList;
typedef vector<Expression*> expList;
typedef vector<VariableDecl*> VariableDecls; //Changed from VariableDecl tp VariableDecls

class Node{
    public: 
        virtual ~Node() {};
};

class BaseStatement : public Node {};

class Statement : public BaseStatement {};

class Expression : public Statement {};

class Program : public Node{};

class Block : public Expression {
    stmtList statements; 
    Block() {}
};

class EnhancedBlock : public Expression {
    baseStmtList statements;
    EnhancedBlock() {}
};

class Class : public Node {
    EnhancedBlock& block;
    Identifier& id;

    Class(EnhancedBlock& block, Identifier& id): block(block) , id(id) {}   
};

class Integer : public Expression {
    int num; 
    Integer(int num) : num(num) {}
};

class Double : public Expression {
    double num; 
    Double(double num) : num(num) {}
};

class String : public Expression {
    string str; 
    String(string str) : str(str) {}
};

class Bool : public Expression {
    bool boolean; 
    Bool(bool boolean) : boolean(boolean) {} 
};

class UnaryOperator : public Expression {
    Expression& operant;
    int oper;
    UnaryOperator(Expression& operant, int oper) : operant(operant), oper(oper){}       
};

class BinaryOperator : public Expression {
    Expression& rhs;
    Expression& lhs;
    int oper;
    BinaryOperator(Expression& rhs, Expression& lhs, int oper) : rhs(rhs), lhs(lhs), oper(oper) {}
};

class Modifier : public Node { //Moved from between ControlFlowStatements and MethodDecl
    int Bpublic;
    int Bprivate; 
    int Bstatic;
    int Bprotected; 
    public: //Added
    Modifier() {}
    Modifier(int Bpublic, int Bprivate, int Bstatic, int Bprotected) : Bpublic(Bpublic), Bprivate(Bprivate), Bstatic(Bstatic), Bprotected(Bprotected) {}
};

class Declaration : public Statement {
    const Identifier& type;
    Identifier& id; 
    Expression val;
    Modifier mod;

    Declaration(const Identifier& type, Identifier& id) :  type(type), id(id) {} 
    Declaration(const Identifier& type, Identifier& id, Expression& val) : type(type), id(id), val(val) {}   
    Declaration(Modifier mod, const Identifier& type, Identifier& id) : mod(mod), type(type), id(id) {} 
    Declaration(Modifier mod, const Identifier& type, Identifier& id, Expression& val) : mod(mod), type(type), id(id), val(val) {}    
};

class Assignment : public Expression {
    Expression& lhs;
    Expression& rhs;
    Assignment(Expression& lhs, Expression& rhs) : lhs(lhs), rhs(rhs) {}
};

class Identifier : public Expression {
    string name; 
    Identifier(const string& name) : name(name) {}
};

class ControlFlowStatements : public Expression {
    Expression& condition;
    int type;
    Block& block; 
    ControlFlowStatements& proceeding;
    ControlFlowStatements(Expression& condition, int type, Block& block, ControlFlowStatements& proceeding) : condition(condition), type(type), block(block), proceeding(proceeding) {} 
};



class MethodDecl : public BaseStatement {
    Block& block;
    const Identifier& type; 
    Identifier& id; 
    Modifier mod;

    MethodDecl(Block& block, const Identifier& type, Identifier& id) : block(block), id(id), type(type) {}
    MethodDecl(Modifier mod, Block& block, const Identifier& type, Identifier& id) : mod(mod), block(block), id(id), type(type) {}

};
