#include "NFA.h"

extern int example_nfa() {

	/**
	* Let's say we want to make an NFA that matches the regular language of identifiers.
	* In this case, our regex will be: \w+(\w|\d|_)*
	* The first step is to build the state machine table (transition function).
	* The next step is to construct the NFA.
	* Now, we can run our NFA against any input string we would like to check whether it is an identifier, and also to get information about it (e.g. indecies).
	**/

	// for convenience (not recommended in real life)
	using namespace m0st4fa;

	// 1. building the transition function

	/**
	* Initial state will be 1 (can be anything, but 1 is convenient).
	* Final states: {2}
	**/

	TransFn<> transFunction{};
	// set \w+
	for (char c = 'a'; c <= 'z'; c++)
		transFunction(1, c) = { 2 };
	for (char c = 'A'; c <= 'Z'; c++)
		transFunction(1, c) = { 2 };

	// set (\w|\d|_)* following \w+
	for (char c = 'a'; c <= 'z'; c++)
		transFunction(2, c) = { 2 };
	for (char c = 'A'; c <= 'Z'; c++)
		transFunction(2, c) = { 2 };

	for (char c = '0'; c <= '9'; c++)
		transFunction(2, c) = { 2 };

	transFunction(2, '_') = { 2 };

	// 2. construct the NFA
	NonDeterFiniteAutomatan<TransFn<>> automaton{ {2}, transFunction };
	// here, the set of final states is {2} and `transFunction` is our transition function.

	// 3. now, we can simulate the automaton against any "potential identifier" we have and It will return us whether it is an "identifier" as well as other useful information.
	std::cout << automaton.simulate("x", FSM_MODE::MM_LONGEST_PREFIX) << "\n";
	std::cout << automaton.simulate("x_y_z", FSM_MODE::MM_LONGEST_PREFIX) << "\n";
	std::cout << automaton.simulate("x_2_3", FSM_MODE::MM_LONGEST_PREFIX) << "\n";

	// all of the above test must succeed and give us relevant info about the strings

	return 0;
}