#include "CoreGen/CoreGenBackend/Passes/Analysis/ConstrParser.h"
#include <iostream>
#include <cctype>
#include <exception>
#define ERROR_LIMIT 5


ConstrParser::ConstrParser() {
	this->lineNum = 1;
	this->colNum = 0;
	this->opcount = 0;
	this->errcount = 0;
	this->inFile = NULL;
	this->inFileName = "";
	this->start_new_terms_list = true;
	this->valid_program = true;
}

ConstrParser::ConstrParser(std::string fileName) {
	this->lineNum = 1;
	this->colNum = 0;
	this->opcount = 0;
	this->errcount = 0;
	this->inFile = new std::ifstream(fileName, std::ifstream::in);
	this->inFileName = fileName;
	this->start_new_terms_list = true;
	this->valid_program = true;
}

ConstrParser::~ConstrParser() {
	this->inFile->close();
	delete inFile;
}

void ConstrParser::set_inFile(std::string fileName) {
	this->inFileName = fileName;
	this->inFile = new std::ifstream(fileName, std::ifstream::in);
}

char ConstrParser::next_token() {
	this->colNum++;
	char c = this->inFile->get();
	return c;
}

void ConstrParser::return_token() {
	this->colNum--;
	this->inFile->unget();
}

std::string ConstrParser::get_name() {
	char c;
	std::string predicate = "";
	do {
		c = this->inFile->get();
		if (isalnum(c)) {
			predicate += c;
		}
	} while (isalnum(c));
	this->return_token();
	return predicate;
}

void ConstrParser::clear_space() {
	char c;
	do {
		c = this->next_token();
		if (c == '\n') {
			this->lineNum++;
			this->colNum = 0;
		}
	} while (isspace(c));
	this->return_token();
}

void ConstrParser::add_error(std::string err) {
	if (this->errcount == ERROR_LIMIT) {
		this->errors = "Max errors (" + std::to_string(ERROR_LIMIT) + ") reached\n" + this->errors;
		throw std::exception();
	}
	this->errors += "line " + std::to_string(this->lineNum);
	this->errors += " col " + std::to_string(this->colNum);
	this->errors += ": " + err + "\n";
	this->errcount++;
}

bool ConstrParser::is_keyword(std::string word) {
	if (word == "if") {
		return true;
	}
	else if (word == "then") {
		return true;
	}
	else if (word == "mh") {
		return true;
	}
	else if (word == "mnh") {
		return true;
	}
	else if (word == "and") {
		return true;
	}
	else if (word == "or") {
		return true;
	}

	return false;
}

void ConstrParser::parse() {
	//clear whitespace
	this->clear_space();
	//check if there are more constraints to parse
	if (this->inFile->peek() >= 0) {
		//check constraint
		try {
			this->constraint();
		}
		catch (std::exception e) {
			return;
		}
		char c = this->next_token();
		if (c != '.') {
			this->valid_program = false;
			this->add_error("Expected a '.'");
			this->return_token();
		}
		this->parse();
	}
}

void ConstrParser::constraint() {
	char c;
	//get constraint name;
	std::string cName = this->get_name();
	if (cName.size() == 0) {
		this->valid_program = false;
		this->add_error("Expected a constraint name");
	}
	else if (this->is_keyword(cName)) {
		this->valid_program = false;
		this->add_error("Constraint name can't be a keyword");
		this->return_token();
		this->return_token();
	}
	this->clear_space();
	c = this->next_token();

	//check for : to indicate beginning of constraint

	if (c != ':') {
		this->valid_program = false;
		this->add_error("Expected a ':'");
		this->return_token();
	}
	//clear space after :
	this->clear_space();
	std::string pred = this->get_name();
	//check for must or must not
	if (pred != "mh" && pred != "mnh" && pred != "if") {
		this->valid_program = false;
		this->add_error("Expected 'mh', 'mnh', or 'if");
	}
	//add constraint to AST
	if (this->valid_program) {
		this->ast.add_constraint(pred);
		this->ast.set_name(cName);
	}
	this->start_new_terms_list = true;
	this->opcount = 0;
	this->curr_constr = cName + "_";
	this->clear_space();
	// check for "if"
	if (pred == "if") {
		this->term();
		this->clear_space();
		pred = this->get_name();
		if (pred != "then") {
			this->valid_program = false;
			this->add_error("Expected 'then'");
		}
		this->clear_space();
		if (this->inFile->peek() != '(') {
			pred = this->get_name();
			if (pred != "mh" && pred != "mnh") {
				this->valid_program = false;
			}
		}
		if (this->valid_program) {
			this->ast.add_sibling(pred);
			this->ast.advance();
			this->ast.set_name(this->curr_constr + pred);
		}
		this->start_new_terms_list = true;
	}
	if (pred != "mh" && pred != "mnh") {
		this->valid_program = false;
		this->add_error("Expected 'mh' or 'mnh'");
	}
	c = this->next_token();
	if (c != '(') {
		this->valid_program = false;
		this->add_error("Expected '('");
		this->return_token();
	}
	this->clear_space();
	this->term();
	if (this->valid_program) {
		this->ast.ascend();
	}
	this->clear_space();
	c = this->next_token();
	if (c != ')') {
		this->valid_program = false;
		this->add_error("Expected ')'");
		this->return_token();
	}
}

