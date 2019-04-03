#include "AST.h"
#include <iostream>

///ASTNode
AST::ASTNode::ASTNode(nodeType type){
  this->type = type;
  this->terms = 0;
  this->parent = 0;
  this->next = 0;
  this->prev = 0;
  this->asp = "";
  this->name = "Default ASTNode";
}

void AST::ASTNode::add_var(std::string var, std::string pred){
  this->vars[var] = pred;
}

void AST::ASTNode::set_name(std::string name){
  this->name = name;
}

void AST::ASTNode::build(){
    if(this->next) this->next->build();
    if(this->terms) this->terms->build();
    this->build_asp();
}

void AST::ASTNode::build_asp(){
  std::cout << "ASTNode" << std::endl;
}

std::string AST::ASTNode::get_asp(){
  return this->asp;
}

///ConstrNode
AST::ConstrNode::ConstrNode(constrType type): ASTNode(CONSTR){
  this->cType = type;
  this->built = false;
  this->name = "Default ConstrNode";
}

void AST::ConstrNode::build_asp(){
  std::string predString = ", ";
  std::string varString = "(";
  std::map<std::string, std::string>::iterator it = this->vars.begin();
  varString += it->first;
  bool varsSorted = true;
  if(this->parent && (!this->parent->vars.count(it->first) ||
     this->parent->vars[it->first] == "")){
    this->parent->vars[it->first] = it->second;
  }
  if(it->second == ""){
    varsSorted = false;
  }
  predString += it->second + "(" + it->first + ")";
  for(it++; it != this->vars.end(); it++){
    varString += ", " + it->first;
    predString += ", " + it->second + "(" + it->first + ")";
    if(this->parent && (!this->parent->vars.count(it->first) ||
       this->parent->vars[it->first] == "")){
      this->parent->vars[it->first] = it->second;
    }
    if(it->second == ""){
      varsSorted = false;
    }
  }
  if(!varsSorted){
    return;
  }
  if(this->terms->type == EXPR){
    ExprNode *eNode = dynamic_cast<ExprNode*>(this->terms);
    if(!eNode->built){
      for(it = eNode->vars.begin(); it != eNode->vars.end(); it++){
        if(it->second == ""){
          if(this->vars.count(it->first) && this->vars[it->first] != ""){
            eNode->vars[it->first] = this->vars[it->first];
          }
          else{
            varsSorted = false;
          }
        }
      }
      if(varsSorted){
        eNode->build_asp();
      }
      else{
        return;
      }
    }
  }
  varString += ")";
  this->asp += "%" + this->name + "\n" + this->name;
  if(this->cType == MH){
    this->asp += varString + " :- not "+ this->name \
               + "_met" + varString + predString + ".\n";
    this->asp += this->name + "_met" + varString + " :- " \
               + this->terms->name + predString + ".\n";

  }
  this->name += varString;
  if(this->cType == MNH){
    this->asp += varString + " :- " + this->terms->name + predString + ".\n";
  }
  if(this->cType == IF){
    ConstrNode *cNode = dynamic_cast<ConstrNode*>(this->terms->next);
    if(!cNode->built){
      for(it = cNode->vars.begin(); it != cNode->vars.end(); it++){
        if(it->second == ""){
          if(this->vars.count(it->first) && this->vars[it->first] != ""){
            cNode->vars[it->first] = this->vars[it->first];
          }
          else{
            varsSorted = false;
          }
        }
      }
      if(varsSorted){
        cNode->build_asp();
      }
      else{
        return;
      }
    }
    this->asp += varString + " :- " + this->terms->name \
               + ", " + this->terms->next->name + predString + ".\n";
    if(this->terms->type == EXPR){
      this->asp += this->terms->asp;
    }
    this->asp += this->terms->next->asp;
    this->built = true;
    return;
  }
  for(ASTNode *temp = this->terms; temp; temp = temp->next){
    this->asp += temp->get_asp();
  }
  this->built = true;
}

///ExprNode
AST::ExprNode::ExprNode(exprType type): ASTNode(EXPR){
  this->eType = type;
  this->built = false;
  this->name = "Default ExprNode";
}

