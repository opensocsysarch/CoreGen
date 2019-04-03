#include "ConstrParser.h"
#include <iostream>


ConstrParser::ConstrParser(){
  this->lineNum = 1;
  this->colNum = 0;
  this->opcount = 0;
  this->inFile = NULL;
  this->inFileName = "";
  this->start_new_terms_list = true;
}

ConstrParser::ConstrParser(std::string fileName){
  this->lineNum = 1;
  this->colNum = 0;
  this->opcount = 0;
  this->inFile = new std::ifstream(fileName, std::ifstream::in);
  this->inFileName = fileName;
  this->start_new_terms_list = true;
}

ConstrParser::~ConstrParser(){
  this->inFile->close();
  delete inFile;
}

void ConstrParser::set_inFile(std::string fileName){
  this->inFileName = fileName;
  this->inFile = new std::ifstream(fileName, std::ifstream::in);
}

char ConstrParser::next_token(){
  this->colNum++;
  char c = this->inFile->get();
  if(c == '\n'){
    this->lineNum++;
  }
  return c;
}

void ConstrParser::return_token(){
  this->colNum--;
  this->inFile->unget();
}

std::string ConstrParser::get_name(){
  char c;
  std::string predicate = "";
  do{
    c = this->inFile->get();
    if(isalnum(c)){
      predicate += c;
    }
  }while(isalnum(c));
  this->return_token();
  return predicate;
}

void ConstrParser::clear_space(){
  char c;
  do{
    c = this->next_token();
    if(c == '\n'){
      this->lineNum++;
    }
  }while(isspace(c));
  this->return_token();
}

bool ConstrParser::parse(){
  //clear whitespace
  this->clear_space();
  //check if there are more constraints to parse
  if(this->inFile->peek() < 0){
    return true;
  }
  //check constraint
  if(this->constraint()){
    char c = this->next_token();
    if(c == '.'){
      if(this->parse()){
        return true;
      }
      else{
        //constraint must be followedby another constraint or nothing
        return false;
      }
    }
    else{
      //constraint must be followed by a period
      return false;
    }
  }
  //must have at least one constraint
  return false;
}

bool ConstrParser::constraint(){
  char c;
  //get constraint name;
  std::string cName = this->get_name();
  this->clear_space();
  c = this->next_token();

  //check for : to indicate beginning of constraint
  if(c == ':'){
    if(cName.size() == 0){
      //must have a constraint name
      return false;
    }
    else{
      //clear space after :
      this->clear_space();
      std::string pred = this->get_name();
      //check for must or must not
      if(pred == "mh" || pred == "mnh"){
        this->ast.add_constraint(pred);
        this->ast.set_name(cName);
        this->start_new_terms_list = true;
        this->opcount = 0;
        this->curr_constr = cName + "_";
        this->clear_space();
        c = this->next_token();
        if(c == '('){
          this->clear_space();
          if(this->expr()){
            this->ast.ascend();
            this->clear_space();
            c = this->next_token();
            if(c == ')'){
              this->clear_space();
              return true;
            }
            else{
              //must expression must be followed by ')'
              return false;
            }
          }
          else{
            //invalid expression
            return false;
          }
        }
        else{
          //must predicate should be followed by '('
          return false;
        }
      }
      if(pred == "if"){
        this->ast.add_constraint(pred);
        this->ast.set_name(cName);
        this->start_new_terms_list = true;
        this->opcount = 0;
        this->curr_constr = cName + "_";
        this->clear_space();
        if(this->term()){
          this->clear_space();
          pred = this->get_name();
          if(pred == "then"){
            this->clear_space();
            pred = this->get_name();
            if(pred == "mh" || pred == "mnh"){
              this->clear_space();
              c = this->next_token();
              if(c == '('){
                this->ast.add_sibling(pred);
                this->ast.advance();
                this->ast.set_name(this->curr_constr + pred);
                this->clear_space();
                this->start_new_terms_list = true;
                if(this->expr()){
                  this->clear_space();
                  c = this->next_token();
                  if(c == ')'){
                    this->clear_space();
                    return true;
                  }
                  else{
                    //must expression must be followed by ')'
                    return false;
                  }
                }
                else{
                  //'(' must be followed by an expression
                  return false;
                }
              }
              else{
                //mh/mnh must be followed by '('
                return false;
              }
            }
            else{
              //then must be followed by must expression
              return false;
            }
          }
          else{
            //if expression must be followed by "then"
            return false;
          }
        }
        else{
          //if must be followed by an expression
          return false;
        }
      }
      //constraint must start with mh, mnh, or if
      return false;
    }
  }
  //constraint name must be followed by :
  return false;
}

