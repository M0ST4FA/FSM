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
#include <algorithm>

#include "utility/Logger.h"
#include "tabulate/table.hpp"
#include "fmt/ranges.h"

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

	// Forward declaration of `FSMTable`
	class FSMTable;

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
		 * @param[in] input The input used to access the entry corresponding to a given state.
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

		// STATIC

		//! @brief The start state that will be used by the state machine.
		static constexpr FSMStateType START_STATE = 1;

		//! @brief The dead state that will be used by the state machine.
		static constexpr FSMStateType DEAD_STATE = 0;

	public:

		/**
		* @brief Gets the start state of all automata.
		*/
		static constexpr FSMStateType getStartState() {
			return FiniteStateMachine<TransFuncT, InputT>::START_STATE;
		}

		/**
		* @brief Gets the dead state of all automata.
		*/
		static constexpr FSMStateType getDeadState()  {
			return FiniteStateMachine<TransFuncT, InputT>::DEAD_STATE;
		}

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
				m_Logger.log(LoggerInfo::LL_ERROR, message);
				throw InvalidStateMachineArgumentsException{ message };
			};

			if (machineType == FSM_TYPE::MT_MACHINE_TYPE_COUNT) {
				const std::string message = R"(FSM: The machine type is invalid.)";
				m_Logger.log(LoggerInfo::LL_ERROR, message);
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

		Logger logger;

	private:
		mutable VecType m_Table;

		std::vector<size_t> get_column_sizes() const {
			std::vector<size_t> columnSizes{};
			for (const auto& row : this->m_Table) {
				columnSizes.push_back(row.size());
			}

			return columnSizes;
		}

		static std::set<size_t> get_non_empty_columns(const VecType& tb) {

			std::set<size_t> indeciesOfNonEmptyColumns{};

			// loop through each row
			for (const auto& row : tb) {

				// loop through each column
				for (size_t i = 0; const auto & col : row) {

					auto atLeastOneNonDeadState = [&col]() {
						return std::none_of(col.begin(), col.end(), [](const auto& state) {
							return state == FiniteStateMachine<fsm::TransFn<fsm::FSMTable>>::getDeadState();
							});
						};

					if (col.empty()) {
						i++;
						continue;
					}

					// if column is empty
					if (atLeastOneNonDeadState())
						indeciesOfNonEmptyColumns.insert(i);

					i++;
				}
			}

			return indeciesOfNonEmptyColumns;
		}

		void filter_columns(const size_t maxColSize, const std::vector<bool>& colStatus, tabulate::Table& table) const {

			using namespace tabulate;

			Table::Row_t headerRow{};
			// Fill the first row (the header row)
			for (size_t col = 0; col < colStatus.size(); col++) {
				bool status = colStatus.at(col);

				// if the columns is empty
				if (status == false)
					continue;

				// the columns is filled
				headerRow.push_back(std::string{char(col)});
			}

			table.add_row(headerRow);
			table.row(0).format().font_style({ tabulate::FontStyle::bold }).font_color(Color::cyan);

			// Fill the rest of rows
			for (const auto& row : this->m_Table) {

				if (row.size() == 0)
					continue;

				Table::Row_t newRow{};
				for (size_t col = 0; col < maxColSize; col++)
					if (colStatus.at(col))
						newRow.push_back(fmt::format("{}", row.at(col)));

				table.add_row(newRow);
			}

		}

	public:

		operator std::string() const {
			return utility::toString<FSMStateSetType>(this->m_Table, &get_non_empty_columns);
		}

		/**
		 * @brief Accesses the table entry indexed by `state` and `input`.
		 * @param[in] state The state whose corresponding entry will be accessed.
		 * @param[in] input The input (typically character) used to access the entry corresponding to a given state.
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
		 * @param[in] input The input (typically character) used to access the entry corresponding to a given state.
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
		 * @brief Gets the size of the table (the number of rows in the table).
		 */
		size_t size() const {
			return this->m_Table.size();
		}

		/**
		 * @brief Same as operator[](const FSMStateType& state) const.
		 */
		const StateSetVecType& at(const FSMStateType& state) const {
			if (m_Table.size() <= state)
				m_Table.resize(state + 1);

			return m_Table.at(state);
		}

		template<typename InputT>
		FSMStateType set(const FSMStateType&, const std::basic_string<InputT>&);

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
		FSMStateSetType finalState = { FiniteStateMachine<FSMStateType>::getStartState()};
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
	 * @note Typically used only internally to record matched substrings.
	 */
	template<typename T = FSMStateType>
	struct Substring {
		/**
		 * @brief The path (that mimics the substring) through the FSM.
		 */
		std::vector<T> matchedStates;
		/**
		 * @brief The indicies of the substring within the input string.
		 */
		Indicies indicies;

		/**
		 * @brief Gets an iterator to the beginning of the path (that mimics the substring) through the FSM.
		 */
		auto begin() const {
			return matchedStates.begin();
		}
		/**
		 * @brief Gets an iterator to the end of the path (that mimics the substring) through the FSM.
		 */
		auto end() const {
			return matchedStates.end();
		}

		/**
		 * @brief Gets the number of states in the path (that mimics the substring) through the FSM.
		 */
		size_t size() const {
			return matchedStates.size();
		}

	};

};

// DEFINITIONS
namespace m0st4fa::fsm {

	/**
	* @brief Adds an entire string of input characters to the state machine, at once.
	* @details This function inserts the characters of the `input` string, one by one, starting from the `initState`. Each character is mapped to the next. The state that each one gets assigned to gets incremented by one for each character.
	* @param[in] initState The state that insertion will begin from. The other states that will be used to insert the following characters will go incrementally higher starting from this state.
	* @param[in] input The input string whose characters will be inserted into the table.
	* @return The state to which the last character of the input is mapped.
	* @bug Let's see.
	* @attention Do this and don't do that.
	*/
	template<typename InputT>
	FSMStateType FSMTable::set(const FSMStateType& initState, const std::basic_string<InputT>& input) { 

		// if the input is empty
		if (input.empty())
			return initState;

		FSMStateType nextState = initState + 1;

		this->operator()(initState, input.at(0)) = nextState;

		// if the input has one element only
		if (input.size() < 1)
			return nextState;

		// if the input has more than one element
		for (IndexType i = 1; i < input.size(); i++)
		{
			InputT c = input.at(i);
			this->operator()(nextState, c) = nextState + 1;
			nextState++;
		}

		return nextState; // subtract one to compensate for the last one added in the loop.
	};

}
