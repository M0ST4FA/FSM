#pragma once

#include <stack>
#include <ranges>
#include <functional>
#include <assert.h>

#include "FiniteStateMachine.h"

// DECLARATIONS
namespace m0st4fa::fsm {

	/**
	* @brief An NFA that can be used to match strings.
	* @noop The transition function must map states and input to sets of states.
	*/
	template <typename TransFuncT, typename InputT = std::string_view>
	class NonDeterFiniteAutomaton : public FiniteStateMachine<TransFuncT, InputT> {
		using Base = FiniteStateMachine<TransFuncT, InputT>;
		using SubstringType = Substring<FSMStateSetType>;

		// static variables
		/**
		 * @brief The dead state used by the NFA simulation methods.
		 */
		constexpr static FSMStateType DEAD_STATE = 0;

		// PRIVATE METHODS

		// MAIN
		FSMResult _simulate_whole_string(const InputT&) const;
		FSMResult _simulate_longest_prefix(const InputT&) const;
		FSMResult _simulate_longest_substring(const InputT&) const;

		// HELPERS
		bool _check_accepted_longest_prefix(const std::vector<FSMStateSetType>&, size_t&) const;

		bool _check_accepted_substring(const InputT&, std::vector<FSMStateSetType>&, const size_t, size_t&) const;
		inline std::vector<SubstringType> _extract_matching_substrings(const InputT) const;
		FSMResult _get_longest_substring_from_matched_sets(const InputT, const std::vector<SubstringType>&) const;

		FSMStateSetType _epsilon_closure(const FSMStateSetType&) const;

	public:
		/**
		 * @brief Default constructor.
		 */
		NonDeterFiniteAutomaton() = default;
		/**
		 * @brief Initialize a new NFA object.
		 * @param[in] fStates The set of final states of the new NFA object.
		 * @param[in] tranFn The transition function of the new NFA object.
		 * @param[in] machineType The type of the NFA.
		 * @param[in] flags The flags given to the new NFA object.
		 * @see See m0st4fa::fsm::FSM_TYPE for setting the type of the machine and m0st4fa::fsm::FlagsType for determining flags.
		 */
		NonDeterFiniteAutomaton(const FSMStateSetType& fStates, const TransFuncT& tranFn, FSM_TYPE machineType = FSM_TYPE::MT_EPSILON_NFA, FlagsType flags = FSM_FLAG::FF_FLAG_NONE) :
			FiniteStateMachine<TransFuncT, InputT>{ fStates, tranFn, machineType, flags }
		{


			// if the correct machine type is not passed
			if (!(machineType == FSM_TYPE::MT_EPSILON_NFA || machineType == FSM_TYPE::MT_NON_EPSILON_NFA)) {
				const std::string message = R"(NonDeterFiniteAutomaton: machineType must be either "MT_EPSILON_NFA" or "MT_NON_EPSILON_NFA")";
				this->m_Logger.log(LoggerInfo::FATAL_ERROR, message);
				throw InvalidStateMachineArgumentsException(message);
			};

		};


		FSMResult simulate(const InputT&, const FSM_MODE) const;

	};

	/**
	 * @brief An alias type for NonDeterFiniteAutomaton.
	 */
	template <typename TransFuncT, typename InputT = std::string>
	using NFA = NonDeterFiniteAutomaton<TransFuncT, InputT>;
}

// IMPLEMENTATIONS
namespace m0st4fa::fsm {

	/**
	 * @brief Simulate against whole string. The simulation returns true if and only if the whole string accepts.
	 * @param[in] input The input string against which the simulation will run.
	 * @return FSMResult object that indicates the result of the simulation.
	 */
	template<typename TransFuncT, typename InputT>
	FSMResult NonDeterFiniteAutomaton<TransFuncT, InputT>::_simulate_whole_string(const InputT& input) const
	{
		constexpr FSMStateType startState = FiniteStateMachine<TransFuncT, InputT>::START_STATE;
		FSMStateSetType currState = { startState };

		/**
		 * Follow a path through the machine using the characters of the string.
		 * Break if you hit a dead state since it is dead.
		*/
		if (this->getMachineType() == FSM_TYPE::MT_NON_EPSILON_NFA) // If the machine is a non-epsilon NFA
			for (auto c : input)
				currState = this->m_TransitionFunc(currState, c);
		else // If the machine is an epsilon NFA
			for (auto c : input)
				currState = _epsilon_closure(this->m_TransitionFunc(currState, c));
		
		// assert whether we've reached a final state
		FSMStateSetType finalStates = this->_get_final_states_from_state_set(currState);
		bool accepted = !finalStates.empty();
				

		return FSMResult(accepted, finalStates, { 0, accepted ? input.size() : 0 }, input);
	}

