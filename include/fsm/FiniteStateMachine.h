#pragma once

#include <set>
#include <unordered_set>
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <source_location>
#include <functional>

#include "Logger.h"
#include <concepts>

// EXCEPTION TYPES
namespace m0st4fa::fsm {
	
	//! @brief The exception thrown when there is an invalid state machine argument.
	struct InvalidStateMachineArgumentsException : public std::invalid_argument {

		InvalidStateMachineArgumentsException(const std::string& message) : std::invalid_argument{ message} {};

	};

	/**
	 * @brief The exception thrown when there is an unrecognized simulation mode.
	 * @see NonDeterFiniteAutomaton<TransFuncT, InputT>::simulate(const InputT& input, FSM_MODE mode) const;
	 * @see DeterFiniteAutomaton<TransFuncT, InputT>::simulate(const InputT& input, FSM_MODE mode) const;
	 */
	struct UnrecognizedSimModeException : public std::runtime_error {

		UnrecognizedSimModeException() : std::runtime_error{ "Unrecognized mode give to `simulate()` function."} {};

	};

}

// TYPE ALIASES
namespace m0st4fa::fsm {

	//! @brief The type of a state used by `FSM` objects and their descendants.
	using FSMStateType = unsigned;

	//! @brief The type of a set of FSMStateType objects used by FiniteStateMachine objects and their descendants.
	struct FSMStateSetType {
		using SetType = std::set<FSMStateType>;

		SetType m_StateSet;

	public:

		FSMStateSetType(const std::initializer_list<FSMStateType>& i) : m_StateSet{ i } {};
		FSMStateSetType(const SetType& set) : m_StateSet{ set } {};
		FSMStateSetType(const FSMStateType state) : m_StateSet{ state } {};
		FSMStateSetType() = default;

		// CONVERSION
		explicit operator SetType() const {
			return m_StateSet;
		}
		explicit operator FSMStateType() const {

			if (this->empty())
				return FSMStateType{};

			return *this->begin();
		}

		// MODIFIERS
		template<typename InputIt>
		void insert(const InputIt begin, const InputIt end) {
			m_StateSet.insert(begin, end);
		}
		void insert(std::initializer_list<FSMStateType> ilist) {
			m_StateSet.insert(ilist);
		}
		void insert(const FSMStateType state) {
			m_StateSet.insert(state);
		}
		FSMStateSetType& operator=(const FSMStateSetType& rhs) {
			this->m_StateSet = rhs.m_StateSet;
			return *this;
		}

		// ITERATORS
		SetType::iterator begin() {
			return m_StateSet.begin();
		}
		SetType::iterator end() {
			return m_StateSet.end();
		}
		SetType::iterator begin() const {
			return m_StateSet.begin();
		}
		SetType::iterator end() const {
			return m_StateSet.end();
		}

		// LOOKUP AND CAPACITY
		bool contains(const FSMStateType val) const {
			return m_StateSet.contains(val);
		}
		size_t size() const {
			return m_StateSet.size();
		}
		bool empty() const {
			return m_StateSet.empty();
		}

	};

	template <typename T>
	concept StateSetConcept = std::is_same_v<T, FSMStateSetType> || std::is_same_v<T, std::vector<FSMStateType>>;

	template <StateSetConcept T>
	std::ostream& operator<<(std::ostream& os, const T& set)
	{

		// if the set is empty
		if (set.empty()) {
			std::cout << "{ }";

			return os;
		}


		// if it is not empty
		std::string temp = "{ ";
		temp += std::to_string(*set.begin());
		for (auto s : set) {
			if (s == *set.begin())
				continue;

			temp += (", " + std::to_string(s));
		}
		temp += " }\n";

		std::cout << temp;

		return os;
	}

	typedef unsigned FlagsType;
	typedef unsigned long long IndexType;

}

// ENUMERATIONS
namespace m0st4fa::fsm {
	/**
	 * @brief The simulation mode that simulation functions use.
	 * @see NonDeterFiniteAutomaton<TransFuncT, InputT>::simulate(const InputT& input, FSM_MODE mode) const;
	 * @see DeterFiniteAutomaton<TransFuncT, InputT>::simulate(const InputT& input, FSM_MODE mode) const;
	 */
	enum class FSM_MODE {

		//! @brief The simulation returns true if and only if the whole string accepts.
		MM_WHOLE_STRING = 0,

		//! @brief Look for the longest prefix only. Once found, the string accepts.
		MM_LONGEST_PREFIX,

		//! @brief Look for the longest substring, which might be the entire string.
		MM_LONGEST_SUBSTRING,

		//! @brief The default value.
		MM_NONE,

