//Trying to model https://en.wikipedia.org/wiki/Abstract_syntax_tree 

//------- TODO LIST -------
// Improve DictType
// Update ALL verifys (And add missing ones)
// Verify operator in BinaryOperator
// Check lhs first in BinaryOperator?
// Verify operator in ControlFlowStatements
// Process mod in MethodDecl
// Multiple classes



//#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <typeinfo>
#include <ctime>


using std::cout;
using std::map;
using std::vector; 
using std::string;

class Statement;
class BaseStatement;
class Expression;
class VariableDecl; 
class Identifier; //Added
class Modifier; //Added
class DictEntry;

enum Boolean {
    False,
    True
};

enum DictType { //TODO: Improve
  Variable,
  Method
};

//Operators
enum BinOperator{
  SUBTRACT,
  NOT_EQUAL
};


//Controls
enum FlowTypes{
  IF,
  ELSE
};

typedef vector<Statement*> stmtList;
typedef vector<BaseStatement*> baseStmtList;
typedef vector<Expression*> expList;
typedef vector<VariableDecl*> VariableDecls; //Changed from VariableDecl tp VariableDecls

//          Name, DictEntry
typedef map<string, DictEntry> m;

class DictEntry
{
  public:

    string typeName;
    DictType typeUsage;

    DictEntry(string t, DictType n)
    {
      typeName = t;
      typeUsage = n;
    }

    DictEntry() {} //WHY?

    DictEntry(const DictEntry& d)
    {
      typeName = d.typeName;
      typeUsage = d.typeUsage;
    }
};

class CompileStatement
{
  public:
  bool valid;
  string msg;


  CompileStatement(string m, bool v)
  {
    valid = v;
    msg = m;
  }
};


class Node{
    public: 
        virtual ~Node() {};
        virtual CompileStatement verify(vector<m>& stack) 
        { 
          std::cout<<"Name: " << typeid(*this).name() << "\n";
          string ans = "Error: undefinded verify for class ";
          ans = ans.append(typeid(*this).name());

          return CompileStatement(ans, false); 
        }
};

class BaseStatement : public Node {};

class Statement : public BaseStatement {};

class Expression : public Statement 
{
  public:
  virtual ~Expression() { }
};

class Program : public Node{};

class Block : public Expression {
  public:
    stmtList statements; 
    Block() {}

    virtual CompileStatement verify(vector<m>& dict)
    {
      cout<<"Verifying block\n";
      for(std::vector<m>::size_type i = 0; i != statements.size(); i++) 
      {
        CompileStatement cs = statements[i]->verify(dict);

        if(!cs.valid)
          return cs;
      }
      return CompileStatement("", true);
    }
};

class EnhancedBlock : public Expression {
  public:
    baseStmtList statements;
    EnhancedBlock() {}
};

class Class : public Node {
    EnhancedBlock& block;
    Identifier& id;

    Class(EnhancedBlock& block, Identifier& id): block(block) , id(id) {}   
};

class Integer : public Expression {
  public:
    int num; 
    Integer(int num) : num(num) {}

    virtual CompileStatement verify(vector<m>& dict)
    {
      cout<<"Verifying int\n";
      return CompileStatement("int", true);;
    }
};

class Double : public Expression {
    double num; 
    Double(double num) : num(num) {}

    virtual CompileStatement verify(vector<m>& dict)
    {
      cout<<"Verifying Double\n";
      return CompileStatement("double", true);
    }
};

class String : public Expression {
  public:  
    string str; 
    String(string str) : str(str) {}

    virtual CompileStatement verify(vector<m>& dict)
    {
      cout<<"Verifying String\n";
      return CompileStatement("string", true);;
    }
};

class Bool : public Expression {
    bool boolean; 
    Bool(bool boolean) : boolean(boolean) {} 

    virtual CompileStatement verify(vector<m>& dict)
    {
      cout<<"Verifying boolean\n";
      return CompileStatement("boolean", true);;
    }
};

class UnaryOperator : public Expression {
  public:
    Expression& operant;
    int oper;
    UnaryOperator(Expression& operant, int oper) : operant(operant), oper(oper){}       
};

class Identifier : public Expression {
  
  public:
    string name; 
    Identifier(const string& name) : name(name) {}
    virtual ~Identifier() { }

    virtual CompileStatement verify(vector<m>& dictionary)
    {
      cout<<"Verifying Identifier\n";
      return CompileStatement("Error: Identifiers should NEVER be verified from itsself!", false);
    }

    virtual bool isType(vector<m> &dictionary)
    {
      for(std::vector<m>::size_type i = 0; i != dictionary.size(); i++) 
      {
        m::iterator it = dictionary[i].find(name);
        if(it != dictionary[i].end())
        {
          if(it->second.typeUsage == Variable)
            return true;
        }
      }
      return false;
    }

