# FSM

finite state machine library in C++.

# Notes

- The tests depend on gtest, so you need to install the package if you want to run the tests.
- The `utility` directory has general utility functions used by this project and others, so it is important.

# Documentation

## Exception Types

### `InvalidStateMachineArgumentsException`

- A wrapper over `std::invalid_argument` that is thrown from the constructor of a state machine in case there is at least one invalid argument given to a state machine.

### `UnrecognizedSimModeException`

- A wrapper over `std::runtime_error` that is thrown from any `simulate()` function in case the simulation mode is unrecognized.

## Utility Data Structures

file: FiniteStateMachine.h

### `FSMStateType`

- A typedef representing a single state of the fsm. It is an alias to `unsigned`.

### `FSMStateSetType`

- A data structure representing a set of `FSMStateType`. This is used primarily for `NFA` simulation.

### `FSMTableType`

- A data structure representing a 2D table of sets of states.

  | field | definition                           |
  | ----- | ------------------------------------ |
  | table | the actual object holding the table. |

  | method                                                 | definition                                                                                             |
  | ------------------------------------------------------ | ------------------------------------------------------------------------------------------------------ |
  | `FSMStateType&` operator()(state, c)                   | returns `table[state][c]`, and makes sure the vectors are not overflowed (resizing them if necessary). |
  | `const FSMStateType&` operator()(state, c) const       | returns `table[state][c]`.                                                                             |
  | `const vector<FSMStateType>&` operator[] (index) const | return `table.at(index).`                                                                              |
  | `const vector<FSMStateType>&` at(index) const          | return `table.at(index)`.                                                                              |

- `FSMStateSetType` is used as the value of each element of the table rather than `FSMStateType` in order for the table to be general, i.e., able to be used with either a `DFA` or an `NFA`.

### `TransitionFunction<TableT>`

- A data structure that represents a transition function. It is a function object.

  | field          | definition        |
  | -------------- | ----------------- |
  | `TableT` table | the actual table. |

  | method                                              | definition                                                                                             |
  | --------------------------------------------------- | ------------------------------------------------------------------------------------------------------ |
  | `FSMStateType&` operator()(state, c)                | returns `table[state][c]`, and makes sure the vectors are not overflowed (resizing them if necessary). |
  | `const FSMStateSetType&` operator()(state, c) const | returns `table[state][c]`.                                                                             |
  | `const FSMStateType&` operator()(stateSet, c) const | returns `table[state][c]` for every state within `stateSet`.                                           |

- The goal of this data structure is to treat a `TableT` table as a function, i.e., it is just a wrapper around a `TableT` object.
- `TransFn<TableT>` is an alias for this data structure.

### `FSMResult`

- A data structure that represents the result of a single simulation of a `DFA` or an `NFA`.

  | field                        | definition                                                                                                          |
  | ---------------------------- | ------------------------------------------------------------------------------------------------------------------- |
  | `bool` accepted              | whether (a possibly substring of, depending on flags given to the simulation function) the input has been accepted. |
  | `FSMStateSetType` finalState | the final states reached when (the substring of) the input was accepted.                                            |
  | `Indecies` indecies          | the indecies of the matched (substring from the) input.                                                             |
  | `std::string_view` input     | the input against which the simulation has been run.                                                                |

  | method                              | definition                                                                        |
  | ----------------------------------- | --------------------------------------------------------------------------------- |
  | `size_t` size() const               | the size of the matched substring. equivalent to `indecies.end - indecies.start`. |
  | `std::string_view` getMatch() const | returns the matched substring from input.                                         |
  | `Indecies` getIndecies() const      | returns `this->indecies`.                                                         |

  | function                     | definition                     |
  | ---------------------------- | ------------------------------ |
  | `std::ostream<<(Result res)` | print `res` to `std::ostream`. |

### `Indecies`

- A data structure that represents the indecies of a lexeme found during a single simulation of a `DFA` or an `NFA`.

  | field             | definition                                               |
  | ----------------- | -------------------------------------------------------- |
  | `IndexType` start | the start index of a found lexeme relative to the input. |
  | `IndexType` end   | the end index of a found lexeme relative to the input.   |

  | method                            | definition                                                                                  |
  | --------------------------------- | ------------------------------------------------------------------------------------------- |
  | `std::string` toString()          | method that converts the `Indecies` object to a string.                                     |
  | `operator std::string()`          | conversion operator that just calls `toString()`.                                           |
  | `Indecies` operator +(`size_t` x) | method that adds whatever `x` to both `start` and `end`, returning a new `Indecies` object. |

### `Substring<T>`

- Represents a single matched substring in a simulation that matches a longest substring (the data structure is mainly used internally).

  | field                          | definition                                                              |
  | ------------------------------ | ----------------------------------------------------------------------- |
  | `std::vector<T>` matchedStates | the set of states the machine has went through to match this substring. |
  | `Indecies` indecies            | the indecies of the matched substring within the entire string.         |

  | method          | definition                     |
  | --------------- | ------------------------------ |
  | begin() `const` | return `matchedStates.begin()` |
  | end() `const`   | return `matchedStates.end()`   |
  | size() `const`  | return `matchedStates.size()`  |

