//
// _AST_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class AST
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen AST infrastructure for logic solvers
 *
 */
#ifndef AST_H
#define AST_H
#include <vector>
#include <map>
#include <string>

class AST {
	enum nodeType {      ///< AST: type of node
		CONSTR,						 ///< nodeType: constant expression
		EXPR,							 ///< nodeType: expression
		TERM               ///< nodeType: terminal
	};

	enum constrType {    ///< AST: constant expression
		IF,                ///< constrType: If expression
		MH,                ///< constrType: Mh expression
		MNH                ///< constrType: Mnh expression
	};

	enum exprType {      ///< AST: type of expression
		AND,               ///< exprType: AND expression
		OR                 ///< exprType: OR expression
	};

	enum termType {      ///< AST: types of terminals
		PRED,              ///< termType: predicate
		COMP               ///< termType: complex
	};

	/**
   * \class ASTNode
   *
   * \ingroup CoreGen
   *
   * \brief CoreGen combinatorial logic solver AST nodes
   *
   */
	class ASTNode {
	protected:
		nodeType type;                            ///< Node type
		std::map<std::string, std::string> vars;  ///< Map of variable names
		ASTNode *terms;                           ///< Terms AST node
		ASTNode *parent;                          ///< Parent AST node
		ASTNode *next;                            ///< Next AST node
		ASTNode *prev;                            ///< Previous AST node
		std::string name;                         ///< Name of the AST node
		std::string asp;                          ///< ASP string
	public:
		/// Default constructor
		ASTNode(nodeType);

		/// Virtual destructor
		virtual ~ASTNode() {};

		/// Add a variable name and it's sort
		void add_var(std::string, std::string);

		/// Set the name of the node
		void set_name(std::string);

		/// build the node
		void build();

		/// virtual function t0 build the ASP
		virtual void build_asp();

		/// Retrieve the ASP
		std::string get_asp();

		friend class AST;
		friend class ConstrNode;
		friend class ExprNode;
		friend class TermNode;
	};

	/**
   * \class ConstrNode
   *
   * \ingroup CoreGen
   *
   * \brief CoreGen combinatorial logic solver Constraint nodes
   *
   */
	class ConstrNode : public ASTNode {
		constrType cType;               ///< constraint type
		bool built;                     ///< is this built?
	public:
		/// Default constructor
		ConstrNode(constrType);

		/// Virtualized build_asp function
		void build_asp();
	};

	/**
   * \class ExprNode
   *
   * \ingroup CoreGen
   *
   * \brief CoreGen combinatorial logic solver expression nodes
   *
   */
	class ExprNode : public ASTNode {
		exprType eType;                 ///< expression type
		bool built;                     ///< is this built?
	public:
		/// Default constructor
		ExprNode(exprType);

		/// Virtualized build_asp function
		void build_asp();

		friend class AST;
	};

	/**
   * \class TermNode
   *
   * \ingroup CoreGen
   *
   * \brief CoreGen combinatorial logic solver terminal nodes
   *
   */
	class TermNode : public ASTNode {
		termType tType;                     ///< terminal type
		int varPos;                         ///< variable position
		std::vector<std::string> posToVar;  ///< map a var to it's positionin the predicate
	public:
		/// Default constructor
		TermNode(termType);

		/// Virtualized build_asp function
		void build_asp();

		friend class AST;
	};

	// Private variables
	ASTNode *constraints;                         ///< AST node constraints
	ASTNode *curr;                                ///< current AST node
	std::map<std::string, std::string*> sortMap;  ///< maps sorts to predicates
	std::string asp;                              ///< ASP string
  std::string buildError;												///< build error text
	bool buildSucceeded;													///< did an error occur?

	// Private functions
  /// Initialize the sorted predicates
	void initialize_pred_sorts();

	/// Recursive elper for the destructor
	void destr_helper(ASTNode*);

	/// Create a new ASTNode
	ASTNode* create_node(std::string);

public:
	/// Default constructor
	AST();

	/// Default destructor
	~AST();

	/// Add the target constraint
	void add_constraint(std::string type);

	/// Add a sibling to the target node
	void add_sibling(std::string);

	/// Add a child to the target node
	void add_child(std::string);

	/// Add a variable to the target node
	void add_var(std::string);

	/// Ascend the AST
	void ascend();

	/// Descend the AST
	void descend();

	/// Advance the AST
	void advance();

	/// Reverse the AST
	void reverse();

	/// Set the name of the target node
	void set_name(std::string);

	/// Build the ASP
	void build_asp();

	/// Retrieve the ASP
	std::string get_asp();

	/// Retrieve the boolSucceeded variable
	bool build_succeeded() { return this->buildSucceeded; }

	/// Retrive the error string
	std::string build_error() { return this->buildError; }
};

#endif

// EOF
