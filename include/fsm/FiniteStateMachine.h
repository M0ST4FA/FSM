#pragma once

#include <set>
#include <unordered_set>
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <source_location>
#include <functional>
#include <concepts>

#include "utility/Logger.h"

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

// TYPE ALIASES AND CONCEPTS (AND A RELATED STRUCT)
namespace m0st4fa::fsm {

	/**
	 * @brief A finite state machine state. 
	 * @remark It is used by FiniteStateMachine objects and their descendants (objects of derived classes).
	 */
	using FSMStateType = unsigned;

	/**
	 * @brief A set of m0st4fa::fsm::FSMStateType objects. 
	 * @remark It is used by FiniteStateMachine objects and their descendants (objects of derived classes).
	 */
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

	/**
	 * @brief Flags given to finite state machine upon initialization.
	 * @note Not implemented yet.
	 */
	using FlagsType = unsigned;

	//! @brief Index into some collection.
	using IndexType = unsigned long long;

	/**
	 * @brief Ensure that `T` is the same as m0st4fa::fsm::FSMStateSetType or is the same as std::vector<m0st4fa::fsm::FSMStateType>.
	 */
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

	/**
	 * @brief A transition table used to decide on which states comes next when simulating.
	 */
	class FSMTable {
		
	protected:
		/**
		 * @brief The type of a vector of FSMStateSetType.
		 * @remark Used here as the type of a table entry. It stores the set of states mapped to a given state.
		 */
		using StateSetVecType = std::vector<FSMStateSetType>;

		/**
		 * @brief The internal type of the table (the table is an abstraction over this type).
		 * @note I've chosen the internal type of the table to be a std::vector instead of a std::map or something else, because I wanted performance, given that the typical range of states will not be large. For this reason, it is advisable to have a set of states that falls within a tight range, as this will influence the size of the table used by the machine.
		 */
		using VecType = std::vector<StateSetVecType>;

		//! @brief The type of an non-constant iterator.
		using ItType = VecType::iterator;
		//! @brief The type of a constant iterator.
		using ConstItType = VecType::const_iterator;

	private:
		mutable VecType m_Table;
		
	public:

		/**
		 * @brief Accesses the table entry indexed by `state` and `input`.
		 * @param[in] state The state whose corresponding entry will be accessed.
		 * @param input The input (typically character) used to access the entry corresponding to a given state.
		 * @return A reference to the table entry indexed by `state` and `input`.
		 */
		template<typename InputT = char>
		FSMStateSetType& operator()(const FSMStateType& state, const InputT input) noexcept(true) {
			
			if (m_Table.size() <= state)
				m_Table.resize(state + 1);
			
			std::vector<FSMStateSetType>& stateMap = m_Table.at(state);

			if (stateMap.size() <= input)
				stateMap.resize(input + 1);

			return stateMap.at(input);
		}

		/**
		 * @brief Accesses the table entry indexed by `state` and `input`.
		 * @param[in] state The state whose corresponding entry will be accessed.
		 * @param input The input (typically character) used to access the entry corresponding to a given state.
		 * @return A constant reference to the table entry indexed by `state` and `input`.
		 */
		template<typename InputT = char>
		const FSMStateSetType& operator()(const FSMStateType& state, const InputT input) const noexcept(true) {
			if (m_Table.size() <= state)
				m_Table.resize(state + 1);

			std::vector<FSMStateSetType>& stateMap = m_Table.at(state);

			if (stateMap.size() <= input)
				stateMap.resize(input + 1);

			// Impossible to throw an exception, as sizes of both dimensions have been already checked.
			return stateMap.at(input);
		}

		/**
		 * @brief Accesses the set of states corresponding to `state` (on all of its characters).
		 * @param[in] state The state used to index the table.
		 * @return A *vector* of *sets of states*. `state` is mapped to each set of states in this vector via some input character (you can get the set of states corresponding to a given input (assuming it exists) by indexing the vector). 
		 */
		const StateSetVecType& operator[](const FSMStateType& state) const {
			if (m_Table.size() <= state)
				m_Table.resize(state + 1);

			return m_Table.at(state);
		}

		/**
		 * @brief Same as operator[](const FSMStateType& state) const.
		 */
		const StateSetVecType& at(const FSMStateType& state) const {
			if (m_Table.size() <= state)
				m_Table.resize(state + 1);

			return m_Table.at(state);
		}

		//! @return Iterator to the beginning.
		ItType begin() {
			return m_Table.begin();
		}
		//! @return Constant iterator to the beginning.
		ConstItType begin() const {
			return m_Table.begin();
		}

