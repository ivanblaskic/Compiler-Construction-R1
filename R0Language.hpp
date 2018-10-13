// (program (+ 5 (let(x (+ 5 6)) (+ x 2)))) 
// add (int(5),let(x,add(5,6),add(x,2))) 
// --> crucial step: add (int(5),add(add(5,6),2)) 
// 

#pragma once

#ifndef R0LANGUAGE_H
#define R0LANGUAGE_H

#include <iostream>
#include <string>
#include <iostream>
#include <list>

using namespace std;

static list<pair<std::string, int>> list_vars;
static int counter;

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

class varR0 : public expR0 {

public:

	varR0(string label) {
		lab = label;
	}

	string toString() {
		return "(" + this->lab + ")";
	}

	void setVal(int value) {
		this->val = value;
		pair<std::string, int> temp; 
		temp.first = lab;
		temp.second = val;
		list_vars.push_back(temp);
	}

	int eval() {
		int temp;
		cout << "Input the value for read: ";
		cin >> temp;
		setVal(temp);
		return temp;
	}

private:
	string lab;
	int val;
};

/*class letR0 : public expR0 {

public:

	letR0(varR0* label, expR0* var_val, expR0* fun) {
		this->lab = label;
		this->vr_vl = var_val;
		this->fn = fun;
	}

	int eval() {
		this->lab->setVal(this->vr_vl->eval());

	}

private:
	varR0 *lab;
	expR0 *vr_vl, *fn;
};
*/

class readR0 : public expR0 {

public:
	
	readR0(varR0* label) {
		this->lab = label;
	}

	int eval() {
		return this->lab->eval();
	}

	string toString() {
		return "(Read" + this->lab->toString() + ")";
	}

private:
	varR0 *lab;
};

#endif