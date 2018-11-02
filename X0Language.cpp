/*

	Ivan Blaskic [ @UML 4Jay's-CC-class ]
	X0Language.cpp

	Tester Code

	Questions for Jay: 
	- All in regards to how you said that I only just have to implement read function call
	1) Do I have to implement pushq and popq?
	2) Do I have to implement -->int (%reg)<--?
	3) Does my retq have to change the PC value?

*/

#include <stdlib.h>

#include "X0Language.hpp"

int main(void) {

	/*
	// instructions tester
	instrX0 *movq_tester1		= new movqX0	(new intX0(10),		new regX0("rax"));		// movq		int -> reg
	instrX0 *movq_tester2		= new movqX0	(new regX0("rax"),	new regX0("rbx"));		// movq		reg -> reg
	instrX0 *addq_tester1		= new addqX0	(new intX0(15),		new regX0("rax"));		// addq		int -> reg
	instrX0 *addq_tester2		= new addqX0	(new regX0("rbx"),	new regX0("rax"));		// addq		reg -> reg
	instrX0 *subq_tester1		= new subqX0	(new intX0(100),	new regX0("rax"));		// subq		int -> reg
	instrX0 *subq_tester2		= new subqX0	(new regX0("rax"),	new regX0("rbx"));		// subq		reg -> reg
	instrX0 *retq_tester		= new retqX0	();											// retq
	instrX0 *negq_tester		= new negqX0	(new regX0("rbx"));							// negq		reg
	instrX0 *callq_tester		= new callqX0	();											// callq	print_int -> calling function that prints %rdi
	//instrX0 *pushq_tester1	= new pushqX0	(new intX0(10));							// pushq	int
	//instrX0 *pushq_tester2	= new pushqX0	(new regX0("rax"));							// pushq	reg
	//instrX0 *popq_tester		= new popqX0	(new regX0("rcx"));							// popq		reg
	*/


	// program tester initialization
	list<std::unique_ptr<instrX0>> prog_tester;
	pcnt = 0;

	prog_tester.emplace_back(new movqX0(new intX0(10), new regX0("rax")));
	prog_tester.emplace_back(new movqX0(new regX0("rax"), new regX0("rbx")));
	prog_tester.emplace_back(new addqX0(new intX0(15), new regX0("rax")));
	prog_tester.emplace_back(new addqX0(new regX0("rbx"), new regX0("rax")));
	prog_tester.emplace_back(new subqX0(new intX0(100), new regX0("rax")));
	prog_tester.emplace_back(new subqX0(new regX0("rax"), new regX0("rbx")));
	//prog_tester.emplace_back(new retqX0());
	prog_tester.emplace_back(new negqX0(new regX0("rbx")));
	prog_tester.emplace_back(new callqX0());
	prog_tester.emplace_back(new pushqX0(new regX0("rax")));
	prog_tester.emplace_back(new popqX0(new regX0("rbx")));

	programX0 *x0_tester = new programX0(&prog_tester);

	// program tester execution
	x0_tester->execute(&regs);

	/*
	// instructions execution test
	movq_tester1->eval(&regs);
	movq_tester2->eval(&regs);
	addq_tester1->eval(&regs);
	addq_tester2->eval(&regs);
	subq_tester1->eval(&regs);
	subq_tester2->eval(&regs);
	negq_tester->eval(&regs);
	callq_tester->eval(&regs);
	retq_tester->eval(&regs);

	// instructions print
	cout << "\nProgram:\n";
	cout << movq_tester1->toString();
	cout << movq_tester2->toString();
	cout << addq_tester1->toString();
	cout << addq_tester2->toString();
	cout << subq_tester1->toString();
	cout << subq_tester2->toString();
	cout << negq_tester->toString();
	cout << callq_tester->toString();
	cout << retq_tester->toString();
	*/

	/*
	// registers print
	cout << "\nMemory:\n";
	cout << "\tRegister\tValue\n";
	for (std::list<pair<std::string,int>>::iterator it = ptr_register_list->begin(); it != ptr_register_list->end(); ++it) {
		cout << "\t" << (*it).first << "\t\t" << (*it).second << "\n";
	}
	cout << "\n";
	*/

	system("pause");

}