	/**
	 * @brief Simulates the NFA against `input` looking for the longest prefix only.
	 * @param[in] input The input string against which the simulation will run.
	 * @return FSMResult object that indicates the result of the simulation.
	 */
	template<typename TransFuncT, typename InputT>
	FSMResult NonDeterFiniteAutomaton<TransFuncT, InputT>::_simulate_longest_prefix(const InputT& input) const
	{
		constexpr FSMStateType startState = FiniteStateMachine<TransFuncT, InputT>::START_STATE;
		std::vector<FSMStateSetType> matchedStates = { {startState} };
		size_t charIndex = 0;

		/**
		* keeps track of the path taken through the machine.
		* Will be used to figure out the longest matched prefix, if any.
		*/

		/**
		 * Follow a path through the machine using the characters of the string.
		 * Keep track of that path in order to be able to find the longest prefix if the whole string is not accepted.
		*/
		if (this->getMachineType() == FSM_TYPE::MT_NON_EPSILON_NFA)
			for (; charIndex < input.size(); charIndex++) {
				const auto c = input.at(charIndex);
				// get next set of states and update our path through the machine
				matchedStates.push_back(this->m_TransitionFunc(matchedStates.back(), c));
			}
		else 
			for (; charIndex < input.size(); charIndex++) {
				const auto c = input.at(charIndex);
				// get next set of states and update our path through the machine
				matchedStates.push_back(this->_epsilon_closure(this->m_TransitionFunc(matchedStates.back(), c)));
			}

		// make sure `charIndex` < input.size()
		if (charIndex == input.size())
			charIndex = input.size() - 1;

		// set the index of the current character; figure out whether there is an accepted longest prefix; set the end index
		bool accepted = _check_accepted_longest_prefix(matchedStates, charIndex);
		size_t end = accepted ? charIndex + 1 : 0;
		
		// get the final states we've reached
		const FSMStateSetType& finalStates = this->_get_final_states_from_state_set(matchedStates.back());

		return FSMResult(accepted, finalStates, { 0, end }, input);
	}

	/**
	 * @brief Simulates the NFA against `input` looking for the longest substring, which might be the entire string.
	 * @param[in] input The input string against which the simulation will run.
	 * @return FSMResult object that indicates the result of the simulation.
	 */
	template<typename TransFuncT, typename InputT>
	FSMResult NonDeterFiniteAutomaton<TransFuncT, InputT>::_simulate_longest_substring(const InputT& input) const
	{

		// The substrings that are matching within `input`.
		std::vector<SubstringType> substrings = _extract_matching_substrings(input);

		// If there's at least one accepted substring
		if (substrings.size())
			return _get_longest_substring_from_matched_sets(input, substrings);

		// If there was no accepted substring.
		return FSMResult(false, {}, {0, 0}, input);
	}

	/**
	* @brief Simulates the DFA against `input` looking for the longest prefix. It also updates `charIndex` to the index of the last character of that prefix.
	* @param[in] matchedStates A set of states that constitute a path through the NFA, following `input` character by character starting from the start state of the NFA.
	* @param[out] charIndex The index of the last character of the matched prefix, if found; otherwise, 0.
	* @return `true` if a prefix matches, `false` otherwise.
	**/
	template<typename TransFuncT, typename InputT>
	bool NonDeterFiniteAutomaton<TransFuncT, InputT>::_check_accepted_longest_prefix(const std::vector<FSMStateSetType>& matchedStates, size_t& charIndex) const
	{
		constexpr FSMStateType startState = FiniteStateMachine<TransFuncT, InputT>::START_STATE;

		/**
		* Loop through the path from the end seeking the closes final state.
		* Update the character index as you do so.
		*/
		std::ranges::reverse_view rv{matchedStates};
		for(const FSMStateSetType& state : rv)
		{
			if (this->_is_state_final(state))
				return true;
			
			charIndex--;
		}
		
		// make sure `charIndex` does not get below 0
		if (charIndex < 0)
			charIndex = 0;

		return false;
	}

