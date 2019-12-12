//
// _Constr_Parser_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
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

#ifndef CONSTR_PARSER_H
#define CONSTR_PARSER_H
#include <string>
#include <fstream>
#include "CoreGen/CoreGenBackend/Passes/Analysis/AST.h"

class ConstrParser {
	AST ast;                    ///< AST handler
	int lineNum;                ///< Current line number
	int colNum;                 ///< Current column number
	int opcount;                ///< Current column number
	int errcount;								///< Track errors found
	std::ifstream *inFile;      ///< File handler
	std::string inFileName;     ///< Input file name
	std::string curr_constr;    ///< Current constraint
	std::string errors;					///< Error messages
	bool start_new_terms_list;  ///< Flag indicating on when to start the new terms
	bool valid_program;					///< Flag indication if the program is valid

	/// Get the next token from the parser
	char next_token();

	/// Return the token
	void return_token();

	/// Retrieve the name
	std::string get_name();

	/// Clear all the whitespace
	void clear_space();

	/// Add an error message
	void add_error(std::string);

	/// Check if word is a keyword
	bool is_keyword(std::string);

	/// Parse a constraint
	void constraint();

	/// Parse an expression
	void expr();

	/// Parse a terms list
	void terms_list();

	/// Parse terms
	void terms();

	/// parse a term
	void term();

	/// Parse a variable list
	void varlist();

	/// Parse a comparison
	void compop();

public:
	/// Default constructor
	ConstrParser();

	/// Overloaded constructor
	ConstrParser(std::string);

	/// Default destructor
	~ConstrParser();

	/// Get the input filename
	std::string get_inFileName() { return this->inFileName; }

	/// Set the input filename
	void set_inFile(std::string fn);

	/// Parse the input
	void parse();

	/// Build the ASP reprsesentation
	void build_asp();

	/// Retrieve the ASP
	std::string get_asp();

	/// Retrieve the valid_program variable
	bool valid() { return this->valid_program; }

	/// Retrieve error messages
	std::string get_errors() { return this->errors; }
};

#endif

// EOF