		//! @brief The number of enumerators that this enumeration has.
		MM_FSM_MODE_COUNT,
	};

	/**
	 * @brief The type of the FiniteStateMachine.
	 * @see NonDeterFiniteAutomaton
	 * @see DeterFiniteAutomaton
	 */
	enum class FSM_TYPE {
		//! @brief This sets the NFA that the NonDeterFiniteAutomaton object simulates to an epsilon NFA.
		MT_EPSILON_NFA = 0,

		//! @brief This sets the NFA that the NonDeterFiniteAutomaton object simulates to a non-epsilon NFA.
		MT_NON_EPSILON_NFA,

		//! @brief The type of a DeterFiniteAutomaton. This is set automatically for every DeterFiniteAutomaton object created.
		MT_DFA,

		//! @brief The number of enumerators that this enumeration has.
		MT_MACHINE_TYPE_COUNT,
	};

	/**
	 * @brief Flags to customize the behavior of the FSM.
	 * @note Right now, they are not implemented.
	 * @todo Implement the commented out flags (in the source code) in the state machines.
	 * @see NonDeterFiniteAutomaton
	 * @see DeterFiniteAutomaton
	 */
	enum FSM_FLAG {
		//FF_CASE_INSENSITIVE,
		//FF_CASE_SENSITIVE,

		//! @brief The default value.
		FF_FLAG_NONE = 0,

		//! @brief The number of enumerators that this enumeration has.
		FF_FLAG_COUNT
	};

}

// CLASSES & STRUCTURES
namespace m0st4fa::fsm {

	struct FSMTable {
		using StateSetVecType = std::vector<FSMStateSetType>;
		using VecType = std::vector<StateSetVecType>;
		using ItType = VecType::iterator;
		using ConstItType = VecType::const_iterator;

		mutable VecType table;
		
		template<typename InputT = char>
		FSMStateSetType& operator()(const FSMStateType& state, const InputT c) noexcept(true) {
			
			if (table.size() <= state)
				table.resize(state + 1);
			
			std::vector<FSMStateSetType>& stateMap = table.at(state);

			if (stateMap.size() <= c)
				stateMap.resize(c + 1);

			return stateMap.at(c);
		}

		template<typename InputT = char>
		const FSMStateSetType& operator()(const FSMStateType& state, const InputT c) const {
			if (table.size() <= state)
				table.resize(state + 1);

			std::vector<FSMStateSetType>& stateMap = table.at(state);

			if (stateMap.size() <= c)
				stateMap.resize(c + 1);

			return stateMap.at(c);
		}

		const StateSetVecType& operator[](const FSMStateType& state) const {
			if (table.size() <= state)
				table.resize(state + 1);

			return table.at(state);
		}

		const StateSetVecType& at(const FSMStateType& state) const {
			if (table.size() <= state)
				table.resize(state + 1);

			return table.at(state);
		}

		ItType begin() {
			return table.begin();
		}
		ConstItType begin() const {
			return table.begin();
		}

		ItType end() {
			return table.end();
		}
		ConstItType end() const {
			return table.end();
		}

	};

	template <typename TableT = FSMTable>
	struct TransitionFunction {
		TableT m_Table;
		
		TransitionFunction() = default;
		TransitionFunction(const TableT& table) : m_Table(table) {}
		
		template<typename InputT>
		FSMStateSetType& operator()(const FSMStateType& state, const InputT c) noexcept(true) {
			return this->m_Table(state, c);
		}

		template <typename InputT>
		FSMStateSetType operator()(const FSMStateType state, const InputT input) const {

			return m_Table(state, input);
		}

		template <typename InputT>
		FSMStateSetType operator()(const FSMStateSetType& stateSet, const InputT input) const {
			FSMStateSetType res;
			
			for (FSMStateType state : stateSet) {
				FSMStateSetType tmp = m_Table(state, input);
				res.insert(tmp.begin(), tmp.end());
			}
				
			return res;
		}

	};

	template<typename TableT = FSMTable>
	using TransFn = TransitionFunction<TableT>;

	struct FSMResult;
	std::ostream& operator<<(const std::ostream&, const FSMResult&);

	/**
	 * @brief A finite state machine that can check whether a string accepts according to some regular grammar.
	 * @details This class is designed so as to contain common constructs that a typical state machine, no matter its type, will have; thus, it is mainly designed to be inherited from rather than used directly.
	 */
	template <typename TransFuncT, typename InputT = std::string_view>
	class FiniteStateMachine {

		// friends
		friend struct FSMResult;
		
		// private instance data members
		FSMStateSetType m_FinalStates{};
		FSM_TYPE m_MachineType;
		FlagsType m_Flags;

	protected:

