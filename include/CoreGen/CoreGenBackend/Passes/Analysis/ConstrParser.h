#ifndef CONSTR_PARSER_H
#define CONSTR_PARSER_H
#include <string>
#include <fstream>
#include "AST.h"

class ConstrParser {
    AST ast;
    int lineNum, colNum, opcount;
    std::ifstream *inFile;
    std::string inFileName, curr_constr;
    bool start_new_terms_list;
    char next_token();
    void return_token();
    std::string get_name();
    void clear_space();
    bool constraint();
    bool expr();
    bool terms_list();
    bool terms();
    bool term();
    bool varlist();
    bool compop();
  public:
    ConstrParser();
    ConstrParser(std::string);
    ~ConstrParser();
    std::string get_inFileName(){ return this->inFileName; }
    void set_inFile(std::string fn);
    bool parse();
    void build_asp();
    std::string get_asp();
};
#endif