void AST::ExprNode::build_asp(){
  std::string predString = ", ";
  std::string varString = "(";
  std::map<std::string, std::string>::iterator it = this->vars.begin();
  bool varsSorted = true;
  varString += it->first;
  if(!this->parent->vars.count(it->first) ||
     this->parent->vars[it->first] == ""){
    this->parent->vars[it->first] = it->second;
  }
  if(it->second == ""){
    varsSorted = false;
  }
  predString += it->second + "(" + it->first + ")";
  for(it++; it != this->vars.end(); it++){
    varString += ", " + it->first;
    predString += ", " + it->second + "(" + it->first + ")";
    if(!this->parent->vars.count(it->first) ||
       this->parent->vars[it->first] == ""){
      this->parent->vars[it->first] = it->second;
    }
    if(it->second == ""){
      varsSorted = false;
    }
  }
  if(!varsSorted){
    return;
  }
  varString += ")";
  std::string buildASP, termASP, subExprASP = "";
  buildASP += this->name + varString + " :- ";
  if(this->terms->type == EXPR){
    ExprNode *eNode = dynamic_cast<ExprNode*>(this->terms);
    if(!eNode->built){
      for(it = eNode->vars.begin(); it != eNode->vars.end(); it++){
        if(it->second == ""){
          if(this->vars.count(it->first) && this->vars[it->first] != ""){
            eNode->vars[it->first] = this->vars[it->first];
          }
          else{
            varsSorted = false;
          }
        }
      }
      if(varsSorted){
        eNode->build_asp();
      }
      else{
        return;
      }
    }
    buildASP += this->terms->name;
    subExprASP += this->terms->asp;
  }
  else{
    buildASP += this->terms->asp;
  }
  if(this->eType == OR){
    buildASP += predString;
  }
  for(ASTNode *temp = this->terms->next; temp; temp = temp->next){
    if(temp->type == EXPR){
      ExprNode *eNode = dynamic_cast<ExprNode*>(temp);
      if(!eNode->built){
        for(it = eNode->vars.begin(); it != eNode->vars.end(); it++){
          if(it->second == ""){
            if(this->vars.count(it->first) && this->vars[it->first] != ""){
              eNode->vars[it->first] = this->vars[it->first];
            }
            else{
              varsSorted = false;
            }
          }
        }
        if(varsSorted){
          eNode->build_asp();
        }
      }
      termASP = temp->name;
      subExprASP += temp->asp;
    }
    else{
      termASP = temp->asp;
    }
    if(this->eType == AND){
      buildASP += ", " + termASP;
    }
    else{
      buildASP += ".\n" + this->name + varString + " :- " + termASP + predString;
    }
  }
  if(this->eType == AND){
    buildASP += predString;
  }
  this->name += varString;
  this->asp += buildASP + ".\n" + subExprASP;
  this->built = true;
}

///TermNode
AST::TermNode::TermNode(termType type): ASTNode(TERM){
  this->tType = type;
  this->varPos = 0;
  this->name = "Default TermNode";
}

void AST::TermNode::build_asp(){
  this->asp += this->name;
  if(this->tType == PRED){
    this->asp += "(";
    std::map<std::string, std::string>::iterator it;
    for(it = this->vars.begin(); it != this->vars.end(); it++){
      if(it != this->vars.begin()){
        this->asp += ", ";
      }
      this->asp += it->first;
      if(!this->parent->vars.count(it->first)||
         this->parent->vars[it->first] == ""){
        this->parent->vars[it->first] = it->second;
      }
    }
    this->asp += ")";
  }
  else{
    std::string oper, operand;
    if(this->vars.begin()->second == "op"){
      oper = this->vars.begin()->first;
      operand = std::prev(this->vars.end())->first;
    }
    else{
      oper =  std::prev(this->vars.end())->first;
      operand =  this->vars.begin()->first;
    }
    if(!this->parent->vars.count(this->asp)){
      this->parent->vars[this->asp] = "";
    }
    if(!this->parent->vars.count(operand)){
      this->parent->vars[operand] = "";
    }
    this->asp += " " + oper + " " + operand;
    this->name = this->asp;
  }
}

///AST
AST::AST(){
  this->constraints = 0;
  this->curr = 0;
  this->asp = "";
  this->initialize_pred_sorts();
}

AST::~AST(){
  this->destr_helper(this->constraints);
}

