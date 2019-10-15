//Trying to model https://en.wikipedia.org/wiki/Abstract_syntax_tree 

//------- TODO LIST -------
// Improve DictType
// Add missing verifies: 
//    UnaryOperator, 
//    DeclGroup, 
//    ControlLoop, 
//    ExpressionStatement, 
//    EmpMethodDecl, 
//    MethodCall, 
//    EmpMethodCall - NOT YET FINISHED
// Check that Modifiers are always called
// SecControlStatements: Process prev Control to verify valid placement
// Change some ints in UnaryOperator to enum
// Check Abstract vs Interface in modifiers
// Check Abstract and interface fully defined
// Multiple classes
// Add method for checking dictionary so that arguments become new method
// Check all verifies have the ability to write files

#include <iostream>
#include <vector>
#include <string>
#include <map> //Changed: Added
#include <typeinfo> //Changed: Added
#include <ctime> //Changed: Added
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include<stdlib.h>
#include <fstream>

using std::vector; 
using std::string;

using std::cout; //Changed: Added
using std::map; //Changed: Added
using std::ostringstream;
using std::pair;
using std::endl;

class Statement;
class BaseStatement;
class Class;
class Expression;
class Declaration; 

class ProgramBlock; //CHANGED: added this and following definitions
class Identifier;
class Modifier;
class DictEntry;

class DeclGroup;

struct DictHash;


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

//Added all between this and ending comment
//          Name, DictEntry
typedef map<string, DictEntry> DictLevel;
typedef pair<string, DictEntry> entry;
//typedef std::unordered_map<string, DictEntry&> NodeMap;

std::ofstream cppout ("ignus_proj.txt");

enum DictType { 
  Variable,
  Method,
  DictClass
};

class DictEntry
{
  public:

   string name;
   string typeName;
   DictType typeUsage;
   Modifier* modifier; //Verify this is correct
   declGroup* arguments;

   bool verified;


    //For nests:
   DictLevel *nodemap; //SHOULD BE NODE MAP
   DictEntry(string n, string t, DictType type, Modifier* m, declGroup* args)
   {
      name = n;
      typeName = t;
      typeUsage = type;
      modifier = m;
      arguments = args;
      nodemap = new DictLevel();//SHOULD BE NODE MAP

      verified = true;
   }


   DictEntry(bool v, string n, string t, DictType type, Modifier* m, declGroup* args)
   {
      name = n;
      typeName = t;
      typeUsage = type;
      modifier = m;
      arguments = args;
      nodemap = new DictLevel();//SHOULD BE NODE MAP

      verified = v;
   }

   DictEntry() {} //WHY?

   DictEntry(const DictEntry& d)
   {
      typeName = d.typeName;
      typeUsage = d.typeUsage;
      modifier = d.modifier;
      nodemap = d.nodemap;
      name = d.name;
   
      arguments = d.arguments;

      verified = d.verified;
   }

   vector<string> getDisplay()
   {
      vector<string> ans = vector<string>();
      if(verified)
      ans.push_back(name);
      else
      ans.push_back("\u2715 " + name);
      
      DictLevel::iterator it = nodemap->begin(); //SHOULD BE NODEMAP
      
      while(it != nodemap->end())
      {
         vector<string> temp = it->second.getDisplay();
      
         vector<string>::iterator tmpiterator = temp.begin();
          
         while(tmpiterator != temp.end())
         {
            string c = "\u251C"; //(char) 195(char) 195;
              //if(it+ == nodemap->end())
            c = "\u2514";//(unsigned char) 192;
            if(!verified)
                c = "\u2514";
              
            string add = "";
            if((*tmpiterator).find(c) != string::npos)
               c = " ";
            add += c;
            add += *tmpiterator;
         
            ans.push_back(add);
         
            tmpiterator++;
         }
      
         it++;
      }
   
      return ans;
   }

