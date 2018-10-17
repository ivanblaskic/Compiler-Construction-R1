// (program (+ 5 (let(x (+ 5 6)) (+ x 2)))) 
// add (int(5),let(var(x),add(int(5),int(6)),add(var(x),int(2))))  --> add (int(5), let(var(x),int(11),add(var(x),int(2)))	--> list: (x, 11)  --> 
// --> crucial step: add (int(5),add(add(5,6),2)) 
/*

	exp ::= int
		  | (read)
		  | (-exp)
		  | (+ exp exp)
		  | var
		  | (let ([var exp]) exp)
	R1  ::= (program exp)

	register ::= rsp
			   | rbp
			   | rax
			   | rbx
			   | rcx
			   | rdx
			   | rsi
			   | rdi
			   | r8
			   | r9
			   | r10
			   | r11
			   | r12
			   | r13
			   | r14
			   | r15
	arg   ::= (int int)
			| (reg register)
			| (deref register int)
	instr ::= (addq arg arg)
			| (subq arg arg)
			| (movq arg arg)
			| (retq)
			| (negq arg)
			| (callq label)
			| (pushq arg)
			| (popq arg)
	X860  ::= (program int instr+)

	arg  ::= int | var
	exp  ::= arg | (read) | (-arg)
		   | (+ arg arg)
	stmt ::= (assign var exp)
		   | (return arg)
	C0   ::= (program (var*) stmt+)

	uniquify:
		- structurally recursive f-ion
		  translating input AST into an
		  output AST - traversing entire
		  AST

	flatten

*/
// 

#pragma once

#ifndef R0LANGUAGE_H
#define R0LANGUAGE_H

#include <iostream>
#include <string>
#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

static list<pair<std::string, int>> list_vars;

static int lookup(string var) {
	std::list<pair<std::string, int>>::iterator it;
	for (it = list_vars.begin(); it != list_vars.end(); ++it) {
		if ((*it).first == var) return ((*it).second);
	}
	return 0;
}

class expR0 {

public:
	int virtual eval() = 0;
	virtual string toString() = 0;
private:

};

class intR0 : public expR0 {

public:

	int eval() {
		return this->val;
	}

	string toString() {
		return to_string(this->val);
	}

	intR0(int value) {
		this->val = value;
	}

private:
	int val;

};

class addR0 : public expR0 {

public:

	int eval() {
		return (this->l->eval() + this->r->eval());
	}

	string toString() {
		return "(+ " + this->l->toString() + " " + this->r->toString() + ")";
	}

	addR0(expR0* left, expR0* right) {
		this->l = left;
		this->r = right;
	}

private:
	expR0 *l, *r;

};

class negR0 : public expR0 {

public:

	int eval() {
		return (-this->val->eval());
	}

	string toString() {
		return "(- " + this->val->toString() + ")";
	}

	negR0(expR0* value) {
		this->val = value;
	}

private:

	expR0 *val;

};

class readR0 : public expR0 {

public:
	
	readR0(void) { }
	
	int eval() {
		cout << "Input the value for read: ";
		cin >> this->val;
		return this->val;
	}
	
	string toString() {
		return "(read)";
	}

private:
	
	int val;
	
};

class varR0 : public expR0 {

public:

	varR0(string label) {
		this->lab = label;
	}

	string toString() {
		return this->lab;
	}

	void setVal(int value) {
		this->val = value;
		if (lookup(this->lab) == 0) {
			std::pair<std::string, int> new_value = std::make_pair(this->lab, this->val);
			list_vars.push_back(new_value);
		}

	}

	int getVal() {
		if (lookup(this->lab) != 0) {
			return lookup(this->lab);
		}
		else {
			cout << "Error trying to calculate with uninitialized variable: " << this->lab << ".";
			return 0;
		}
	}

	int eval() {
		this->val = getVal();
		return this->val;
	}

private:
	string lab;
	int val;
};

class letR0 : public expR0 {

public:

	letR0(varR0* label, expR0* var_val, expR0* fun) {
		this->lab = label;
		this->vr_vl = var_val;
		this->fn = fun;
	}

	int eval() {
		this->lab->setVal(this->vr_vl->eval());
		return this->fn->eval();					
	}

	string toString() {
		return "(let (" + this->lab->toString() + " " + this->vr_vl->toString() + ")" + this->fn->toString() + ")";
	}

private:

	varR0 *lab;
	expR0 *vr_vl, *fn;
};


#endif