	/**
	* @brief Checks whether the substring starting from `startIndex` accepts. It also updates `charIndex` to the index of the last character of that substring.
	* @param[in] matchedStates The set of states that form the path taken through the machine after simulating the machine against `input`.
	* @param[in] startIndex The index, within `input`, at which the substring starts.
	* @param[out] charIndex The index of the last checked character (the last that didn't result in a dead state).
	* @return `true` if a substring starting from startIndex has accepted; `false` otherwise.
	**/
	template<typename TransFuncT, typename InputT>
	bool NonDeterFiniteAutomaton<TransFuncT, InputT>::_check_accepted_substring(const InputT& input, std::vector<FSMStateSetType>& matchedStates, const size_t startIndex, size_t& charIndex) const	
	{

		assert(charIndex == startIndex);

		/**
		* Follow a path through the machine using the characters of the string.
		* Keep track of that path in order to be able to find the longest prefix if the whole string is not accepted.
		*/
		auto matchInput = [input, &charIndex, &matchedStates, this](bool calcClosure = false) {
			for (; charIndex < input.size(); charIndex++) {

				auto c = input[charIndex];

				FSMStateSetType currStateSet{};

				if (calcClosure) {
					currStateSet = _epsilon_closure(
						this->m_TransitionFunc(matchedStates.back(), c)
					);
				}
				else {
					currStateSet = this->m_TransitionFunc(matchedStates.back(), c);
				}

				// if the current state is empty
				if (!currStateSet.size()) {
					charIndex--;
					return; // break out of the loop and the function
				}

				// get next set of states
				// update our path through the machine
				matchedStates.push_back(currStateSet);
			}

			// make sure charIndex is less that input.size() even if the entire string accepts
			if (charIndex == input.size())
				charIndex--;
		};
		
		if (this->getMachineType() == FSM_TYPE::MT_NON_EPSILON_NFA)
			matchInput();
		else
			matchInput(true);

		// figure out whether there is an accepted longest prefix
		return _check_accepted_longest_prefix(matchedStates, charIndex);
	}

	/**
	* @brief Extracts all matched substrings in `input`, if any, and returns them.
	* @param[in] input The input out of which the substrings will be extracted.
	* @return The set of substrings extracted out of `input`.
	**/
	template<typename TransFuncT, typename InputT>
	inline std::vector<Substring<FSMStateSetType>> NonDeterFiniteAutomaton<TransFuncT, InputT>::_extract_matching_substrings(const InputT input) const
	{
		size_t start = 0;
		size_t charIndex = 0;

		constexpr FSMStateType startState = FiniteStateMachine<TransFuncT, InputT>::START_STATE;
		/*
		* @brief Keeps track of the path taken through the machine.
		* Will be used to figure out the longest matched prefix, if any.
		*/
		std::vector<FSMStateSetType> matchedStates = { {startState} };

		// @note Note that it is necessary to find all substrings first and then find which one is the longest. You cannot simply find the longest substring before finding all substrings first.

		// The substrings that match within `input`.
		std::vector<SubstringType> substrings{};

		// Get all the substrings that match within `input` and store them in `substrings`.
		for (; charIndex < input.size(); charIndex = ++start) {

			// check whether the substring is accepted (and populate everything given by reference)
			bool accepted = _check_accepted_substring(input, matchedStates, start, charIndex);

			// if the substring was accepted
			if (accepted) {
				substrings.push_back({ matchedStates, start, charIndex + 1 });

				// if the rest of the input is less than the current substring in length, leave
				if ((input.size() - start) < matchedStates.size())
					break;
			}

			matchedStates.resize(1);
		}

		return substrings;
	}

