/*

	Ivan Blaskic [ @UML 4Jay's-CC-class ] 
	R0Language.hpp
	
	(+ 5 (let(x (+ 5 6)) (+ x 2))) 

	exp ::= int
		  | (read)
		  | (-exp)
		  | (+ exp exp)
		  | var
		  | (let ([var exp]) exp)

	R1  ::= (program exp)

*/
 

#pragma once

#ifndef R0LANGUAGE_H
#define R0LANGUAGE_H

#include <iostream>
#include <string>
#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

// EXP
class expR0 {

public:

	int virtual eval(list<pair<std::string, int>> *env) = 0;
	virtual string toString() = 0;

private:

};


// INT ( int )
class intR0 : public expR0 {

public:

	int eval(list<pair<std::string, int>> *env) {
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


// ( + exp exp )
class addR0 : public expR0 {

public:

	int eval(list<pair<std::string, int>> *env) {
		return (this->l->eval(env) + this->r->eval(env));
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


// - ( exp )
class negR0 : public expR0 {

public:

	int eval(list<pair<std::string, int>> *env) {
		return (-this->val->eval(env));
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


// ( READ )
class readR0 : public expR0 {

public:
	
	readR0(void) { }
	
	int eval(list<pair<std::string, int>> *env) {
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


// VAR ( string )
class varR0 : public expR0 {

public:

	varR0(string label) {
		this->lab = label;
	}

	string toString() {
		return this->lab;
	}

	int eval(list<pair<std::string, int>> *env) {
		std::list<pair<std::string, int>>::iterator it;
		for (it = (*env).begin(); it != (*env).end(); ++it) {
			if ((*it).first == this->lab) return ((*it).second);
		}
		cout << "- dealing with uninitialized variable -";
		return 0;
	}

private:

	string lab;
	int val;

};


// LET ( [var, exp] exp )
class letR0 : public expR0 {

public:

	letR0(varR0* label, expR0* var_val, expR0* fun) {
		this->lab = label;
		this->vr_vl = var_val;
		this->fn = fun;
	}

	int eval(list<pair<std::string, int>> *env) {
		list<pair<std::string, int>> *new_env = new list<pair<std::string, int>>();
		*new_env = *env;
		std::list<pair<std::string, int>>::iterator it;
		for (it = (*new_env).begin(); it != (*new_env).end(); ++it) {
			if ((*it).first == this->lab->toString()) {
				((*it).second = this->vr_vl->eval(env));
				return fn->eval(new_env);
			}
		}
		(*new_env).push_back(std::make_pair(this->lab->toString(), this->vr_vl->eval(env)));
		return fn->eval(new_env);		
	}

	string toString() {
		return "(let [" + this->lab->toString() + " " + this->vr_vl->toString() + "]" + this->fn->toString() + ")";
	}

private:

	varR0 *lab;
	expR0 *vr_vl, *fn;

};


#endif