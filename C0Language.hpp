/*

	Ivan Blaskic [ @UML 4Jay's-CC-class ]
	C0Language.hpp

	arg		::= int 
			  | var

	exp		::= arg 
		      | (read) 
			  | (- arg)
			  | (+ arg arg)

	stmt	::= (assign var exp)
		      | (return arg)

	C0   ::= (program (var*) stmt+)

	C Code: progC0 
		- receives list of stmts initialized in main
		- executes in a way that it receives all variables necessary for execution
			- goes statement by statement
				- evaluates sharing list of variables

	R --> C:	flatten in the deepest node 
					--> creates new list of statements and variables when returning 
				flatten in operation node
					--> adds new statement to existing set of statements received from arguments

*/

#pragma once

#include <stdlib.h>
#include <string>
#include <list>
#include <utility>
#include <iostream>
#include <iterator>
#include <memory>

using namespace std;

#ifndef C0LANGUAGE_H
#define C0LANGUAGE_H

static list<pair<string, int>> variables;


// (exp)
class expC0 {

public:

	virtual int eval(list<pair<string, int>> *vars_) = 0;
	string virtual toString() = 0;

private:

};


// (argument)
class argC0 : public expC0 {

public:

	virtual int eval(list<pair<string, int>> *vars_) = 0;
	string virtual toString() = 0;

private:

};


// (int) <-- argument
class intC0 : public argC0 {

public:

	intC0(int value_) {
		this->value = value_;
	}

	int eval(list<pair<string,int>> *vars_) {
		return this->value;
	}

	string toString() {
		return "\t(" + to_string(this->value) + ")";
	}

private:

	int value;

};


// (var) <-- argument
class varC0 : public argC0 {

public:

	varC0(string name_) {
		this->name = name_;
	}

	int setVal(int value_, list<pair<string, int>> *vars_) {
		this->vars = vars_;
		for (list<pair<string, int>>::iterator it = this->vars->begin(); it != this->vars->end(); it++) {
			if ((*it).first == this->name) {
				(*it).second = value_;
				return 0;
			}
		}
		cout << "\tVariable: " << this->name << " is not found.\n";
		return 1;
	}

	int eval(list<pair<string, int>> *vars_) {
		this->vars = vars_;
		for (list<pair<string, int>>::iterator it = this->vars->begin(); it != this->vars->end(); it++) {
			if ((*it).first == this->name) {
				return (*it).second;
			}
		}
		cout << "\tVariable: " << this->name << " is not found.\n";
		return 1;
	}

	string toString() {
		return "\t(" + this->name + ")";
	}

private:

	list<pair<string, int>> *vars;
	string name;

};


// (read) <-- exp
class readC0 : public expC0 {

public:

	readC0() {}

	int eval(list<pair<string, int>> *vars_) {
		cout << "Enter the integer value for (read): ";
		cin >> this->value;
		cout << "\n";
		return this->value;
	}

	string toString() {
		return "\t(read)";
	}

private:

	int value;
	list<pair<string, int>> *vars;

};


// (- arg) <-- exp
class negC0 : public expC0 {

public:

	negC0(argC0 *arg_) {
		this->arg = arg_;
	}

	int eval(list<pair<string, int>> *vars_) {
		this->vars = vars_;
		return (-this->arg->eval(this->vars));
	}

	string toString() {
		return "\t(- " + this->arg->toString() + ")";
	}

private:

	list<pair<string, int>> *vars;
	argC0 *arg;

};


// (+ arg arg) <-- exp
class addC0 : public expC0 {

public:

	addC0(argC0 *left_, argC0 *right_) {
		this->left = left_;
		this->right = right_;
	}

	int eval(list<pair<string, int>> *vars_) {
		this->vars = vars_;
		return (this->left->eval(this->vars) + this->right->eval(this->vars));
	}

	string toString() {
		return "\t(+ " + this->left->toString() + " " + this->right->toString() + ")";
	}

private:

	argC0 *left, *right;
	list<pair<string, int>> *vars;

};


// (stmt)
class stmtC0 {

public:

	virtual int eval(list<pair<string, int>> *vars_) = 0;
	virtual string toString() = 0;

private:

};


// (assign var exp) <-- stmt
class assignC0 : public stmtC0 {

public:

	assignC0(varC0 *var_, expC0 *exp_) {
		this->var = var_;
		this->exp = exp_;
	}

	int eval(list<pair<string, int>> *vars_) {
		this->vars = vars_;
		if (this->var->setVal(this->exp->eval(this->vars), this->vars) == 0) {
			return 0;
		}
		else {
			cout << "\tError setting value for: " << this->var->toString() << "\n";
			return 1;
		}
	}

