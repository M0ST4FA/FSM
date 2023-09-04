
Getting Started
===============

Welcome! This tutorial highlights the core use cases of FSM library. For more information, you can take a look at the API documentation (get to the homepage and find it).

Simulate a finite state machine
-------------------------------

This is perhaps the only use of this library. To get started, you need to create an FSM object to represent your state machine. 

.. role:: cpp(code)
  :language: cpp

There are a few basic steps:
  1. Create a transition table mapping the characters of your alphabet to states of the machine. You do this using the struct :cpp:`FSMTable`.
  2. Create a transition function based on that table (this is optional, as the transition function is just an abstraction over the table). You do this using the template struct :cpp:`TransitionFunction<TableT>`
  3. Create the automatan using the template class :cpp:`DeterFiniteAutomatan<TransitionFunctionT, InputT>`
  4. No you can simulate your state machine using the :cpp:`simulate` method called on the automatan object.

The following code  example demonstrates these steps.

DFA Example
-----------

Let's say you want to make a DFA that matches the regular language of identifiers. In this case, the regex of this machine will be :cpp:`\w+(\w|\d|_)*`

First, build the state machine table (transition function). Then construct the DFA object.
Now, you can simulate the DFA using the DFA object 

One last note: observe that everything in this library is within the namespace ``m0st4fa``. This is my special signature. It is a deviation from my name (which is Mostafa).

.. code-block:: c++
  :name: DFAExample
  :caption: An example showing how to setup a basic state machine.

  #include "DFA.h"

	// for convenience (not recommended in real life)
	using namespace m0st4fa;

	// 1. building the transition table

	/**
	* Dead state is always 0 (fixed by the library).
	* Initial state will be 1 (chosen by me).
	* Final states: {2} (you can make it whater you want).
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

  // 2. build the transition function
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

	// 3. construct the DFA
	DeterFiniteAutomatan<TransFn<>> automaton{ {2}, transFunction };
	// here, the set of final states is {2} and `transFunction` is our transition function.

	// 4. now, you can simulate the automaton against any "potential identifier" you have and It will return you whether it is an "identifier" as well as other useful information.
	std::cout << automaton.simulate("x", FSM_MODE::MM_LONGEST_PREFIX) << "\n";
	std::cout << automaton.simulate("x_y_z", FSM_MODE::MM_LONGEST_PREFIX) << "\n";
	std::cout << automaton.simulate("x_2_3", FSM_MODE::MM_LONGEST_PREFIX) << "\n";

	// all of the above test must succeed and give us relevant info about the strings

That's it!!!  