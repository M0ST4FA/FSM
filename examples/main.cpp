#include <iostream>
#include "fsm/FiniteStateMachine.h"
#include "fsm/DFA.h"

extern int example_dfa();
extern int example_nfa();

int main(int argc, char** argv) {

	example_dfa();
	example_nfa();

	m0st4fa::fsm::FSMTable table;
	/*table(1, 'a') = 2;
	table(1, 'b') = 3;
	table(2, 'a') = 2;
	table(2, 'b') = 3;
	table(3, 'b') = 4;
	table(4, 'b') = 4;*/
	table.set(1, (std::string)"ababbb");

	std::cout << (std::string)table; // MAKE SURE THIS WORKS

	return 0;
}