## Enums

file: FiniteStateMachine.h

```C++
  // the mode of simulation.
 enum class FSM_MODE {
  MM_WHOLE_STRING = 0,
  MM_LONGEST_PREFIX,
  MM_LONGEST_SUBSTRING,
  MM_NONE,
  MM_FSM_MODE_MAX,
 };

 // the type of the FSM.
 enum class FSMType {
  MT_EPSILON_NFA = 0,
  MT_NON_EPSILON_NFA,
  MT_DFA,
  MT_MACHINE_TYPE_MAX,
 };

 // flags to customize the behavior of the FSM.
 enum FSM_FLAG {
  FF_FLAG_NONE = 0b0000000,
  FF_FLAG_MAX
 };
```

## Main Data Structures

### `FSM`

file: FiniteStateMachine.h

```C++
template <typename TransFuncT, typename InputT = std::string_view>
 class FiniteStateMachine {

  public:
  FiniteStateMachine() = default;
  FiniteStateMachine(const FSMStateSetType& fStates, const TransFuncT& tranFn, FSMType machineType ,FSMFlag flags);

  FiniteStateMachine& operator=(const FiniteStateMachine& rhs);

  const FSMStateSetType& getFinalStates() const;
  FSMFlag getFlags() const;
  FSMType getMachineType() const;
}


```

| constructor parameter | definition                                  |
| --------------------- | ------------------------------------------- |
| fStates               | the set of final states for this FSM.       |
| tranFn                | the transfer function for this FSM.         |
| machineType           | the type of the machine.                    |
| flags                 | flag to modify the behavior of the machine. |

| method           | definition                                                                                                |
| ---------------- | --------------------------------------------------------------------------------------------------------- |
| getFinalStates() | returns the set of final states of this machine; i.e. returns the value of constructor parameter fStates. |
| getFlags()       | returns the flags of this machine.                                                                        |
| getMachineType() | returns the machine type of this machine.                                                                 |

### `NFA`

file: NFA.h

```C++
template <typename TransFuncT, typename InputT = std::string_view>
 class NonDeterFiniteAutomatan : FiniteStateMachine<TransFuncT, InputT> {

public:
  NonDeterFiniteAutomatan() = default;
  NonDeterFiniteAutomatan(const FSMStateSetType& fStates, const TransFuncT& tranFn, FSMType machineType = FSMType::MT_EPSILON_NFA, FSMFlag flags = FSM_FLAG::FF_FLAG_NONE);

  FSMResult simulate(const InputT& input, const FSM_MODE mode) const;
  }

```

| constructor parameter | definition                                                                                                                                                               |
| --------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| fStates               | the set of final states for this FSM.                                                                                                                                    |
| tranFn                | the transfer function for this FSM.                                                                                                                                      |
| machineType           | the type of the machine. this must be either `FSMType::MT_EPSILON_NFA` or `FSMType::MT_NON_EPSILON_NFA`. if not, an exception of type `std::invalid_argument` is thrown. |
| flags                 | flag to modify the behavior of the machine.                                                                                                                              |

| method     | definition                                                                                                           |
| ---------- | -------------------------------------------------------------------------------------------------------------------- |
| simulate() | simulates the string given to it as input, according to the simulation mode and returns the result as a `FSMResult`. |

- <b>Note</b>: types of simulation will be discussed at the end of this documentation.

### `DFA`

file: DFA.h

```C++
template <typename TransFuncT, typename InputT = std::string_view>
 class DeterFiniteAutomatan: public FiniteStateMachine<TransFuncT, InputT> {
public:
  DeterFiniteAutomatan() = default;
  DeterFiniteAutomatan(const FSMStateSetType& fStates, const TransFuncT& tranFn, FSMFlag flags = FSM_FLAG::FF_FLAG_NONE);

  DeterFiniteAutomatan& operator=(const DeterFiniteAutomatan& rhs);

  FSMResult simulate(const InputT& input, const FSM_MODE mode) const;
 };
```

| constructor parameter | definition                                  |
| --------------------- | ------------------------------------------- |
| fStates               | the set of final states for this FSM.       |
| tranFn                | the transfer function for this FSM.         |
| flags                 | flag to modify the behavior of the machine. |

| method     | definition                                                                                                           |
| ---------- | -------------------------------------------------------------------------------------------------------------------- |
| simulate() | simulates the string given to it as input, according to the simulation mode and returns the result as a `FSMResult`. |

## About Simulation Types

| simulation type   | definition                                                                                                                                |
| ----------------- | ----------------------------------------------------------------------------------------------------------------------------------------- |
| whole string      | checks whether the entire input string is accepted or not, and returns positive result iff it is accepted.                                |
| longest prefix    | checks for the longest accepted prefix of the input string, and returns positive resulf for it. otherwise, it returns negative result.    |
| longest substring | checks for the longest accepted substring of the input string, and returns positive resulf for it. otherwise, it returns negative result. |
