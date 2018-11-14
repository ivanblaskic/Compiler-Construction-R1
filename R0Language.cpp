/*

	Ivan Blaskic [ @UML 4Jay's-CC-class ]
	R0Language.cpp

	Tester Code

*/

#include "R0Language.hpp"

int main() {

	list<pair<std::string, int>> *code = new list<pair<std::string, int>>();
	//list<pair<std::string, int>> *code1 = new list<pair<std::string, int>>();



	/*
	expR0 *program = new addR0(new negR0(new intR0(5)), 
							   new addR0(new readR0(), 
										 new intR0(10)));
	cout << program->toString() << " = " << program->eval(code) << "\n\n";
	*/

	expR0 *program1 = new addR0(new negR0(new intR0(5)),
								new addR0(new readR0(),
										  new intR0(10)));

	// + (- (5)) (+ (read) (10)) 
	//			--> vars: tmp.1 tmp.2 tmp.3 
	//			--> [assign tmp.1 (-5)] [assign tmp.2 (read)] [assign tmp.3 (+ tmp.1 tmp.2)] [return tmp.3] 

	static list<pair<unique_ptr<varR0>, unique_ptr<varR0>>> *map_vars = new list<pair<unique_ptr<varR0>, unique_ptr<varR0>>>();
	
	/*
	expR0 *program3 = program1->uniquify(map_vars);
	cout << program3->toString() << " = " << program3->eval(code) << "\n\n";
	*/

	/*
	expR0 *program2 = new letR0(new varR0("x"), 
								new addR0(new intR0(5), 
										  new intR0(7)), 
								new addR0(new varR0("x"), 
										  new intR0(5)));
	cout << program2->toString() << " = " <<  program2->eval(code) << "\n\n";
	*/
	
	expR0* program4 = new letR0(new varR0("x"), 
								new intR0(5),
								new addR0(new letR0(new varR0("x"), 
													new intR0(6), 
													new varR0("x")), 
										  new varR0("x")));
	
	cout << "\n\t" << program4->toString() << " = " << program4->eval(code) << "\n\n";
	
	system("pause");

	expR0 *program3 = program4->uniquify(map_vars);
	cout << "\n\t" << program3->toString() << " = " << program3->eval(code) << "\n\n";

	//stmtC0 *assign_int2var = new assignC0(new varC0("y"), new intC0(5)); --> want this as an result

	static list<std::unique_ptr<stmtC0>> program_tester;
	static list<pair<std::string, int>> variables;

	progC0 *program5 = program1->flatten();
	program5->execute(&variables);

	// add execution of the code in C0 and R0

	system("pause");

	if (program3->eval(code) == program4->eval(code))
		cout << "\n\tCompiler Uniquifying Variable Names is a Beast Bro.\n\n";

	system("pause");

} 