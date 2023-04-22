#include "../DFA.h"

extern int example_dfa() {

	/**
	* Let's say we want to make a DFA that matches the regular language of identifiers.
	* In this case, our regex will be: \w+(\w|\d|_)*
	* The first step is to build the state machine table (transition function).
	* The next step is to construct the DFA.
	* Now, we can run our DFA against any input string we would like to check whether it is an identifier, and also to get information about it (e.g. indecies).
	**/

	// for convenience (not recommended in real life)
	using namespace m0st4fa;

	// 1. building the transition function

	/**
	* Dead state is always 0 (fixed by the library).
	* Initial state will be 1 (chosen by me).
	* Final states: {2}
	**/

	FSMTable table{ };
	// set \w+
	for (char c = 'a'; c <= 'z'; c++)
		table(1, c) = 2;
	for (char c = 'A'; c <= 'Z'; c++)
		table(1, c) = 2;

	// set (\w|\d|_)* following \w+
	for (char c = 'a'; c <= 'z'; c++)
		table(2, c) = 2;
	for (char c = 'A'; c <= 'Z'; c++)
		table(2, c) = 2;

	for (char c = '0'; c <= '9'; c++)
		table(2, c) = 2;

	table(2, '_') = 2;


	TransFn<> transFunction{ table }; // remember: `TransFn<>` is just an abstraction, the actual table/function is `table`

	/** alternatively, you can set `transFunction` directly:
		TransFn<> transFunction{ };

		for (char c = 'a'; c <= 'z'; c++)
		transFunction(1, c) = 2;
	for (char c = 'A'; c <= 'Z'; c++)
		transFunction(1, c) = 2;

	// set (\w|\d|_)* following \w+
	for (char c = 'a'; c <= 'z'; c++)
		transFunction(2, c) = 2;
	for (char c = 'A'; c <= 'Z'; c++)
		transFunction(2, c) = 2;

	for (char c = '0'; c <= '9'; c++)
		transFunction(2, c) = 2;

	transFunction(2, '_') = 2;

	**/


	// 2. construct the DFA
	DeterFiniteAutomatan<TransFn<>> automaton{ {2}, transFunction };
	// here, the set of final states is {2} and `transFunction` is our transition function.

	// 3. now, we can simulate the automaton against any "potential identifier" we have and It will return us whether it is an "identifier" as well as other useful information.
	std::cout << automaton.simulate("x", FSM_MODE::MM_LONGEST_PREFIX) << "\n";
	std::cout << automaton.simulate("x_y_z", FSM_MODE::MM_LONGEST_PREFIX) << "\n";
	std::cout << automaton.simulate("x_2_3", FSM_MODE::MM_LONGEST_PREFIX) << "\n";

	// all of the above test must succeed and give us relevant info about the strings

	return 0;
}