void AST::initialize_pred_sorts(){
  //TODO: make sure second parameters have predicates in CoreGenYaml.cpp
  this->sortMap["regWidth"] = new std::string[2]{"reg", "int"};
  this->sortMap["regIndex"] = new std::string[2]{"reg", "int"};
  this->sortMap["regIsFixed"] = new std::string[2]{"reg", "bool"};
  this->sortMap["regFixedVal"] = new std::string[2]{"reg", "int"};
  this->sortMap["regIsSIMD"] = new std::string[2]{"reg", "bool"};
  this->sortMap["regIsRWReg"] = new std::string[2]{"reg", "bool"};
  this->sortMap["regIsROReg"] = new std::string[2]{"reg", "bool"};
  this->sortMap["regIsCSRReg"] = new std::string[2]{"reg", "bool"};
  this->sortMap["regIsAMSReg"] = new std::string[2]{"reg", "bool"};
  this->sortMap["regIsTUSReg"] = new std::string[2]{"reg", "bool"};
  this->sortMap["regPseudoName"] = new std::string[2]{"reg", "name"};
  this->sortMap["regSIMDWidth"] = new std::string[2]{"reg", "int"};
  this->sortMap["regAttrs"] = new std::string[2]{"reg", "int"};
  this->sortMap["regIsShared"] = new std::string[2]{"reg", "bool"};
  this->sortMap["regSubreg"] = new std::string[2]{"reg", "subreg"};
  this->sortMap["subregStartBit"] = new std::string[2]{"subreg", "int"};
  this->sortMap["subregEndBit"] = new std::string[2]{"subreg", "int"};
  this->sortMap["regClassReg"] = new std::string[2]{"regClass", "reg"};
  this->sortMap["instFormatISA"] = new std::string[2]{"instFormat", "isa"};
  this->sortMap["instFormatField"] = new std::string[2]{"instFormat", "field"};
  this->sortMap["fieldType"] = new std::string[2]{"field", "fType"};
  this->sortMap["fieldWidth"] = new std::string[2]{"field", "int"};
  this->sortMap["fieldStartbit"] = new std::string[2]{"field", "int"};
  this->sortMap["fieldEndBit"] = new std::string[2]{"field", "int"};
  this->sortMap["fieldIsMandatory"] = new std::string[2]{"field", "bool"};
  this->sortMap["fieldRegClass"] = new std::string[2]{"field", "regClass"};
  this->sortMap["instISA"] = new std::string[2]{"inst", "isa"};
  this->sortMap["instIF"] = new std::string[2]{"inst", "instFormat"};
  this->sortMap["encFieldValue"] = new std::string[2]{"encField", "int"};
  this->sortMap["pseudoInstISA"] = new std::string[2]{"pseudoInst", "isa"};
  this->sortMap["pseudoInstIN"] = new std::string[2]{"pseudoInst", "inst"};
  this->sortMap["pEncFieldValue"] = new std::string[2]{"pEncField", "int"};
  this->sortMap["cacheSets"] = new std::string[2]{"cache", "int"};
  this->sortMap["cacheWays"] = new std::string[2]{"cache", "int"};
  this->sortMap["cacheParentOf"] = new std::string[2]{"cache", "cache"};
  this->sortMap["cacheChildeOf"] = new std::string[2]{"cache", "cache"};
  this->sortMap["coreCache"] = new std::string[2]{"core", "cache"};
  this->sortMap["coreISA"] = new std::string[2]{"core", "isa"};
  this->sortMap["coreThreadUnits"] = new std::string[2]{"core", "int"};
  this->sortMap["coreRegClass"] = new std::string[2]{"core", "regClass"};
  this->sortMap["socCore"] = new std::string[2]{"soc", "core"};
  this->sortMap["spadMemSize"] = new std::string[2]{"spad", "int"};
  this->sortMap["spadRqstPorts"] = new std::string[2]{"spad", "int"};
  this->sortMap["spadRspPorts"] = new std::string[2]{"spad", "int"};
  this->sortMap["spadStartAddr"] = new std::string[2]{"spad", "int"};
  this->sortMap["memCtrlPorts"] = new std::string[2]{"memCtrl", "int"};
  this->sortMap["pluginFeature"] = new std::string[2]{"plugin", "feature"};
  this->sortMap["featureType"] = new std::string[2]{"feature", "fType"};
  this->sortMap["pluginReg"] = new std::string[2]{"plugin", "reg"};
  this->sortMap["pluginRegClass"] = new std::string[2]{"plugin", "regClass"};
  this->sortMap["pluginISA"] = new std::string[2]{"plugin", "isa"};
  this->sortMap["pluginInstFormat"] = new std::string[2]{"plugin", "instFormat"};
  this->sortMap["pluginInst"] = new std::string[2]{"plugin", "inst"};
  this->sortMap["pluginPsuedoInst"] = new std::string[2]{"plugin", "pseudoInst"};
  this->sortMap["pluginCache"] = new std::string[2]{"plugin", "cache"};
  this->sortMap["pluginCore"] = new std::string[2]{"plugin", "core"};
  this->sortMap["pluginScratchpad"] = new std::string[2]{"plugin", "spad"};
  this->sortMap["pluginMemoryController"] = new std::string[2]{"plugin", "memCtrl"};
  this->sortMap["pluginExtension"] = new std::string[2]{"plugin", "extension"};
  this->sortMap["pluginVTP"] = new std::string[2]{"plugin", "vtp"};
  this->sortMap["pluginSoc"] = new std::string[2]{"plugin", "soc"};
  this->sortMap["pluginComm"] = new std::string[2]{"plugin", "comm"};
  this->sortMap["extensionType"] = new std::string[2]{"extension","eType"};
  this->sortMap["extensionReg"] = new std::string[2]{"extension", "reg"};
  this->sortMap["extensionRegClass"] = new std::string[2]{"extension", "regClass"};
  this->sortMap["extensionISA"] = new std::string[2]{"extension", "isa"};
  this->sortMap["extensionInstFormat"] = new std::string[2]{"extension", "instFormat"};
  this->sortMap["extensionInst"] = new std::string[2]{"extension", "inst"};
  this->sortMap["extensionPsuedoInst"] = new std::string[2]{"extension", "pseudoInst"};
  this->sortMap["extensionCache"] = new std::string[2]{"extension", "cache"};
  this->sortMap["extensionCore"] = new std::string[2]{"extension", "core"};
  this->sortMap["extensionScratchpad"] = new std::string[2]{"extension", "spad"};
  this->sortMap["extensionMemoryController"] = new std::string[2]{"extension", "memCtrl"};
  this->sortMap["extensionExtension"] = new std::string[2]{"extension", "extension"};
  this->sortMap["extensionComm"] = new std::string[2]{"extension", "comm"};
  this->sortMap["commType"] = new std::string[2]{"comm", "cType"};
  this->sortMap["commWidth"] = new std::string[2]{"comm", "int"};
  this->sortMap["commEndpoint"] = new std::string[2]{"comm", "endpoint"};
}

