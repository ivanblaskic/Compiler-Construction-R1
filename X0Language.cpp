/*

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

#include "X0Language.hpp"

#include <stdlib.h>

int main(void) {

	//test function 

	system("pause");

}

/*

programX0 = list <instrX0>;

programX0 *main = new programX0(	new movqX0(	new intX0(10),	RAX ),
									new addqX0(	new intX0(32),	RAX ), 
									new movqX0( RAX,			RDI ),
									new callX0(	new lblX0( "print_int" ) ),	--> lista <lbl, adr>	
									new movqX0( new intX0(0),	RAX ),
									new retqX0( )						
								);

*/