void ConstrParser::expr() {
	std::string pred = this->get_name();

	
	this->opcount++;
	//save current value for when returning from terms_list()
	bool start_list = this->start_new_terms_list;
	this->start_new_terms_list = true;
	if (this->valid_program) {
		if (start_list) {
			this->ast.add_child(pred);
			this->ast.descend();
		}
		else {
			this->ast.add_sibling(pred);
			this->ast.advance();
		}
	}
	pred = this->curr_constr + pred + std::to_string(this->opcount);
	if (this->valid_program) {
		this->ast.set_name(pred);
	}
	this->clear_space();
	this->terms_list();
	if (this->valid_program) {
		this->ast.ascend();
	}
}

void ConstrParser::terms_list() {
	char c = this->next_token();

	if (c != '(') {
		this->valid_program = false;
		this->return_token();
		this->add_error("and/or must be followed by a '('.");
	}
	this->clear_space();
	this->terms();
		
}

void ConstrParser::terms() {
	char c;

	this->term();
	this->clear_space();
	c = this->next_token();
	this->start_new_terms_list = false;
	if (c == ',') {
		this->clear_space();
		this->terms();
	}
	else if (c != ')') {
		this->valid_program = false;
		this->return_token();
		this->add_error("Term bust be followed by a ',' or ')'");
	}
}

void ConstrParser::term() {
	std::string predicate = this->get_name();
	if (predicate == "and") {
		this->return_token();
		this->return_token();
		this->return_token();
		this->expr();
	}
	else if (predicate == "or") {
		this->return_token();
		this->return_token();
		this->expr();
	}
	else {
		if (predicate.size() <= 0) {
			this->valid_program = false;
			this->add_error("Missing predicate/variable name");
		}
		this->clear_space();
		char c = this->next_token();
		bool start_list = this->start_new_terms_list;
		this->start_new_terms_list = false;
		if (c == '(') {
			if (this->valid_program) {
				if (start_list) {
					this->ast.add_child("PRED");
					this->ast.descend();
				}
				else {
					this->ast.add_sibling("PRED");
					this->ast.advance();
				}
				this->ast.set_name(predicate);
			}
			this->clear_space();
			this->varlist();
		}
		else {
			this->return_token();
			if (this->valid_program) {
				if (start_list) {
					this->ast.add_child("COMP");
					this->ast.descend();
				}
				else {
					this->ast.add_sibling("COMP");
					this->ast.advance();
				}
				this->ast.set_name(predicate);
			}
			this->compop();
			this->clear_space();
			std::string variable = this->get_name();
			if (variable.size() > 0) {
				if (this->valid_program) {
					this->ast.add_var(variable);
				}
			}
			else {
				this->valid_program = false;
				this->add_error("Comparison operator must be followed by variable/constant.");
			}
		}
	}
}

void ConstrParser::varlist() {
	std::string variable = this->get_name();

	if (variable.size() <= 0) {
		this->valid_program = false;
		this->add_error("Expected a variable/constant.");
	}
	if (this->valid_program) {
		this->ast.add_var(variable);
	}
	this->clear_space();
	char c = this->next_token();
	if (c == ',') {
		this->clear_space();
		this->varlist();
	}
	else if (c != ')') {
		this->valid_program = false;
		this->add_error("Expected ',' or ')' following variable/constant.");
		if (isalpha(c)) {
			this->return_token();
		}
		this->varlist();
	}
}

void ConstrParser::compop() {
	char c = this->next_token();

	if (c == '=') {
		if (this->valid_program) {
			this->ast.add_var("=");
		}
	}
	else if (c == '<' || c == '>') {
		std::string op = "";
		op += c;
		c = this->next_token();
		if (c != '=') {
			this->return_token();
		}
		else {
			op += "=";
		}
		if (this->valid_program) {
			this->ast.add_var(op);
		}
	}
	else if (c == '!') {
		c = this->next_token();
		if (c == '=') {
			if (this->valid_program) {
				this->ast.add_var("!=");
			}
		}
		else {
			this->valid_program = false;
			this->add_error("Expected '!='.");
			this->return_token();
		}
	}
	else {
		this->valid_program = false;
		this->add_error("Expected comparison operator.");
		this->return_token();
	}
}

void ConstrParser::build_asp() {
	if (this->valid_program) {
		this->ast.build_asp();
		if (!this->ast.build_succeeded()) {
			this->valid_program = false;
			this->errors = this->ast.build_error();
		}
	}
}

std::string ConstrParser::get_asp() {
	if (this->valid_program) {
		return this->ast.get_asp();
	}
	else {
		return this->errors;
	}
}