void AST::destr_helper(ASTNode *node){
  if(node){
    this->destr_helper(node->next);
    this->destr_helper(node->terms);
    delete node;
  }
}

AST::ASTNode* AST::create_node(std::string type){
  ASTNode *newNode = 0;
  if(type == "IF" || type == "if"){
    newNode = new ConstrNode(IF);
  }
  else if(type == "MH" || type == "mh"){
    newNode = new ConstrNode(MH);
  }
  else if(type == "MNH" || type == "mnh"){
    newNode = new ConstrNode(MNH);
  }
  else if(type == "AND" || type == "and"){
    newNode = new ExprNode(AND);
  }
  else if(type == "OR" || type == "or"){
    newNode = new ExprNode(OR);
  }
  else if(type == "PRED" || type == "pred"){
    newNode = new TermNode(PRED);
  }
  else if(type == "COMP" || type == "comp"){
    newNode = new TermNode(COMP);
  }
  return newNode;
}

void AST::add_constraint(std::string type){
  ASTNode *newNode = this->create_node(type);
  ASTNode *temp = this->constraints;
  if(!temp){
    this->constraints = newNode;
    this->curr = newNode;
  }
  else{
    while(temp->next){
      temp = temp->next;
    }
    temp->next = newNode;
    newNode->prev = temp;
  }
  this->curr = newNode;
}

void AST::add_sibling(std::string type){
  ASTNode *newNode = this->create_node(type);
  ASTNode *temp = this->curr;
  while(temp->next){
    temp = temp->next;
  }
  temp->next = newNode;
  newNode->prev = temp;
  newNode->parent = temp->parent;
}

void AST::add_child(std::string type){
  ASTNode *newNode = this->create_node(type);
  newNode->parent = this->curr;
  if(!this->curr->terms){
    this->curr->terms = newNode;
  }
  else{
    ASTNode *temp = this->curr->terms;
    while(temp->next){
      temp = temp->next;
    }
    temp->next = newNode;
    newNode->prev = temp;
  }
}

void AST::add_var(std::string var){
  TermNode *tNode = dynamic_cast<TermNode*>(this->curr);
  if(tNode->tType == PRED){
    tNode->add_var(var, this->sortMap[tNode->name][tNode->varPos]);
    tNode->varPos++;
  }
  else{
    if(var == "="  ||
       var == "<"  ||
       var == ">"  ||
       var == "<=" ||
       var == ">=" ||
       var == "!="){
      tNode->add_var(var, "op");
    }
    else{
      tNode->add_var(var, "");
    }
  }
}

void AST::ascend(){
  if(this->curr->parent){
    this->curr = this->curr->parent;
  }
}

void AST::descend(){
  if(this->curr->terms){
    this->curr = this->curr->terms;
  }
}

void AST::advance(){
  if(this->curr->next){
    this->curr = this->curr->next;
  }
}

void AST::reverse(){
  if(this->curr->prev){
    this->curr = this->curr->prev;
  }
}

void AST::set_name(std::string name){
  this->curr->set_name(name);
}


void AST::build_asp(){
  ASTNode *temp = this->constraints;
  if(!temp) return;
  temp->build();

  while(temp){
    this->asp += temp->get_asp() + "\n";
    temp = temp->next;
  }
}

std::string AST::get_asp(){
  return this->asp;
}