	/**
	* @brief Gets the longest substring from a set of matching substrings.
	* @param[in] input The input string to the simulation method (the matching substrings are substrings of this input string).
	* @param[in] substrings The set of matching substrings.
	* @return FSMResult object representing the longest matching substring from the set of matching substrings given to it.
	**/
	template<typename TransFuncT, typename InputT>
	inline FSMResult NonDeterFiniteAutomaton<TransFuncT, InputT>::_get_longest_substring_from_matched_sets(const InputT input, const std::vector<SubstringType>& substrings) const {

		// longest-matching substring
		const SubstringType* longest = nullptr;
		// size of longest-matching substring.
		size_t size = 0;

		// choose the longest substring or the first of many having the same length
		for (const auto& substring : substrings) {
			// if the length of this substring is longer than that of the previously caught
			if (substring.size() > size) {
				longest = &substring;
				size = longest->size();
			}
		}

		size_t start = 0;
		size_t end = 0;

		if (longest) {
			start = longest->indicies.start;
			end = longest->indicies.end;
		}

		// the final states we've reached
		const FSMStateSetType currState = longest->matchedStates.back();
		const FSMStateSetType finalStateSet = this->_get_final_states_from_state_set(currState);
		//assert("This set must contain at least a single final state" && finalStateSet.size());

		return FSMResult(true, finalStateSet, { start, end }, input);
	};

	/**
	 * @brief Calculate the epsilon closure of a set of states.
	 * @param[in] set The set for which epsilon closure will be calculated.
	 * @return The set that represents the epsilon closure of `set`.
	 */
	template<typename TransFuncT, typename InputT>
	FSMStateSetType NonDeterFiniteAutomaton<TransFuncT, InputT>::_epsilon_closure(const FSMStateSetType& set) const
	{
		FSMStateSetType res{set};

		std::stack<FSMStateType> stack{};
		// initialize the stack
		for (auto s : set)
			stack.push(s);
		
		while (stack.size()) {
			// get the last state and pop it; we will get its closure now, so we will not need it in the future
			FSMStateType s = stack.top();
			stack.pop();

			// check if the state has at least one transition on epsilon
			FSMStateSetType epsilonTransitions = this->m_TransitionFunc(s, '\0');

			// if the set has at least a single epsilon-transition
			if (!epsilonTransitions.empty()) {
				/*
				* Push all the states in the epsilon transitions onto the stack.
				* We do this to consider whether the state itself has any epsilon transitions.
				* This applies the recursiveness of the algorithm.
				* Before we push a state, we check to see if it is already in the set so that we don't consider the state again.
				* If we don't do that, we might end up with an infinite loop.
				*/ 
				for (const FSMStateType state : epsilonTransitions)
					if (!res.contains(state))
						stack.push(state);

				res.insert(epsilonTransitions.begin(), epsilonTransitions.end());
			};
			
		};
		
		return res;
	}
	
	/**
	* @brief Simulate the given input string using the given simulation method.
	* @param[in] input The input string to be simulated.
	* @param[in] mode The simulation mode.
	* @throw UnrecognizedSimModeException Thrown in case an incorrect simulation mode is entered, which is in fact unreachable. Thus, this exception is almost impossible to throw under normal conditions.
	* @return FSMResult object indicating the result of the simulation.
	*/
	template<typename TransFuncT, typename InputT>
	inline FSMResult NonDeterFiniteAutomaton<TransFuncT, InputT>::simulate(const InputT& input, FSM_MODE mode) const
	{
		switch (mode) {
		case FSM_MODE::MM_WHOLE_STRING:
			return this->_simulate_whole_string(input);
		case FSM_MODE::MM_LONGEST_PREFIX:
			return this->_simulate_longest_prefix(input);
		case FSM_MODE::MM_LONGEST_SUBSTRING:
			return this->_simulate_longest_substring(input);
		default:
			this->m_Logger.log(LoggerInfo::ERROR, "Unreachable: simulate() cannot reach this point. The provided mode is probably erroneous.");
			throw UnrecognizedSimModeException();
		}

	}
	
}
