#include "R0Language.hpp"

int main() {

	expR0 *program = new addR0(new negR0(new intR0(5)), new addR0(new readR0(new varR0("x")), new intR0(10)));
	cout << program->eval() << "\n";
	cout << program->toString() << "\n";
	system("pause");

}