   void InsertToMap(DictEntry d)
   {
      nodemap->insert(entry(d.name, d));
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


string binOperName(binOper b)
{
   if(b == checkEqual) 
      return "==";
   if(b == checkNotEqual) 
      return "!=";
   if(b == lessThan) 
      return "<";
   if(b == lessEqThan) 
      return "<=";
   if(b == greaterThan) 
      return ">";
   if(b == greaterEqThan) 
      return ">=";
   if(b == typeOf) 
      return "===";
   if(b == notTypeOf) 
      return "!===";
   if(b == plus) 
      return "+";
   if(b == minus) 
      return "-";
   if(b == multi) 
      return "*";
   if(b == divide) 
      return "/";
   if(b == power) 
      return "^";
   if(b == mod) 
      return "%";
   return "UnknownBinOper";
}

string CtrlName(CTRLStmt c)
{
   if(c == IF) 
      return "if";
   if(c == ELIF) 
      return "elif";
   if(c == ELSE) 
      return "else";
   if(c == SWITCH) 
      return "switch";
   if(c == CASE) 
      return "case";
   if(c == FOR) 
      return "for";
   if(c == WHILE) 
      return "while";
   return "UnknownCTRLStmt";
}

string modifierName(modifiers m)
{
   if(m == Public) 
      return "public";
   if(m == Private) 
      return "private";
   if(m == Protected) 
      return "protected";
   if(m == Static) 
      return "static";
   if(m == Final) 
      return "final";
   if(m == Abstract) 
      return "abstract";
   if(m == Interface) 
      return "interface";
   return "UnknownModifer";
}

//End of completley new stuff

class Node{
    public: 
   virtual ~Node() {};

   virtual CompileStatement verify(vector<DictLevel>& stack) 
   { 
      std::cout<<"Name: " << typeid(*this).name() << "\n";
      string ans = "Error: undefinded verify for class ";
      ans = ans.append(typeid(*this).name());
      return CompileStatement(ans, false); 
   }

   virtual DictEntry getDictEntry() 
   { 
      string ans = "Error: undefinded DictEntry for class ";
      ans = ans.append(typeid(*this).name());
      cout << ans << "\n";
      exit (EXIT_SUCCESS);
   }
};

//CHANGED: Moved Modifier to here
class Modifier : public Node {
    public:
   vector<modifiers> modif; //CHANGED: from int to modifiers
   Modifier() {}


   virtual CompileStatement verify(vector<DictLevel>& stack) 
   { 
      cout<<"Verifying Modifier\n";
      if((find(modif.begin(), modif.end(), Public) != modif.end()) + (find(modif.begin(), modif.end(), Private) != modif.end()) + (find(modif.begin(), modif.end(), Protected) != modif.end()) > 1)
         return CompileStatement("Error: Conflicting Visibility Modifiers", false);
      
      return CompileStatement("", true); 
   }

   string debugString()
   {
      ostringstream o;
      for(vector<modifiers>::size_type i = 0; i != modif.size(); i++)
      {
         o<< modifierName(modif[i]) << " ";
      }
      return o.str();
   }
};


class BaseStatement : public Node {};

class Statement : public BaseStatement {};

class Expression : public Statement {

    public:
   virtual string debugString()
   { 
      std::cout<<"Name: " << typeid(*this).name() << "\n";
      string ans = "Error: called on base expression instead of ";
      ans = ans.append(typeid(*this).name());
      return ans;
   }
};

//CHANGED: 318 Moved here
class Identifier : public Expression {
    public:
   string name; 
   Identifier(const string& name) : name(name) {}

   virtual CompileStatement verify(vector<DictLevel>& dictionary)
   {
      cout<<"Verifying Identifier\n";
      return CompileStatement("Error: Identifiers should NEVER be verified from itsself!", false);
   }

   virtual bool isType(vector<DictLevel> &dictionary)
   {
      for(vector<DictLevel>::size_type i = 0; i != dictionary.size(); i++) 
      {
         DictLevel::iterator it = dictionary[i].find(name);
         if(it != dictionary[i].end())
         {
            if(it->second.typeUsage ==  Variable || it->second.typeUsage == DictClass)
               return true;
         }
      }
      return false;
   }

   virtual DictEntry* isDefinedID(vector<DictLevel> &dictionary)
   {
      for(vector<DictLevel>::size_type i = 0; i != dictionary.size(); i++) 
      {
         DictLevel::iterator it = dictionary[i].find(name);
         if(it != dictionary[i].end())
         {
            return &(it->second);
         }
      }
      return NULL;
   }


   virtual string debugString()
   { 
      return name;
   }
};


class Declaration : public Statement {
    public:
   const Identifier& type; //CHANGED: 48 from & to *
   Identifier& id; 
   Modifier mod;

   DictEntry dictentry;

   Declaration(Modifier mod, const Identifier& type, Identifier& id) : mod(mod), type(type), id(id) {dictentry = DictEntry(id.name, type.name, Variable, &mod, NULL);} 
   Declaration( const Identifier& type, Identifier& id) :  type(type), id(id) {dictentry = DictEntry(id.name, type.name, Variable, &mod, NULL);} 

   virtual CompileStatement verify(vector<DictLevel>& dictionary)
   {
      cout<<"Verifying Declaration\n";
      cout<<"\t";
      for(int i = 0; i < dictionary.size(); i++)
         cout<<"\t";
      cout<<mod.debugString() << type.name <<" " << id.name << "\n";
   
      
      CompileStatement test1 = mod.verify(dictionary);
      if(!test1.valid)
         return test1;
    //Ensures that it hasnt been defiend before
      if(id.isDefinedID(dictionary) != NULL)
         return CompileStatement("Error: double declaration of: " + id.name + " found in Declaration", false);
      
      DictLevel currlayer = dictionary.back();
      dictionary.pop_back();//DictLevel();
      currlayer[id.name] = dictentry;//DictEntry(id.name, DictClass, &mod);
   
      dictionary.push_back(currlayer); //This pushback is managed
   
      return CompileStatement("", true);
   }

   virtual DictEntry getDictEntry() 
   { 
      return dictentry;
   }
};

//CHANGED: Order of block and programBlock
class ProgramBlock : public BaseStatement {
    public:
   baseStmtList baseStatements;
   ProgramBlock() {}
    
   virtual CompileStatement verify(vector<DictLevel>& dict)
   {
      cout<<"Verifying ProgramBlock\n";
      for(vector<DictLevel>::size_type i = 0; i != baseStatements.size(); i++) 
      {
         CompileStatement cs = baseStatements[i]->verify(dict);
      
         if(!cs.valid)
            return cs;
      }
      return CompileStatement("", true);
   }
};

class Program : public Node{
    public:
   ProgramBlock& programBlock;
   Program(ProgramBlock& programBlock) : programBlock(programBlock) {} //CHANGED: made constructor have pointer to match variable
        
   virtual CompileStatement verify(vector<DictLevel>& dict)
   {
      cout<<"Verifying Program\n";
      return programBlock.verify(dict);
   }
};

class Block : public Expression {
    public:
   stmtList statements; 
   Block() {}

   virtual CompileStatement verify(vector<DictLevel>& dict)
   {
      cout<<"Verifying Block\n";
      for(vector<DictLevel>::size_type i = 0; i != statements.size(); i++) 
      {
         CompileStatement cs = statements[i]->verify(dict);
      
         if(!cs.valid)
            return cs;
      }
      return CompileStatement("", true);
   }

};

class EnhancedBlock : public BaseStatement {
    public:
   baseStmtList statements;
   EnhancedBlock() {}

   virtual CompileStatement verify(vector<DictLevel>& dict)
   {
      cout<<"Verifying EnhancedBlock\n";
      for(vector<DictLevel>::size_type i = 0; i != statements.size(); i++) 
      {
         CompileStatement cs = statements[i]->verify(dict);
      
         if(!cs.valid)
            return cs;
      }
      return CompileStatement("", true);
   }
};

class Class : public BaseStatement {
    public: 
   EnhancedBlock& block;
   Identifier& id;
   Modifier mod;
   DictEntry dictentry;

   Class(EnhancedBlock& block, Identifier& id): block(block) , id(id) {
      dictentry = DictEntry(id.name, id.name, DictClass, &mod, NULL);
   }
   Class(EnhancedBlock& block, Identifier& id, Modifier mod): block(block) , id(id), mod(mod) 
   {
      dictentry = DictEntry(id.name, id.name, DictClass, &mod, NULL);
   }
    
   virtual CompileStatement verify(vector<DictLevel>& dictionary)
   {
      cout<<"Verifying Class\n";
      cout<<"\t";
     // cout<<"\033[1m";
      for(int i = 1; i < dictionary.size(); i++)
      {
         cout<<"\t";
         cppout << "\t";
      }
      cout<<mod.debugString() << "class " << id.name << "\n";
      cppout<<mod.debugString() << "class " << id.name << endl << "{" << endl;
      //cout<<"\033[0m";
   
      
      CompileStatement test1 = mod.verify(dictionary);
      if(!test1.valid)
         return test1;
    //Ensures that it hasnt been defiend before
      if(id.isDefinedID(dictionary) != NULL)
         return CompileStatement("Error: double declaration of: " + id.name + " found in Class", false);
      
      DictLevel currlayer = dictionary.back();
      dictionary.pop_back();//DictLevel();
      currlayer[id.name] = dictentry;//DictEntry(id.name, DictClass, &mod);
   
      dictionary.push_back(currlayer); //This pushback is managed
    
      DictLevel layer = DictLevel(); 
      dictionary.push_back(layer);
   
      for(vector<DictLevel>::size_type i = 0; i != block.statements.size(); i++) 
      {
        
         CompileStatement cs = block.statements[i]->verify(dictionary);
         
         if(!cs.valid)
            return cs;
      
         DictEntry entry = block.statements[i]->getDictEntry();
      
         dictentry.nodemap->insert(std::pair<string, DictEntry>(entry.name, entry));
      }
   
      cppout << "}" << endl;
      dictionary.pop_back();
   
      return CompileStatement("", true); 
   }

   virtual DictEntry getDictEntry() 
   { 
      return dictentry;
   }
};

class Integer : public Expression {
    public:
   int num; 
   Integer(int num) : num(num) {}

   virtual CompileStatement verify(vector<DictLevel>& dict)
   {
      cout<<"Verifying int\n";
      return CompileStatement("int", true);
   }
   virtual string debugString()
   { 
      ostringstream o;
      o << num <<"";
      return o.str();
   }
};

class Double : public Expression {
    public:
   double num; 
   Double(double num) : num(num) {}

   virtual CompileStatement verify(vector<DictLevel>& dict)
   {
      cout<<"Verifying double\n";
      return CompileStatement("double", true);
   }
   virtual string debugString()
   { 
      ostringstream o;
      o << num <<"";
      return o.str();
   }
};

class String : public Expression {
    public:
   string str; 
   String(string str) : str(str) {}

   virtual CompileStatement verify(vector<DictLevel>& dict)
   {
      cout<<"Verifying string\n";
      return CompileStatement("string", true);
   }
   virtual string debugString()
   { 
      return str;
   }
};

class Char : public Expression {
    public:
   char ch; 
   Char(char ch) : ch(ch) {}

   virtual CompileStatement verify(vector<DictLevel>& dict)
   {
      cout<<"Verifying char\n";
      return CompileStatement("char", true);
   }
   virtual string debugString()
   { 
      ostringstream o;
      o << ch <<"";
      return o.str();
   }
};

class Bool : public Expression {
    public:
   bool boolean; 
   Bool(bool boolean) : boolean(boolean) {} 

   virtual CompileStatement verify(vector<DictLevel>& dict)
   {
      cout<<"Verifying bool\n";
      return CompileStatement("bool", true);
   }
   virtual string debugString()
   { 
      ostringstream o;
      o << boolean <<"";
      return o.str();
   }
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
   binOper oper; //CHANGED: 318 from int to binOper
   BinaryOperator(Expression& lhs, Expression& rhs, binOper oper) : rhs(rhs), lhs(lhs), oper(oper) {}


   virtual CompileStatement verify(vector<DictLevel>& dict)
   {
      cout<<"Verifying BinaryOperator\n";
      
      string typereturn = "";
      string typelhs = "";
      string typerhs = "";
      
   
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
   
      for(vector<DictLevel>::size_type i = 0; i != dict.size(); i++)
      {
         DictEntry test = dict[i][typelhs];
      
         if(!test.name.empty())
         {
            DictEntry operdef = test.nodemap->operator[](binOperName(oper));
            if(operdef.name.empty())
               return CompileStatement("Error: undefinded operator for " + test.name + " of " + binOperName(oper) + " in BinaryOperator", false);
            declGroup decls = *(operdef.arguments);
            typelhs = (decls[0]->type.name);
            typereturn = operdef.typeName;
            break;
         }
      }
   
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
   
   
      if(typerhs.compare(typelhs) != 0)
         return CompileStatement("Errory in bin operator: rhs does not match lhs (" + typerhs + " != " + typelhs + ")", false);
   
      return CompileStatement(typereturn, true);
   }

   virtual string debugString()
   { 
      ostringstream o;
   
      o << lhs.debugString() << " " << binOperName(oper) << " " << rhs.debugString() << ""; //TODO: If used, make sure oper displays correctly
      return o.str();
   }
};

class DeclGroup : public Expression {
    public: 
   declGroup arguments; 
   DeclGroup() {}
};



class DeclWAssign : public Statement {
   public:
   Identifier& type; //CHANGED: 318 Removed const so I could verify
   Identifier& id; 
   Modifier mod;
   Expression& val;
   DeclWAssign(Modifier mod, Identifier& type, Identifier& id, Expression& val) : mod(mod), type(type), id(id), val(val) {}    
   DeclWAssign(Identifier& type, Identifier& id, Expression& val) : type(type), id(id), val(val) {}  


   virtual CompileStatement verify(vector<DictLevel>& dict)
   {
      cout<<"Verifying DeclWAssign\n";
      cout<<"\t";
      for(int i = 1; i < dict.size(); i++)
      {
         cout<<"\t";
         cppout<<"\t";
      }
      cout<<mod.debugString() << type.name << " " << id.name << " = " << val.debugString() << "\n";

      cppout<<mod.debugString() << type.name << " " << id.name << " = " << val.debugString() << ";" << endl;

   
      CompileStatement test1 = mod.verify(dict);
      if(!test1.valid) 
         return test1;
   
      if(type.isType(dict))
      {
         DictEntry * de = id.isDefinedID(dict);
         if(de == NULL)
         {
            DictLevel layer = dict.back();
            dict.pop_back();
         
            layer[id.name] = getDictEntry();//DictEntry(type.name, Variable, &mod);
         
            dict.push_back(layer); //This pushback is good?
         
            return CompileStatement("", true);
         }
      
         return CompileStatement("Error: " + id.name + " is already defined in dict!", false); 
      }
   
      return CompileStatement("Error: Could not find assigment type: " + type.name, false);
   } 


   virtual DictEntry getDictEntry() 
   { 
      return DictEntry(id.name, type.name, Variable, &mod, NULL);
   }  
};

class Assignment : public Expression {
    public:
   Expression& lhs;
   Expression& rhs;
   Assignment(Expression& lhs, Expression& rhs) : lhs(lhs), rhs(rhs) {}

   virtual CompileStatement verify(vector<DictLevel>& dict)
   {
      cout<<"Verifying assigment\n";
      cout<<"\t";
      for(int i = 0; i < dict.size(); i++)
         cout<<"\t";
      //cout<<"<expression...> = <expression...>\n";
      cout<<lhs.debugString() << " = " << rhs.debugString() <<"\n";
   
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


   virtual CompileStatement verify(vector<DictLevel>& dict)
   {
      cout<<"Verifying ControlStatements\n";
      cout<<"\t";
      for(int i = 0; i < dict.size(); i++)
         cout<<"\t";
      cout<<CtrlName(type)<<" (" << condition.debugString() << ")\n";
   
      if(type != IF)
         return CompileStatement("Error in ControlStatements: type not if", false);
   
      DictLevel currlayer = DictLevel();
   
      dict.push_back(currlayer); //This pushback is managed
   
      CompileStatement cs = condition.verify(dict);
      if(!cs.valid)
         return cs;
   
      if(cs.msg != "bool")
      {
         return CompileStatement("Error in ControlStatements: condition is not a bool!", false);
      }
   
      cs = block.verify(dict);
      if(!cs.valid)
         return cs;
      dict.pop_back(); //Before return maybe?
   
      return CompileStatement("", true);
   }

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
   Control* proceeding; //CHANGED: 318 to other pointed type
   SecControlStatements(Expression* condition, CTRLStmt type, Block& block, Control* proceeding) : condition(condition), type(type), block(block), proceeding(proceeding) {} 

   virtual CompileStatement verify(vector<DictLevel>& dict)
   {
      cout<<"Verifying SecControlStatements\n";
      cout<<"\t";
      for(int i = 0; i < dict.size(); i++)
         cout<<"\t";
      cout<<CtrlName(type);
   
      if(type != ELIF && type != ELSE && type!= CASE)
         return CompileStatement("Error in SecControlStatements: type not else, elif or case", false);
   
      if(condition != NULL)
         cout<< " (" << condition->debugString() << ")";
      cout<<"\n";
   
      DictLevel dl = DictLevel();
      dict.push_back(dl); //TODO: VERIFY PUSHBACK
      //TODO: check type AND proceeding
   
      if(condition != NULL)
      {
         CompileStatement cs = condition->verify(dict);
      
         if(!cs.valid)
            return cs;
      }
   
      CompileStatement cs = block.verify(dict);
      if(!cs.valid)
         return cs;
      dict.pop_back();
   
      return CompileStatement("", true);
   
   }
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
   Identifier& type; //CHANGED: 318 Removed const so that verify can be done
   Identifier& id; 
   Modifier mod; 

   MethodDecl(Block& block,  Identifier& type, Identifier& id) : block(block), id(id), type(type) {}
   MethodDecl(Modifier mod, Block& block,  Identifier& type, Identifier& id) : mod(mod), block(block), id(id), type(type) {}

   virtual CompileStatement verify(vector<DictLevel>& dictionary)
   {
      cout<<"Verify MethodDecl\n";
      cout<<"\t";
      for(int i = 1; i < dictionary.size(); i++)
      {
         cout<<"\t";
         cppout<<"\t";
      }
      cout<<mod.debugString() << type.name << " " << id.name << "()\n";
      cppout<<mod.debugString() << type.name << " " << id.name << "()" << endl << "{" << endl;
   
      CompileStatement test1 = mod.verify(dictionary);
      if(!test1.valid) 
         return test1;
   
      if(!type.isType(dictionary))
         return CompileStatement("Error: MethodDecl cannot be set to invalid type of: " + type.name, false);
   
    //Ensures that it hasnt been defiend before
      if(id.isDefinedID(dictionary) != NULL)
         return CompileStatement("Error: double declaration of: " + id.name + " found in MethodDecl", false);
      
   
      DictLevel currlayer = dictionary.back();
      dictionary.pop_back();
      currlayer[id.name] = getDictEntry();//DictEntry(type.name, Method, &mod);
   
      dictionary.push_back(currlayer); //This pushback is managed
      
      dictionary.push_back(DictLevel());
      test1 = block.verify(dictionary);
      if(!test1.valid)
         return test1;
   
    
      cppout << "}" << endl;
      dictionary.pop_back();
   
      return CompileStatement("", true);
   }

   virtual DictEntry getDictEntry() 
   { 
      return DictEntry(id.name, type.name, Method, &mod, NULL);
   }   
    
};

string getSuperExpressionDisp(expList * super)
{
   ostringstream str;

   for(expList::size_type i = 0; i != super->size(); i++)
   {
      str << super->operator[](i)->debugString() << ".";
   }

   return str.str();
}

class MethodCall : public Expression {
    public:
   const Identifier& id;
   DeclGroup arguments;
   expList superExpression;
   MethodCall(const Identifier& id, DeclGroup arguments) : id(id), arguments(arguments) {}

};

class EmpMethodCall : public Expression {
    public:
   Identifier& id; //CHANGED: 48 removed const
   expList * superExpression;
   EmpMethodCall(Identifier& id) : id(id) {}

   virtual CompileStatement verify(vector<DictLevel>& dict)
   {
      cout<<"Verifying EmpMethodCall - ERROR THIS SHOULD BE FROM PREV VERIFY\n";
      cout<<"\t";
      for(int i = 0; i < dict.size(); i++)
         cout<<"\t";
      cout<< getSuperExpressionDisp(superExpression) << id.name << "()\n";
   
      
    //Ensures that it has been defined
      if(superExpression == NULL)
      {
         DictEntry * def = id.isDefinedID(dict); //DO with base super OR if <- is null, id
      
         if(def == NULL) //TODO: MAKE HAPPEN WHEN NULL (UNNEST LOOPS)
         {
          //PROCESS IF IN OTHER CLASS
            return CompileStatement("Error: EmpMethodCall to undefinded method", false);
         }
         return CompileStatement(def->typeName, true); 
      }
      else
      {
          string basename = superExpression->operator[](0)->debugString();
          DictEntry  base = dict[0][basename];
            
          if(base.name.empty())
          {
              DictEntry add = DictEntry(false, basename, basename, DictClass, NULL, NULL);
              dict[0][basename] = add;
              base = add;
          }

          for(expList::size_type i = 1; i != superExpression->size(); i++)
          {
              basename = superExpression->operator[](i)->debugString();
              if(base.nodemap->operator[](basename).name.empty())
              {
                  DictEntry add = DictEntry(false, basename, basename, DictClass, NULL, NULL);
                base.nodemap->operator[](basename) = add; //TODO: VERIFY WORKS
                base = add;
              }
              else //THIS SHOULD NEVER BE USED
              {
                  cout<<"Well, I was wrong, EmpMethodCall can have this be used. Huh. What caused this? \n";
                  base = base.nodemap->operator[](basename);
              }
          }

          base.nodemap->operator[](id.name) = DictEntry(false, id.name, "ERROR ADD ME IN EmpMethodCall", Method, NULL, NULL);
          
      }
      return CompileStatement("", true);
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

   DeclWAssign declA = DeclWAssign(id_int, identA, int_7);
   DeclWAssign declB = DeclWAssign(id_int, identB, int_5);
  
   BinaryOperator expr1 = BinaryOperator(identA, identB, minus);
   Assignment assignTrue = Assignment(identA, expr1);
  
   BinaryOperator expr2 = BinaryOperator(identB, identA, minus);
   Assignment assignFalse = Assignment(identB, expr2);
  
   Block trueBlock = Block();
   trueBlock.statements.push_back(&assignTrue);
  
   Block falseBlock = Block();
   falseBlock.statements.push_back(&assignFalse);
  
  
  
   BinaryOperator cond = BinaryOperator(identB, int_0, checkNotEqual);

  
   ControlStatements cfsIf = ControlStatements(cond, IF, trueBlock);

   SecControlStatements falseCFS = SecControlStatements(NULL, ELSE, falseBlock, &cfsIf);

   Block mainBlock = Block();
   mainBlock.statements.push_back(&declA);
   mainBlock.statements.push_back(&declB);
  
   mainBlock.statements.push_back(&cfsIf);
   mainBlock.statements.push_back(&falseCFS);

      //FOR TESTS:
   Identifier otherclass = Identifier("Math");
   Identifier currcall = Identifier("PI");

   expList superexpress = expList();
   superexpress.push_back(&otherclass);

   EmpMethodCall testcall = EmpMethodCall(currcall);
   testcall.superExpression = &superexpress;
   
   mainBlock.statements.push_back(&testcall);

   //END OF TESTS

   MethodDecl m1 = MethodDecl(mainBlock, id_int,id_main);

   EnhancedBlock classblock = EnhancedBlock();
   classblock.statements.push_back(&m1);

   Identifier classid = Identifier("TestClass");

   Class c = Class(classblock, classid);

   ProgramBlock pblock = ProgramBlock();
   pblock.baseStatements.push_back(&c);

   Program program = Program(pblock);


//Begins here
   vector<DictLevel> table;

   DictLevel consts = DictLevel();
//name, type

   DictEntry intentry = DictEntry("int","int", Variable, NULL, NULL);

   Modifier publicmod = Modifier();
   publicmod.modif.push_back(Public);

   Identifier oper_add = Identifier("+");
   Identifier oper_sub = Identifier("-");
   Identifier oper_mult = Identifier("*");
   Identifier oper_div = Identifier("/");
   Identifier oper_mod = Identifier("%");
   Identifier id_other_int = Identifier("o");

   Declaration arg_int_other = Declaration(id_int, id_other_int);
   DeclGroup group = DeclGroup();
   group.arguments.push_back(&arg_int_other);

   intentry.InsertToMap(DictEntry("+", "int", Method, &publicmod, &group.arguments));
   intentry.InsertToMap(DictEntry("-", "int", Method, &publicmod, &group.arguments));
   intentry.InsertToMap(DictEntry("*", "int", Method, &publicmod, &group.arguments));
   intentry.InsertToMap(DictEntry("/", "int", Method, &publicmod, &group.arguments));
   intentry.InsertToMap(DictEntry("%", "int", Method, &publicmod, &group.arguments));
   intentry.InsertToMap(DictEntry("!=", "bool", Method, &publicmod, &group.arguments));

   consts["int"] = intentry;
   consts["float"] = DictEntry("float", "float", Variable, NULL, NULL);
   consts["double"] = DictEntry("double","double", Variable, NULL, NULL);
   consts["string"] = DictEntry("string","string", Variable, NULL, NULL);
   consts["boolean"] = DictEntry("boolean","boolean", Variable, NULL, NULL);
//Other class info would go here too - maybe - we'll see

   table.push_back(consts);

//For measuring elapsed time: https://stackoverflow.com/questions/2808398/easily-measure-elapsed-time
   clock_t start = clock(); 

   //Start file:
   cppout << "#include <stdio.h>" << endl;

   CompileStatement cs = program.verify(table);
   cout<< "\n\nCompile completed:\n\n";
   cout<<"-Final messages: "<<cs.msg << "\n-Valid code: " << cs.valid << "\n";

   clock_t end = clock();

   double elapsed = double(end - start)/CLOCKS_PER_SEC;

   cout<<"-Time taken: " << elapsed << "(s)\n";


   cout<< "\n----- Dictionary -----\n";

   for(vector<DictLevel>::size_type i = 0; i != table.size(); i++) 
   {
      cout<<"\nLayer: " << i << "\n\n";
      map<string, DictEntry>::iterator it = table[i].begin();
      while(it != table[i].end())
      {
         vector<string> vec = it->second.getDisplay();
      
         for(vector<string>::size_type c = 0; c != vec.size(); c++)
         {
            cout<< vec[c] << "\n";
         }
      
         ++it;
      }
   }

   cout<<"\nOnly Layer 0 should be remaining!\n";

   return 0;
}