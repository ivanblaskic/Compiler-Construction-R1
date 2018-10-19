/*

	Ivan Blaskic [ @UML 4Jay's-CC-class ]
	R0Language.cpp

	Tester Code

*/

#include "R0Language.hpp"

int main() {

	list<pair<std::string, int>> *code = new list<pair<std::string, int>>();

	expR0 *program = new addR0(new negR0(new intR0(5)), 
							   new addR0(new readR0(), 
										 new intR0(10)));
	cout << program->toString() << " = " << program->eval(code) << "\n\n";

	expR0 *program2 = new letR0(new varR0("y"), 
								new addR0(new intR0(5), 
										  new intR0(7)), 
								new addR0(new varR0("y"), 
										  new intR0(5)));
	cout << program2->toString() << " = " <<  program2->eval(code) << "\n\n";

	expR0* program3 = new letR0(new varR0("x"), 
								new intR0(5),
								new addR0(new letR0(new varR0("x"), 
													new intR0(6), 
													new varR0("x")), 
										  new varR0("x")));
	cout << program3->toString() << " = " << program3->eval(code) << "\n\n";

	system("pause");

} 