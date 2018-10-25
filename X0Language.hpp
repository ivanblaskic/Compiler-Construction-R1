/*

	Ivan Blaskic [ @UML 4Jay's-CC-class ]
	X0Language.hpp

	main:	movq $10, %rax 
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
#include <memory>

using namespace std;


// register ::= rsp | rbp | rax | rbx | rcx | rdx | rsi | rdi | r8 | r9 | r10 | r11 | r12 | r13 | r14 | r15
static list<pair<std::string, int>> regs = {
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


// program counter
static int pcnt;


// arg ::= $int | %reg | int(%reg)
class argX0 {

public:

	int virtual eval(list<pair<std::string,int>> *regs) = 0;
	void virtual setValue(int value) = 0;
	string virtual getName(void) = 0;
	string virtual toString(void) = 0;

private:

};


// $int <-- arg
class intX0 : public argX0 {

public:

	void setValue(int value_) {
		this->value = value_;
	}

	string getName(void) {
		return "Integer type has no name.";
	};

	intX0(int value_) {
		this->value = value_;
	}

	int eval(list<pair<std::string, int>> *regs) {
		return this->value;
	}

	string toString() {
		return "$" + to_string(this->value);
	}

private:

	int value;

};


// %reg <-- arg
class regX0 : public argX0 {

public:

	regX0(string name_) {
		this->name = name_;
		this->registers = &regs;
		// this->value = getVal(lookupList(this->name));
	}

	int eval(list<pair<std::string, int>> *regs) {
		for (std::list<pair<std::string, int>>::iterator it = this->registers->begin(); it != this->registers->end(); ++it) {
			if ((*it).first == this->name) {
				this->setValue((*it).second);
				return this->value;
			}
		}
		return 1;
	}

	void setValue(int value_) {
		this->value = value_;
	}

	string getName() {
		return this->name;
		// setVal(this-> name, this->value);
	}

	string toString() {
		return "%" + this->name;
	}

private:

	int value;
	string name;
	list<pair<std::string, int>> *registers;

};


/*
// int (%reg)
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
*/


//instruction ::= (addq arg arg) | (subq arg arg) | (movq arg arg) | (retq) | (negq arg) | (callq label) | (pushq arg) | (popq arg)
class instrX0 {

public:

	virtual int eval(list<pair<std::string, int>> *regs) {

		return 10;

	}

	virtual string toString() {

		return "Error InstrX0 Class had to do Print.\n";

	}

private:

};


// (retq) <-- instruction (function marking success with storing 0 in %rax)
class retqX0 : public instrX0 {

public: 

	retqX0() {}

	int eval(list<pair<std::string, int>> *regs) {
		this->registers = regs;
		for (std::list<pair<std::string, int>>::iterator it = this->registers->begin(); it != this->registers->end(); ++it) {
			if ((*it).first == "rax") {
				(*it).second = 0;
				success = true;
				return 0;
			}
		}
		return 6;
	}

	string toString() {
		if (success) {
			return "\tretq\n\n\t\t('The code was executed successfully')\n\n";
		}
		else {
			return "\tretq\n";
		}
	}

private:

	bool success = false;
	list<pair<std::string, int>> *registers;

};


// (callq print_int) <-- instruction (function printing out %rdi)
class callqX0 : public instrX0 {

public:

	callqX0() {
	}

	int eval(list<pair<std::string, int>> *regs) {
		this->registers = regs;
		for (std::list<pair<std::string, int>>::iterator it = this->registers->begin(); it != this->registers->end(); ++it) {
			if ((*it).first == "rdi") {
				value = (*it).second;
				return 0;
			}
		}
		return 5;
	}

	string toString() {
		return ("\tcallq\t\tprint_int\n\n\t\t('Value at %rdi: " + to_string(this->value) + "')\n\n");
	}


private:

	int value;
	list<pair<std::string, int>> *registers;

};


// (negq arg) <-- instruction
class negqX0 : public instrX0 {

public:

	negqX0(argX0* dest_) {
		this->dest = dest_;
	}

