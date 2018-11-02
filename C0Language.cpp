/*

	Ivan Blaskic [ @UML 4Jay's-CC-class ]
	C0Language.cpp

	Tester Code

*/

#include "C0Language.hpp"

int main() {

	// assignment tester
	stmtC0 *assign_read2var = new assignC0 (new varC0("x"), new readC0());								// read2arg
	stmtC0 *assign_int2var = new assignC0 (new varC0("y"), new intC0(5));								// arg2arg
	stmtC0 *assign_var2var = new assignC0 (new varC0("z"), new varC0("x"));								// arg2arg
	stmtC0 *assign_negvar2var = new assignC0 (new varC0("a"), new negC0(new varC0("x")));				// neg(var)2arg
	stmtC0 *assign_negint2var = new assignC0 (new varC0("b"), new negC0(new intC0(5)));					// neg(int)2arg
	stmtC0 *assign_plus2var = new assignC0 (new varC0("c"), new addC0(new intC0(5), new varC0("x")));	// plus(int,var)2arg

	// return tester
	stmtC0 *ret_var = new retC0(new varC0("x"));
	stmtC0 *ret_int = new retC0(new intC0(5));

	// list of statements tester
	list<std::unique_ptr<stmtC0>> prog_tester;
	list<pair<std::string,int>> vars;

	prog_tester.emplace_back(assign_read2var);
	prog_tester.emplace_back(assign_int2var);
	prog_tester.emplace_back(assign_var2var);
	prog_tester.emplace_back(assign_negvar2var);
	prog_tester.emplace_back(assign_negint2var);
	prog_tester.emplace_back(assign_plus2var);
	prog_tester.emplace_back(ret_var);
	prog_tester.emplace_back(ret_int);

	vars.push_back(std::make_pair("a", 0));
	vars.push_back(std::make_pair("b", 0));
	vars.push_back(std::make_pair("c", 0));
	vars.push_back(std::make_pair("x", 0));
	vars.push_back(std::make_pair("y", 0));
	vars.push_back(std::make_pair("z", 0));

	progC0 *program = new progC0(&prog_tester);

	program->execute(&vars);

	system("pause");

}