    virtual DictEntry* isDefinedID(vector<m> &dictionary)
    {
      for(std::vector<m>::size_type i = 0; i != dictionary.size(); i++) 
      {
        m::iterator it = dictionary[i].find(name);
        if(it != dictionary[i].end())
        {
          return &(it->second);
        }
      }
      return NULL;
    }
};

class BinaryOperator : public Expression {
  public:
    Expression& rhs;
    Expression& lhs;
    BinOperator oper;
    BinaryOperator(Expression& rhs, Expression& lhs, BinOperator oper) : rhs(rhs), lhs(lhs), oper(oper) {}

    virtual CompileStatement verify(vector<m>& dict)
    {
      cout<<"Verifying BinaryOperator\n";

      string typerhs = "";
      string typelhs = "";

      if(Identifier* test = dynamic_cast<Identifier*>(&rhs))
      {
        if(test->isDefinedID(dict) == NULL)
        {
          return CompileStatement("Error: Identifier in BinaryOperator rhs is not defined: " + test->name, false);
        }
        typerhs = test->isDefinedID(dict)->typeName;
      }
      else
      {
        CompileStatement cs = rhs.verify(dict);
        if(!cs.valid)
          return cs;
        
        typerhs = cs.msg;
      }

      if(Identifier* test = dynamic_cast<Identifier*>(&lhs))
      {
        if(test->isDefinedID(dict) == NULL)
        {
          return CompileStatement("Error: Identifier in BinaryOperator lhs is not defined: " + test->name, false);
        }
        typelhs = test->isDefinedID(dict)->typeName;
      }
      else
      {
        CompileStatement cs = lhs.verify(dict);
        if(!cs.valid)
          return cs;
        typelhs = cs.msg;
      }

      if(typerhs.compare(typelhs) != 0)
        return CompileStatement("Errory in bin operator: rhs does not match lhs (" + typerhs + " != " + typelhs + ")", false);
      
      //TODO: VERIFY oper

      return CompileStatement(typelhs, true);
    }
};

class Modifier : public Node { //Moved from between ControlFlowStatements and MethodDecl
    int Bpublic;
    int Bprivate; 
    int Bstatic;
    int Bprotected; 
    public: //Added
    int condensed;
    Modifier() {}
    Modifier(int Bpublic, int Bprivate, int Bstatic, int Bprotected) : Bpublic(Bpublic), Bprivate(Bprivate), Bstatic(Bstatic), Bprotected(Bprotected) 
    {
      int pub
    }
};


class Declaration : public Statement {
  public:
    Identifier type; //removed cost & 
    Identifier id; //removed & 
    Expression val;
    Modifier mod;

    Declaration(const Identifier& type, Identifier& id) :  type(type), id(id) {} 
    Declaration(const Identifier& type, Identifier& id, Expression& val) : type(type), id(id), val(val) {}   
    Declaration(Modifier mod, const Identifier& type, Identifier& id) : mod(mod), type(type), id(id) {} 
    Declaration(Modifier mod, const Identifier& type, Identifier& id, Expression& val) : mod(mod), type(type), id(id), val(val) {}  

  virtual CompileStatement verify(vector<m>& dict)
  {
    cout<<"Verifying Declaration\n";

    if(type.isType(dict))
    {
      DictEntry * de = id.isDefinedID(dict);
      if(de == NULL)
      {
        m layer = dict.back();
        dict.pop_back();

        layer[id.name] = DictEntry(type.name, Variable);

        dict.push_back(layer); //This pushback is good

        return CompileStatement("", true);
      }
      
        return CompileStatement("Error: " + id.name + " is already defined in dict!", false); 
    }

    return CompileStatement("Error: Could not find assigment type: " + type.name, false);
  }  
};



class Assignment : public Expression {
  public:
    Expression& lhs;
    Expression& rhs;
    Assignment(Expression& lhs, Expression& rhs) : lhs(lhs), rhs(rhs) {}

  virtual CompileStatement verify(vector<m>& dict)
  {
    cout<<"Verifying assigment\n";

    string typelhs = "";
    string typerhs = "";

    if(Identifier* test = dynamic_cast<Identifier*>(&lhs))
    {
      DictEntry * entry = test->isDefinedID(dict);

      if(entry == NULL)
        return CompileStatement("Error: lhs in assignment is not a var: " + test->name, false);
      typelhs = entry->typeName;
    }
    else
    {
      return CompileStatement("Error: lhs is not an Identifier in assignment", false);
    }

    if(Identifier* test = dynamic_cast<Identifier*>(&rhs))
    {
      DictEntry * entry = test->isDefinedID(dict);

      if(entry == NULL)
        return CompileStatement("Error: rhs in assignment is not a var: " + test->name, false);
      typerhs = entry->typeName;
    }
    else
    {
      CompileStatement cs = rhs.verify(dict);
      if(!cs.valid)
        return cs;
      typerhs = cs.msg;
    }

    if(typelhs.compare(typerhs) != 0)
      return CompileStatement("Error: type mismatch in assignment (" + typerhs + " != " + typelhs + " )", false);

    return CompileStatement("", true);
  }
};



