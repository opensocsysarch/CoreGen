//
// _Constr_Parser_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class ConstrParser
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Integrated ASP Constaint Solver Pass Parser
 *
 */

#ifndef _CONSTR_PARSER_H_
#define _CONSTR_PARSER_H_

#include <string>
#include <fstream>
#include "AST.h"

class ConstrParser {
private:
  AST ast;                    ///< AST handler
  int lineNum;                ///< Current line number
  int colNum;                 ///< Current column number
  int opcount;                ///< Operation count
  std::ifstream *inFile;      ///< File handler
  std::string inFileName;     ///< Input file name
  std::string curr_constr;    ///< Current constraint
  bool start_new_terms_list;  ///< Flag indicating on when to start the new terms

  /// Retrieve the name
  std::string get_name();

  /// Get the next token from the parser
  char next_token();

  /// Return the token
  void return_token();

  /// Clear all the whitespace
  void clear_space();

  /// Set the constraint flag
  bool constraint();

  /// Expression flag
  bool expr();

  /// Terms list
  bool terms_list();

  /// Terms
  bool terms();

  /// Term
  bool term();

  /// Variable list
  bool varlist();

  /// Compop
  bool compop();

public:
  /// Default constructor
  ConstrParser();

  // Overloaded constructor
  ConstrParser(std::string);

  /// Default destructor
  ~ConstrParser();

  /// Get the input filename
  std::string get_inFileName(){ return this->inFileName; }

  /// Set the input filename
  void set_inFile(std::string fn);

  /// Parse the input
  bool parse();

  /// Build the ASP reprsesentation
  void build_asp();

  /// Retrieve the ASP
  std::string get_asp();
};

#endif

// EOF