	string toString() {
		return "\t(assign\t" + this->var->toString() + this->exp->toString() + ")\n";
	}

private:

	list<pair<string, int>> *vars;
	varC0 *var;
	expC0 *exp;

};


// (return arg) <-- stmt
class retC0 : public stmtC0 {

public:

	retC0(argC0 *arg_) {
		this->arg = arg_;
	}

	int eval(list<pair<string, int>> *vars_) {
		this->vars = vars_;
		cout << "\t\t>> Return Value is " << this->arg->eval(this->vars) << " for following return statement <<\n";
		return 0;
	}

	string toString() {
		return "\t(return " + this->arg->toString() + ")\n";
	}

private:

	argC0 *arg;
	list<pair<string, int>> *vars;

};


// C0
class C0 {

public:

	virtual void execute (list<pair<string, int>> *vars_) = 0;

private:

};


// (program (var*) stmt+) <-- C0
class progC0 : public C0 {

public:

	progC0 (list<std::unique_ptr<stmtC0>> *stmts_){
		this->stmts = stmts_;
	}

	// called from deepest node returns newly initialized program C0 with return variable
	progC0(varC0 *var_) {
		this->stmts = new list<std::unique_ptr<stmtC0>>();
		this->vars = new list<pair<string, int>>{ (make_pair(var_->toString(), 0)) };
		this->ret_arg = var_;
	}

	progC0() {
		this->stmts = new list<std::unique_ptr<stmtC0>>();
		this->vars = new list<pair<string, int>>();
	}

	// called from deepest node returns newly initialized program C0 with return value of int
	progC0(intC0 *int_) {
		this->stmts = new list<std::unique_ptr<stmtC0>>();
		this->vars = new list<pair<string, int>>();
		this->ret_arg = int_;
	}

	progC0(argC0 *ass_arg, varC0 *var_, progC0 *prog_) {
		this->stmts = prog_->stmts;
		this->vars = prog_->vars;
		this->stmts->emplace_back(new assignC0(var_, ass_arg));
		this->ret_arg = var_;
	}

	// called after flattening arguments in operations so we have program with assignment statements that happened in them
	progC0(expC0 *exp_,varC0 *var_, progC0 *merging_one_, progC0 *merging_two_) {
		this->stmts = merging_one_->stmts;
		this->stmts->emplace_back(merging_two_->stmts);
		this->vars = merging_one_->vars;
		this->vars->emplace_back(merging_two_->vars);
		this->stmts->emplace_back(new assignC0(var_, exp_));
		this->ret_arg = var_;
	}

	void execute() {
		cout << "\nProgram:\n\n";
		for (std::list<std::unique_ptr<stmtC0>>::iterator it = this->stmts->begin(); it != this->stmts->end(); ++it) {
			if ((*it)->eval(this->vars) == 0) {
				cout << "\t" << (*it)->toString();
			}
			else {
				cout << "\n\tError executing program.\n\tCheck statement: " << (*it)->toString() << "\n";
			}
		}
		stmtC0 *ret_var = new retC0(this->ret_argument());
		cout << "\t" << ret_var->toString();
		cout << "\n\tExecution is done.\n\n" << "\nMemory:\n\n" << "\tVariable\tValue\n";
		for (std::list<pair<std::string, int>>::iterator it = this->vars->begin(); it != this->vars->end(); ++it) {
			cout << "\t" << (*it).first << "\t\t" << (*it).second << "\n";
		}
		cout << "\n";
	}

	void execute(list<pair<string, int>> *vars_) {
		this->vars = vars_;
		cout << "\nProgram:\n\n";
		for (std::list<std::unique_ptr<stmtC0>>::iterator it = this->stmts->begin(); it != this->stmts->end(); ++it) {
			if ((*it)->eval(vars) == 0) {
				cout << "\t" << (*it)->toString();
			}
			else {
				cout << "\n\tError executing program.\n\tCheck statement: " << (*it)->toString() << "\n";
			}
		}
		stmtC0 *ret_var = new retC0(this->ret_argument());
		cout << "\t" << ret_var->toString();
		cout << "\n\tExecution is done.\n\n" << "\nMemory:\n\n" << "\tVariable\tValue\n";
		for (std::list<pair<std::string, int>>::iterator it = vars->begin(); it != vars->end(); ++it) {
			cout << "\t" << (*it).first << "\t\t" << (*it).second << "\n";
		}
		cout << "\n";
	}

	argC0* ret_argument(void) {
		return this->ret_arg;
	}

private:

	list<pair<string, int>> *vars;
	list<std::unique_ptr<stmtC0>> *stmts;
	argC0 *ret_arg;

};


#endif // !C0LANGUAGE_H