class ControlFlowStatements : public Expression {
  public:
    Expression* condition;
    FlowTypes type;
    Block& block; 
    ControlFlowStatements * proceeding;

    ControlFlowStatements(Expression * condition, FlowTypes type, Block& block, ControlFlowStatements * proceeding) : condition(condition), type(type), block(block), proceeding(proceeding) 
    {
    } 

    virtual CompileStatement verify(vector<m>& dict)
    {
      cout<<"Verifying ControlFlowStatements\n";
      //TODO: check type

      if(condition != NULL)
      {
        CompileStatement cs = condition->verify(dict);

        if(!cs.valid)
         return cs;
      }

      CompileStatement cs = block.verify(dict);
      if(!cs.valid)
        return cs;

      if(proceeding != NULL)
      {
        cs = proceeding->verify(dict);
        if(!cs.valid)
          return cs;
      }


      return CompileStatement("", true);

    }
};



class MethodDecl : public BaseStatement {
  //Removed const on Identifier
  public:
    Block& block;
    Identifier& type; 
    Identifier& id; 
    Modifier mod;
public:
    MethodDecl(Block& block,  Identifier& type, Identifier& id) : block(block), id(id), type(type) {}
    MethodDecl(Modifier mod, Block& block,  Identifier& type, Identifier& id) : mod(mod), block(block), id(id), type(type) {}

  virtual CompileStatement verify(vector<m>& dictionary)
  {
    cout<<"Verify MethodDecl\n";

    if(!type.isType(dictionary))
      return CompileStatement("Error: MethodDecl cannot be set to invalid type of: " + type.name, false);

    //Ensures that it hasnt been defiend before
    if(id.isDefinedID(dictionary) != NULL)
      return CompileStatement("Error: double declaration of: " + id.name + " found in MethodDecl", false);
      

    m currlayer = m();
    currlayer[id.name] = DictEntry(type.name, Method);

    dictionary.push_back(currlayer); //This pushback is managed

    CompileStatement test1 = block.verify(dictionary);
    if(!test1.valid)
      return test1;

    
    dictionary.pop_back();

    return CompileStatement("", true); //TODO: Process MOD!
  }

};




int main()
{
  
  Identifier id_int = Identifier("int");
  Identifier id_main = Identifier("main");
  
  Identifier identA = Identifier("a");
  Identifier identB = Identifier("b");
  Integer int_0 = Integer(0);
  
  Integer int_5 = Integer(5);
  Integer int_7 = Integer(7);
  
  Declaration declA = Declaration(id_int, identA, int_7);
  Declaration declB = Declaration(id_int, identB, int_7);
  
  BinaryOperator expr1 = BinaryOperator(identA, identB, SUBTRACT);
  Assignment assignTrue = Assignment(identA, expr1);
  
  BinaryOperator expr2 = BinaryOperator(identB, identA, SUBTRACT);
  Assignment assignFalse = Assignment(identB, expr2);
  
  Block trueBlock = Block();
  trueBlock.statements.push_back(&assignTrue);
  
  Block falseBlock = Block();
  falseBlock.statements.push_back(&assignFalse);
  
  ControlFlowStatements falseCFS = ControlFlowStatements(NULL, ELSE, falseBlock, NULL);
  
  BinaryOperator cond = BinaryOperator(identB, int_0, NOT_EQUAL);

  
  ControlFlowStatements cfsIf = ControlFlowStatements(&cond, IF, trueBlock, &falseCFS);

  Block mainBlock = Block();
  mainBlock.statements.push_back(&declA);
  mainBlock.statements.push_back(&declB);
  
  mainBlock.statements.push_back(&cfsIf);

  MethodDecl m1 = MethodDecl(mainBlock, id_int,id_main);


//Begins here
vector<m> table;

m consts = m();
//name, type
consts["int"] = DictEntry("int", Variable);
consts["float"] = DictEntry("float", Variable);
consts["double"] = DictEntry("double", Variable);
consts["string"] = DictEntry("string", Variable);
consts["boolean"] = DictEntry("boolean", Variable);


table.push_back(consts);

//For measuring elapsed time: https://stackoverflow.com/questions/2808398/easily-measure-elapsed-time
clock_t start = clock(); 

std::cout<< m1.verify(table).msg << "\n";

clock_t end = clock();

double elapsed = double(end - start)/CLOCKS_PER_SEC;

std::cout<<"Time taken: " << elapsed << "(s)\n";


std::cout<< "\n---- Dictionary -----\n";

for(std::vector<m>::size_type i = 0; i != table.size(); i++) 
{
  cout<<"\nLayer: " << i << "\n";
  map<string, DictEntry>::iterator it = table[i].begin();
  while(it != table[i].end())
  {
    cout<< it->first << "\n";
    ++it;
  }
}

  cout<<"\nOnly Layer 0 should be remaining!\n";

  return 0;
}



