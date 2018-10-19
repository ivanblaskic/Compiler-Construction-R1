/*

main:
	movq $10, %rax 
	addq $32, %rax 
	movq %rax, %rdi 
	callq print_int 
	movq $0, %rax retq

register  ::= rsp
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
arg   ::= $int
		| %reg
		| int(%reg)
instr ::= (addq arg arg)
		| (subq arg arg)
		| (movq arg arg)
		| (retq)
		| (negq arg)
		| (callq label)
		| (pushq arg)
		| (popq arg)
X860  ::= (instr+)

programX0 = list <instrX0>;

programX0 *main = new programX0(	0,
									(new movqX0(	new intX0(10),	RAX ),
									new addqX0(	new intX0(32),	RAX ),
									new movqX0( RAX,			RDI ),
									new callX0(	new lblX0( "print_int" ) ),	--> lista <lbl, adr>
									new movqX0( new intX0(0),	RAX ),
									new retqX0( ))
								);

main.execute();

cout << "Program: " << main.toString() << " = " << main.printRegs() << ".\n\n";

toString --> through list iterator

*/


#pragma once

#ifndef X0LANGUAGE_H
#define X0LANGUAGE_H

#include <stdlib.h>
#include <string>
#include <iostream>
#include <list>
#include <utility>
#include <iterator>

using namespace std;

static list<pair<std::string, int>> labels_init;

static int pcnt;

// or #define RAX value
static list<pair<std::string, int>> register_list = {
	std::make_pair("rax", 0),
	std::make_pair("rbx", 0),
	std::make_pair("rcx", 0),
	std::make_pair("rdx", 0),
	std::make_pair("rsi", 0),
	std::make_pair("rdi", 0),
	std::make_pair("r8", 0),
	std::make_pair("r9", 0),
	std::make_pair("r10", 0),
	std::make_pair("r11", 0),
	std::make_pair("r12", 0),
	std::make_pair("r13", 0),
	std::make_pair("r14", 0),
	std::make_pair("r15", 0),
	std::make_pair("rsp", 0),
	std::make_pair("rbp", 0)
};


class instrX0 {

public:

	virtual void eval() = 0;
	virtual string toString() = 0;

private:

};


class programX0 {

public:

	programX0(int pcnt_, list<instrX0> *code_) {
		this->code = code_;
		pcnt = pcnt_;
	}

	void execute() {
		for (std::list<instrX0>::iterator it = this->code->begin(); it != this->code->end(); ++it) {
			(*it).eval();
			pcnt += 8;
		}
		pcnt = 0;
	}

	void toString() {
		for (std::list<instrX0>::iterator it = this->code->begin(); it != this->code->end(); ++it) {
			cout << "\t" << pcnt << ":\t" << (*it).toString();
			pcnt += 8;
		}
	}

private:

	list <instrX0> *code;

};


class movqX0 : public instrX0 {

public:

	movqX0(argX0* src_,argX0* dest_) {
		this->src = src_;
		this->dest = dest_;
	}

	void eval() {
		//this->dest->writeValue(this->src->readValue());
	}

	string toString() {
		return "\tmovq\t\t" + this->src->toString() + ",\t" + this->dest->toString() + "\n";
	}

private:

	argX0 *src, *dest;

};


class addqX0 : public instrX0 {

public:

	addqX0(argX0* src_,argX0* dest_) {
		this->src = src_;
		this->dest = dest_;
	}

	void eval() {
		//this->dest->writeValue(this->src->readValue()+this->dest->readValue
	}

	string toString() {
		return "\taddq\t\t" + this->src->toString() + ",\t" + this->dest->toString() + "\n";
	}

private:
	argX0 *src, *dest;
};


/*
class new_lblX0 : public instrX0 {

public:

	new_lblX0 (lblX0 *init_, instrX0 *instr_) {
		this->init = init_;
		this->instr = intr_;
		//input in the list of int address - lbl name the value of the PCNT for this instruction
		//first pass / second pass
	}

	void evaluate() {
		this->instr->evaluate();
	}

	string toString() {
		return this->init->toString() + ": " + this->instr->toString();
	}

	//keep in mind for toString that pcnt could be printed out from the class itself

private:
	lblX0 *init;
	instrX0 *instr;
	
*/


class callX0 : public instrX0 {

public:

	callX0(lblX0 *name) {
		this->init = name;
	}

	void eval() {
		//lookup for the address in the list and then write function to skip instructions
		//if (!( goto_label(this->init->getName())) ) {
		//	cout << "Error Label: " << this->init->toString() << " cannot be found.";
		//}
	}

	string toString() {
		return "\tcallq\t\t" + this->init->toString() + "\n";
	}

private:

	lblX0 *init;

};

class retqX0 : public instrX0 {

public:

	retqX0() {
		//check if (rax == 0), if successful run
	}

	void eval() {
		//what does actually has to be implemented for this one
	}

	string toString() {
		return "\tretq\n";
	}

private:

	//int address;

};


class lblX0 : public instrX0 {

public:

	lblX0(string name) {
		this->name = name;
		//initialize it to PC value at the time in the labels' list
	}

	void eval() {

	}

	string toString() {
		return  this->name;
	}

private:

	string name;

};

class argX0 {

public:

	int virtual eval() = 0;

	string toString() {

	}

private:

};

class intX0 : public argX0 {

public:

	intX0(int value_) {
		this->value = value_;
	}

	int eval() {
		return this->value;
	}

	string toString() {
		return "$" + this->value;
	}

private:

	int value;

};

class regX0 : public argX0 {

public:

	regX0(string name_) {
		this->name = name_;
		// this->value = getVal(lookupList(this->name));
	}

	int evaluate() {
		return this->value;
	}

	void writeValue(int value_) {
		this->value = value_;
		// setVal(this-> name, this->value);
	}

	string toString() {
		return "%" + this->name;
	}

private:

	int value;
	string name;

};

class intRegX0 : public argX0 {

public:

	intRegX0(int offset_, regX0 *reg_) {
		this->offset = offset_;
		this->reg = reg_;
	}

	int evaluate() {
		return this->value;
	}

	void writeValue() {
		//...
	}

	string toString() {
			
	}

private:
	
	string name;
	int offset, value;
	regX0 *reg;

};






#endif // !X0LANGUAGE_H