		//! @return Iterator to the end.
		ItType end() {
			return m_Table.end();
		}
		//! @return Constant iterator the end.
		ConstItType end() const {
			return m_Table.end();
		}

	};

	/**
	 * @brief A transition function used to decide on which states comes next when simulating
	 * @note This is just an abstraction over m0st4fa::fsm::FSMTable. It is advisable to use this directly instead of building a m0st4fa::fsm::FSMTable object first and then passing it to this.
	 */
	template <typename TableT = FSMTable>
	class TransitionFunction {

		TableT m_Table{};
		
	public:
		TransitionFunction() = default;
		TransitionFunction(const TableT& table) : m_Table(table) {}
		
		/**
		 * @brief Accesses the table entry indexed by `state` and `input`.
		 * @param[in] state The state whose corresponding entry will be accessed.
		 * @param input The input used to access the entry corresponding to a given state.
		 * @return A reference to the table entry indexed by `state` and `input`.
		 */
		template<typename InputT>
		FSMStateSetType& operator()(const FSMStateType& state, const InputT c) noexcept(true) {
			return this->m_Table(state, c);
		}

		/**
		 * @brief Accesses the table entry indexed by `state` and `input`.
		 * @param[in] state The state whose corresponding entry will be accessed.
		 * @param input The input used to access the entry corresponding to a given state.
		 * @return A copy of the table entry indexed by `state` and `input`.
		 */
		template <typename InputT>
		FSMStateSetType operator()(const FSMStateType state, const InputT input) const noexcept(true) {
			return this->m_Table(state, input);
		}

		/**
		 * @brief Accesses the table entries indexed by every state in `stateSet` set and `input`.
		 * @details You can think of this as a loop over every state in `stateSet`. Entries are accessed in each iteration using the state from `stateSet` corresponding to that iteration and `input`. The entires are then collected in a FSMStateSetType object and returned.
		 * @param[in] stateSet The set of states whose corresponding entires will be accessed.
		 * @param input The input used to access the entry corresponding to a given state.
		 * @return A collection of table entries corresponding to the table entries indexed by every state in `stateSet` set and `input`.
		 */
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

	/**
	 * @brief Indicies of matches by an FSM.
	 */
	struct Indicies {
		/**
		 * @brief The index of the first-matched letter.
		 */
		IndexType start = 0; 
		/**
		 * @brief The index of the letter after the last-matched letter.
		 */
		IndexType end = 0;   

		/**
		 * @brief Converts this Indicies object to a string.
		 */
		operator std::string() const {
			return toString();
		}
		/**
		 * @brief Converts this Indicies object to a string. Uses the `Indicies::operator std::string() const` behind the scenes.
		 */
		std::string toString() const {
			return std::format("({}, {})", start, end);
		}

		/**
		 * @brief Compares this Indicies object with another for equality.
		 */
		bool operator==(const Indicies&) const = default;

		/**
		 * @brief Creates a new Indicies object whose indicies equal the indicies of this object + `num`.
		 * @param[in] num The number that will offset `start` and `end` of this Indicies object in the new object.
		 * @return A new Indicies object with `start = this->start + num` and `end = this->end + num`.
		 */
		Indicies operator+(const size_t num) const {
			return Indicies{ start + num, end + num };
		}
	};

	/**
	 * @brief The result of a simulation by some state machine against some string.
	 */
	struct FSMResult {

		// DATA MEMBERS AND NESTED TYPES
		/**
		 * @brief Whether the string was accepted by simulation. `true` if the string was accepted; `false` otherwise.
		 */
		bool accepted = false;
		/**
		 * @brief The final states used for this simulation.
		 */
		FSMStateSetType finalState = { FiniteStateMachine<FSMStateType>::START_STATE };
		/**
		 * @brief The indicies of the accepting string, if any. If no string accepts, both indicies will be 0.
		 */
		Indicies indicies;
		/**
		 * @brief The input string that the simulation was performed against.
		 */
		const std::string_view input;

		// UTILITY FUNCTIONS
		/**
		 * @brief Returns the size of the matched string, if any. In case no input matches, it returns 0.
		 */
		size_t size() const {
			return indicies.end - indicies.start;
		}
		/**
		 * @brief Gets (extracts) the matched string from the input against which the match was performed.
		 */
		std::string_view getMatch() const {
			return this->input.substr(indicies.start, this->size());
		}
		/**
		 * @brief Gets the indicies of the match.
		 */
		Indicies getIndecies() const {
			return indicies;
		}
	};

	/**
	 * @brief Represents a single matched substring as the path through an FSM.
	 * @note Typically used internally to record matched substrings.
	 */
	template<typename T = FSMStateType>
	struct Substring {
		std::vector<T> matchedStates;
		Indicies indicies;

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

