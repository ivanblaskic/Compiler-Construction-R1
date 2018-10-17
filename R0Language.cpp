#include "R0Language.hpp"

int main() {

	expR0 *program = new addR0(new negR0(new intR0(5)), new addR0(new readR0(), new intR0(10)));
	cout << program->toString() << " = " << program->eval() << "\n\n";

	expR0 *program2 = new letR0(new varR0("x"), new addR0(new intR0(5), new intR0(7)), new addR0(new varR0("x"), new intR0(5)));
	cout << program2->toString() << " = " <<  program2->eval() << "\n";

	system("pause");

}