bool ConstrParser::expr(){
  std::string pred = this->get_name();

  if(pred == "and" || pred == "or"){
    this->opcount++;
    //save current value for when returning from terms_list()
    bool start_list = this->start_new_terms_list;
    this->start_new_terms_list = true;
    if(start_list){
      this->ast.add_child(pred);
      this->ast.descend();
    }
    else{
      this->ast.add_sibling(pred);
      this->ast.advance();
    }
    pred = this->curr_constr + pred + std::to_string(this->opcount);
    this->ast.set_name(pred);
    this->clear_space();
    bool retval = this->terms_list();
    this->ast.ascend();
    return retval;
  }
  return false;
}

bool ConstrParser::terms_list(){
  char c = this->next_token();

  if(c == '('){
    this->clear_space();
    if(this->terms()){
      return true;
    }
    else{
      //invalid terms
      return false;
    }
  }
  //expression predicate must be followed by a (
  return false;
}

bool ConstrParser::terms(){
  char c;

  if(this->term()){
    this->clear_space();
    c = this->next_token();
    if(c == ')'){
      this->start_new_terms_list = false;
      return true;
    }
    if(c == ','){
      this->clear_space();
      this->start_new_terms_list = false;
      return this->terms();
    }
    //term must be followed by a , or a )
    return false;
  }
  //invalid term
  return false;
}

bool ConstrParser::term(){
  std::string predicate = this->get_name();
  if(predicate == "and"){
    this->return_token();
    this->return_token();
    this->return_token();
    return this->expr();
  }
  if(predicate == "or"){
    this->return_token();
    this->return_token();
    return this->expr();
  }
  if(predicate.size() > 0){
    this->clear_space();
    char c = this->next_token();
    bool start_list = this->start_new_terms_list;
    this->start_new_terms_list = false;
    if(c == '('){
      if(start_list){
        this->ast.add_child("PRED");
        this->ast.descend();
      }
      else{
        this->ast.add_sibling("PRED");
        this->ast.advance();
      }
      this->ast.set_name(predicate);
      this->clear_space();
      return this->varlist();
    }
    this->return_token();
    if(start_list){
      this->ast.add_child("COMP");
      this->ast.descend();
    }else{
      this->ast.add_sibling("COMP");
      this->ast.advance();
    }
    this->ast.set_name(predicate);
    if(this->compop()){
      this->clear_space();
      std::string variable = this->get_name();
      if(variable.size() > 0){
        this->ast.add_var(variable);
        return true;
      }
      //comparison operator must be followed by a variable
      return false;
    }
    //predicate must be followed by a comp, or (
    return false;
  }
  //invalid term
  return false;
}

bool ConstrParser::varlist(){
  std::string variable = this->get_name();

  if(variable.size() > 0){
    this->ast.add_var(variable);
    this->clear_space();
    char c = this->next_token();
    if(c == ')'){
      return true;
    }
    if(c == ','){
      this->clear_space();
      return this->varlist();
    }
    //variable must be followed by ) or ,
    return false;
  }
  //no variable name given
  return false;
}

bool ConstrParser::compop(){
  char c = this->next_token();

  if(c == '='){
    this->ast.add_var("=");
    return true;
  }
  if(c == '<' || c == '>'){
    std::string op = "";
    op += c;
    c = this->next_token();
    if(c != '='){
      this->return_token();
    }else{
      op += "=";
    }
    this->ast.add_var(op);
    return true;
  }
  if(c == '!'){
    c = this->next_token();
    if(c == '='){
      this->ast.add_var("!=");
      return true;
    }
    //invalid != operator
    return false;
  }
  //invalid comparison operator
  return false;
}

void ConstrParser::build_asp(){
  this->ast.build_asp();
}

std::string ConstrParser::get_asp(){
  return this->ast.get_asp();
}
