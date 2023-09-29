
Getting Started
===============

Welcome! This tutorial highlights the core use cases of FSM library. For more information, you can take a look at the API documentation (you can start reading the API at the `FSM-Documentation`_).

.. role:: cpp(code)
  :language: cpp

Simulate a Finite State Machine
-------------------------------

To get started, you need to create an FSM object to represent your state machine. 

There are a few basic steps:

  1. Create a transition table mapping the characters of your alphabet to states of the machine. You do this using the struct :cpp:`FSMTable`.
  2. Create a transition function based on that table (this is optional, as the transition function is just an abstraction over the table). You do this using the template struct :cpp:`TransitionFunction<TableT>`
  3. Create the automatan using the template class :cpp:`DeterFiniteAutomatan<TransitionFunctionT, InputT>`
  4. Now you can simulate your state machine using the :cpp:`simulate` method called on the automatan object.

The following code example demonestrates these steps.

DFA Example
-----------

Let's say you want to make a DFA that matches the regular language of identifiers. In this case, the regular language of this machine will be the language specified by the regex :cpp:`\w+(\w|\d|_)*`.
First, build the state machine table (transition function). Then construct the DFA object.
Now, you can simulate the DFA using that DFA object.

.. note::

	Notice how everything in this library is within the namespace ``m0st4fa``. This is my special signature. It is a deviation from my name (which is Mostafa).

.. literalinclude:: examples/example_dfa.cpp
	:language: cpp
	:caption: Example of building a finite state machine object to run against the identifier grammar of C.

There are some assumption here. These can be summarized as:

	1. Dead state is always 0 (this is fixed by the library).
	2. You can choose the initial state. Here I chose it as 1.
	3. You can also choose the final states. However, this will have an effect on the transition table. I suggest you first create the transition table and then set the final states.

.. note::
	Alternatively, you can work with :cpp:`TranFn<>` directly.

	.. code-block:: cpp

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


Simulation Modes
----------------

When running the :cpp:`simulate` function, the second (optional) argument is the simulation mode. There are three simulation modes. All of them belong to the enum :cpp:`FSM_MODE`.

:cpp:`MM_WHOLE_STRING` : default
    In this mode, the simulation is run against the whole string. In other words, the entire string must be either accepted or rejected, no matter whether it contains a substring that may be rejected or accepted, respectively.

:cpp:`MM_LONGEST_PREFIX`
    In this mode, the simulation looks for the longest prefix (i.e. substring from the front of the string) that may accept.

:cpp:`MM_LONGEST_SUBSTRING`
    In this mode, the simulation looks for the longest substring that accepts, no matter whether it is a prefix, a suffix or the entire string. 

Logically, :cpp:`MM_LONGEST_SUBSTRING` has the poorest performance. What it literally does is that it keeps continuously searching for an accepting prefix, until either one is found or the the entire string is rejected. It is like running :cpp:`simulate` with :cpp:`MM_LONGEST_PREFIX` in a loop.

NFA Example
-----------

This is very similar to using a DFA except that you create an NFA object (instead of a DFA object).
The grammar of the NFA in this example is the C language identifier lexical grammar (the same used for the DFA of the previous example.).
Note, however, the difference that exists when mapping states using the transition table. In this case, you map a state to a **set of states**. With a DFA, you map a state to a **single state** (see the `DFA Example`_.). Everything else is almost exactly the same.

.. literalinclude:: examples/example_nfa.cpp
	:language: cpp
	:caption: Example of the same identifier grammar using an NFA instead of a DFA. 

Choosing the Type of NFA
------------------------

There are several types the state machine can take. All of these types are included in the enumerator :cpp:`FSM_TYPE`.

:cpp:`MT_EPSILON_NFA` : default
	This sets the NFA that the :cpp:`NFA` object simulates to an epsilon NFA.

:cpp:`MT_NON_EPSILON_NFA`
	This sets the NFA that the :cpp:`NFA` object simulates to a non-epsilon NFA.

:cpp:`MT_DFA`
	The type of a :cpp:`DFA`. This is set automatically for every :cpp:`DFA` object created.
