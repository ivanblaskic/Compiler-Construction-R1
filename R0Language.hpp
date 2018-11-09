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
#include <memory>

using namespace std;

static int cnt = 0;


class varR0;


// EXP
class expR0 {

public:

	int virtual eval(list<pair<std::string, int>> *env) = 0;
	virtual string toString() = 0;
	virtual expR0* uniquify(list<pair<unique_ptr<varR0>, unique_ptr<varR0>>> *mapp) = 0;

private:

};


// VAR ( string )
class varR0 : public expR0 {

public:

	varR0(string label) {
		this->lab = label;
	}

	int get_val(void) {
		return this->val;
	}

	void setString(string temp) {
		this->lab = temp;
	}

	varR0(varR0 *old_var) {
		this->lab = old_var->lab + "_" + to_string(cnt++);
		this->val = old_var->get_val();
	}

	string toString() {
		return this->lab;
	}

	int eval(list<pair<std::string, int>> *env) {
		std::list<pair<std::string, int>>::iterator it;
		for (it = (*env).begin(); it != (*env).end(); ++it) {
			if ((*it).first == this->lab) {
				this->val = (*it).second;
				return this->val;
			}
		}
		cout << "- dealing with uninitialized variable -";
		return 0;
	}

	string get_lab() {
		return this->lab;
	}

	varR0* ret_val() {
		return this;
	}

	virtual expR0* uniquify(list<pair<unique_ptr<varR0>, unique_ptr<varR0>>> *mapp) {
		this->map = mapp;
		std::list<pair<unique_ptr<varR0>, unique_ptr<varR0>>>::iterator it;
		for (it = (*map).begin(); it != (*map).end(); ++it) {
			if ((*it).first->toString() == this->toString()) {				
				this->setString((*it).second->toString());
				return this;
			}
		}
		cout << "No mapping - error occured.";
		return this;
	}

private:

	list<pair<unique_ptr<varR0>, unique_ptr<varR0>>> *map;
	string lab;
	int val;

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

	virtual expR0* uniquify(list<pair<unique_ptr<varR0>, unique_ptr<varR0>>> *mapp) {
		return this;
	}

private:

	int val;

};


// ( + exp exp )
class addR0 : public expR0 {

public:

	int eval(list<pair<std::string, int>> *env) {
		return (this->left->eval(env) + this->right->eval(env));
	}

	string toString() {
		return "(+ " + this->left->toString() + " " + this->right->toString() + ")";
	}

	addR0(expR0* left_, expR0* right_) {
		this->left = left_;
		this->right = right_;
	}

	virtual expR0* uniquify(list<pair<unique_ptr<varR0>, unique_ptr<varR0>>> *mapp) {
		map = mapp;
		return new addR0(this->left->uniquify(map), this->right->uniquify(map));
	}

private:

	expR0 *left, *right;
	list<pair<unique_ptr<varR0>, unique_ptr<varR0>>> *map;

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

	virtual expR0* uniquify(list<pair<unique_ptr<varR0>, unique_ptr<varR0>>> *mapp) {
		map = mapp;
		return new negR0(this->val->uniquify(map));
	}

private:

	expR0 *val;
	list<pair<unique_ptr<varR0>, unique_ptr<varR0>>> *map;

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

	virtual expR0* uniquify(list<pair<unique_ptr<varR0>, unique_ptr<varR0>>> *mapp) {
		return this;
	}

private:
	
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
		int temp = this->vr_vl->eval(env);
		(*env).push_back(std::make_pair(this->lab->toString(), temp));
		return fn->eval(env);
	}

	string toString() {
		return "(let [" + this->lab->toString() + " " + this->vr_vl->toString() + "]" + this->fn->toString() + ")";
	}

	virtual expR0* uniquify(list<pair<unique_ptr<varR0>, unique_ptr<varR0>>> *mapp) {
		this->map = mapp;
		varR0 *new_variable = new varR0(this->lab);										// variable with label
		std::list<pair<unique_ptr<varR0>, unique_ptr<varR0>>>::iterator it;				// let's see if we have one with the same init name
		for (it = (*map).begin(); it != (*map).end(); ++it) { 
			if ((*it).first->toString() == this->lab->toString()) {						// if we do
				this->new_map = new list<pair<unique_ptr<varR0>, unique_ptr<varR0>>>(); // create new mapping list
				new_map = map;															// same as the one before 
				std::list<pair<unique_ptr<varR0>, unique_ptr<varR0>>>::iterator it1;	// find the same named var
				for (it1 = (*new_map).begin(); it1 != (*new_map).end(); ++it1) {		
					if ((*it1).first->toString() == this->lab->toString()) {
						(*it1).second = unique_ptr<varR0>(new_variable);				// change it's matching var to new_variable
					}
				}
				return new letR0(new varR0(new_variable->toString()), this->vr_vl->uniquify(new_map), this->fn->uniquify(new_map));	//return let with new mapping
			}
		}																						// if that's not the case enter pair in mapping
		(*map).emplace_back(std::make_pair(unique_ptr<varR0>(new varR0(this->lab->toString())), unique_ptr<varR0>(new_variable)));
		return new letR0(new varR0(new_variable->toString()), this->vr_vl->uniquify(map), this->fn->uniquify(map));	// return regular mapping with let
		this->lab = new_variable;
	}

private:
	
	list<pair<unique_ptr<varR0>, unique_ptr<varR0>>> *new_map;
	varR0 *lab;
	expR0 *vr_vl, *fn;
	list<pair<unique_ptr<varR0>, unique_ptr<varR0>>> *map;

};


#endif