	int eval(list<pair<std::string, int>> *regs) {
		this->registers = regs;
		for (std::list<pair<std::string, int>>::iterator it = this->registers->begin(); it != this->registers->end(); ++it) {
			if ((*it).first == this->dest->getName()) {
				((*it).second = - this->dest->eval(this->registers));
				return 0;
			}
		}
		return 4;
	}

	string toString() {
		return "\tnegq\t\t" + this->dest->toString() + "\n";
	}

private:

	argX0 *dest;
	list<pair<std::string, int>> *registers;

};


// (subq arg, arg) <-- instruction
class subqX0 : public instrX0 {

public:

	subqX0(argX0* src_, argX0* dest_) {
		this->src = src_;
		this->dest = dest_;
	}

	int eval(list<pair<std::string, int>> *regs) {
		this->registers = regs;
		for (std::list<pair<std::string, int>>::iterator it = this->registers->begin(); it != this->registers->end(); ++it) {
			if ((*it).first == this->dest->getName()) {
				((*it).second = this->src->eval(this->registers) - this->dest->eval(this->registers));
				return 0;
			}
		}
		return 3;
	}

	string toString() {
		return "\tsubq\t\t" + this->src->toString() + ",\t" + this->dest->toString() + "\n";
	}

private:

	argX0 *src, *dest;
	list<pair<std::string, int>> *registers;

};


// (addq arg, arg) <-- instruction
class addqX0 : public instrX0 {

public:

	addqX0(argX0* src_,argX0* dest_) {
		this->src = src_;
		this->dest = dest_;
	}

	int eval(list<pair<std::string, int>> *regs) {
		this->registers = regs;
		for (std::list<pair<std::string, int>>::iterator it = this->registers->begin(); it != this->registers->end(); ++it) {
			if ((*it).first == this->dest->getName()) {
				((*it).second = this->src->eval(this->registers) + this->dest->eval(this->registers));
				return 0;
			}
		}
		return 2;
	}

	string toString() {
		return "\taddq\t\t" + this->src->toString() + ",\t" + this->dest->toString() + "\n";
	}

private:

	argX0 *src, *dest;
	list<pair<std::string, int>> *registers;

};


// (movq arg arg) <-- instruction
class movqX0 : public instrX0 {

public:

	movqX0(argX0* src_, argX0* dest_) {
		this->src = src_;
		this->dest = dest_;
	}

	int eval(list<pair<std::string, int>> *regs) {
		this->registers = regs;
		for (std::list<pair<std::string, int>>::iterator it = this->registers->begin(); it != this->registers->end(); ++it) {
			if ((*it).first == this->dest->getName()) {
				((*it).second = this->src->eval(this->registers));
				return 0;
			}
		}
		return 1;
	}

	string toString() {
		return "\tmovq\t\t" + this->src->toString() + ",\t" + this->dest->toString() + "\n";
	}


private:

	list<pair<std::string, int>> *registers;
	argX0 *src;
	argX0 *dest;

};


//X860  ::= (instr+)
class programX0 {

public:

	programX0(list<std::unique_ptr<instrX0>> *instructions_) {
		pcnt = 0;
		this->instructions_set = instructions_;
	}

	void execute(list<pair<std::string, int>> *registers_) {
		this->registers = registers_;
		cout << "\nProgram:\n\n";
		for (std::list<std::unique_ptr<instrX0>>::iterator it = this->instructions_set->begin(); it != this->instructions_set->end(); ++it) {
			if ((*it)->eval(registers) == 0) {
				cout << pcnt << "\t" << (*it)->toString();
				pcnt += 8;
			}
			else {
				cout << "\n\tError executing program.\n\tCheck Address: " << pcnt << ".\n\n";
				cout << (*it)->toString();
			}
		}
		pcnt = 0;
		cout << "\n\tExecution is done.\n\n" << "\nMemory:\n\n" << "\tRegister\tValue\n";
		for (std::list<pair<std::string, int>>::iterator it = registers->begin(); it != registers->end(); ++it) {
			cout << "\t" << (*it).first << "\t\t" << (*it).second << "\n";
		}
		cout << "\n";
	}

private:

	list<pair<std::string, int>> *registers;
	list<std::unique_ptr<instrX0>> *instructions_set;

};


#endif X0LANGUAGE_H