		//! @brief Used to log to the console in a standard way, implementing the correct colors.
		Logger m_Logger;
		//! @brief The function that will be called on each transition.
		TransFuncT m_TransitionFunc;

		/**
		* @brief Checks whether `state` is final (contains at least one final state) and returns a boolean indicating it.
		* @details This is often used to check whether a string is accepted or not (whether we've reached an accepting state).
		* @param[in] state The state set to check for whether it is final or not.
		* @return `True` if `state` is a final state set; `False` otherwise.
		**/
		inline bool _is_state_final(const FSMStateSetType& state) const
		{

			for (auto s : state)
				if (this->getFinalStates().contains(s))
					return true;

			return false;
		}
		
		/**
		* @brief Searches for the final states within a state set and returns them.
		* @param[in] state The state set that will be searched.
		* @return The final states within `state`, if any.
		**/
		inline FSMStateSetType _get_final_states_from_state_set(const FSMStateSetType& state) const
		{
			FSMStateSetType finalStates;

			for (auto s : state)
				if (this->getFinalStates().contains(s))
					finalStates.insert(s);

			return finalStates;
		}

		// static
		//! @brief The start state that will be used by the state machine.
		static constexpr FSMStateType START_STATE = 1;

	public:
		//! @brief Default constructor.
		FiniteStateMachine() = default;

		/**
		 * @brief The typical constructor that will be used to construct a state machine.
		 * @param[in] fStates The final states of the state machine.
		 * @param[in] tranFn The transition function of the state machine.
		 * @param[in] machineType The type of the state machine.
		 * @param[in] flags The flags given to the state machine.
		 */
		FiniteStateMachine(const FSMStateSetType& fStates, const TransFuncT& tranFn, FSM_TYPE machineType ,FlagsType flags) :
			m_FinalStates { fStates }, m_TransitionFunc{ tranFn }, m_MachineType {machineType}, m_Flags{flags}
			{
			
			if (fStates.empty()) {
				const std::string message = "FSM: The set of final states cannot be empty.";
				m_Logger.log(LoggerInfo::ERROR, message);
				throw InvalidStateMachineArgumentsException{ message };
			};

			if (machineType == FSM_TYPE::MT_MACHINE_TYPE_COUNT) {
				const std::string message = R"(FSM: The machine type is invalid.)";
				m_Logger.log(LoggerInfo::ERROR, message);
				throw InvalidStateMachineArgumentsException{ message };
			};
		
		};

		/**
		 * @brief Copy operator for the state machine.
		 * @param[in] rhs The right hand side (right argument) of the copy operator.
		 * @return A reference to the state machine after assignment.
		 */
		FiniteStateMachine& operator=(const FiniteStateMachine& rhs) {
			this->m_TransitionFunc = rhs.m_TransitionFunc;
			this->m_Flags = rhs.m_Flags;
			this->m_MachineType = rhs.m_MachineType;
			this->m_FinalStates = rhs.m_FinalStates;

			return *this;
		}

		//! @brief Gets the final states of the state machine.
		const FSMStateSetType& getFinalStates() const { return m_FinalStates; };

		//! @brief Gets the flags of the state machine.
		FlagsType getFlags() const { return m_Flags; };

		//! @brief Gets the type of the state machine. For a DFA, the type is always `FSM_TYPE::MT_DFA`; for an NFA it varies.
		FSM_TYPE getMachineType() const { return m_MachineType; };
	};

	struct Indicies {
		IndexType start = 0;
		IndexType end = 0;

		std::string toString() const {
			return std::format("({}, {})", start, end);
		}
		operator std::string() const {
			return toString();
		}

		bool operator==(const Indicies&) const = default;

		Indicies operator+(const size_t num) const {
			return Indicies{ start + num, end + num };
		}
	};
	struct FSMResult {

		// DATA MEMBERS AND NESTED TYPES
		bool accepted = false;
		FSMStateSetType finalState = { FiniteStateMachine<FSMStateType>::START_STATE };
		Indicies indecies;
		const std::string_view input;

		// UTILITY FUNCTIONS
		size_t size() const {
			return indecies.end - indecies.start;
		}
		std::string_view getMatch() const {
			return this->input.substr(indecies.start, this->size());
		}
		Indicies getIndecies() const {
			return indecies;
		}
	};

	//! @brief Represents a single matched substring.
	//! @note Not implmeneted yet.
	template<typename T = FSMStateType>
	struct Substring {
		std::vector<T> matchedStates;
		Indicies indecies;

		auto begin() const {
			return matchedStates.begin();
		}
		auto end() const {
			return matchedStates.end();
		}

		size_t size() const {
			return matchedStates.size();
		}

	};

};

