#ifndef AST_H
#define AST_H
#include <vector>
#include <map>
#include <string>

class AST{
  enum nodeType{
    CONSTR,
    EXPR,
    TERM
  };

  enum constrType{
    IF,
    MH,
    MNH
  };

  enum exprType{
    AND,
    OR
  };

  enum termType{
    PRED,
    COMP
  };

  class ASTNode{
  protected:
    nodeType type;
    std::map<std::string, std::string> vars;
    ASTNode *terms, *parent, *next, *prev;
    std::string name, asp;
  public:
    ASTNode(nodeType);
    virtual ~ASTNode(){};
    void add_var(std::string, std::string);
    void set_name(std::string);
    bool has_var(std::string);
    void build();
    virtual void build_asp();
    std::string get_asp();
    friend class AST;
    friend class ConstrNode;
    friend class ExprNode;
    friend class TermNode;
  };

  class ConstrNode: public ASTNode{
    constrType cType;
    bool built;
  public:
    ConstrNode(constrType);
    void build_asp();
  };

  class ExprNode: public ASTNode{
    exprType eType;
    bool built;
  public:
    ExprNode(exprType);
    void build_asp();
    friend class AST;
  };

  class TermNode: public ASTNode{
    termType tType;
    int varPos;
  public:
    TermNode(termType);
    void build_asp();
    friend class AST;
  };

  ASTNode *constraints, *curr;
  std::map<std::string, std::string*> sortMap;
  std::string asp;
  void initialize_pred_sorts();
  void destr_helper(ASTNode*);
  ASTNode* create_node(std::string);
public:
  AST();
  ~AST();
  void add_constraint(std::string type);
  void add_sibling(std::string);
  void add_child(std::string);
  void add_var(std::string);
  void ascend();
  void descend();
  void advance();
  void reverse();
  void set_name(std::string);
  void build_asp();
  std::string get_asp();
};
#endif
