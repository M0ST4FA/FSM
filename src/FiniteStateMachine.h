#pragma once

#include <set>
#include <unordered_set>
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <source_location>
#include <functional>

#include "../utility/Logger.h"
#include <concepts>

namespace m0st4fa {

	// TYPE ALIASES
	using FSMStateType = unsigned;
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


	// ENUMS

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
		//FF_CASE_INSENSITIVE,
		//FF_CASE_SENSITIVE,
		FF_FLAG_NONE = 0b0000000,
		FF_FLAG_MAX
	};

	using FSMTableType = std::vector<std::vector<FSMStateSetType>>;

	template <typename TableT = FSMTableType>
	struct TransitionFunction {
		TableT m_Function;
		
		TransitionFunction() = default;
		TransitionFunction(const TableT& function) : m_Function(function) {}

		// TODO: add this overload
		/*
			TransitionFunction(std::function<void, const TableT&> function) : m_Function(function(TableT())), m_StateMax{ m_Function.size() }, m_InputMa{ m_Function.at(0).size() } {};
		*/
		
		template <typename InputT>
		FSMStateSetType operator()(const FSMStateType state, const InputT input) const {
			return m_Function.at(state).at(input);
		}

		template <typename InputT>
		FSMStateSetType operator()(const FSMStateSetType& stateSet, const InputT input) const {
			FSMStateSetType res;
			
			for (FSMStateType state : stateSet) {
				FSMStateSetType tmp = m_Function.at(state).at(input);
				res.insert(tmp.begin(), tmp.end());
			}
				
			return res;
		}


	};

	template<typename TableT>
	using TransFn = TransitionFunction<TableT>;

	struct FSMResult;
	std::ostream& operator<<(const std::ostream&, const FSMResult&);

	template <typename TransFuncT, typename InputT = std::string_view>
	class FiniteStateMachine {

		// friends
		friend struct FSMResult;
		
		// private instance data members
		FSMStateSetType m_FinalStates{};
		FSMType m_MachineType;
		FlagsType m_Flags;

	protected:

		Logger m_Logger;
		TransFuncT m_TransitionFunc;

		/**
		* @return `true` if a state set is a final state set (contains at least one final state); `false` otherwise.
		**/
		inline bool _is_state_final(const FSMStateSetType& state) const
		{

			for (auto s : state)
				if (this->getFinalStates().contains(s))
					return true;

			return false;
		}
		
		/**
		* @return the final states within a state set (if any)
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
		static constexpr FSMStateType START_STATE = 1;

	public:
		FiniteStateMachine() = default;
		FiniteStateMachine(const FSMStateSetType& fStates, const TransFuncT& tranFn, FSMType machineType ,FlagsType flags) :
			m_FinalStates { fStates }, m_TransitionFunc{ tranFn }, m_MachineType {machineType}, m_Flags{flags}
			{
			
			LoggerInfo loggerInfo = { 
				  .level = LOG_LEVEL::LL_ERROR, 
				  .info = { .errorType = ERROR_TYPE::ET_INVALID_ARGUMENT} 
			  };

			if (fStates.empty()) {
				const std::string message = "FiniteStateMachine: the set of final states cannot be empty.";
				m_Logger.log(loggerInfo, message);
				throw std::invalid_argument(message);
			};

			if (machineType == FSMType::MT_MACHINE_TYPE_MAX) {
				const std::string message = R"(FiniteStateMachine: the machine type is invalid.)";
				m_Logger.log(loggerInfo, message);
				throw std::invalid_argument(message);
			};
		
		};
		FiniteStateMachine& operator=(const FiniteStateMachine& rhs) {
			this->m_TransitionFunc = rhs.m_TransitionFunc;
			this->m_Flags = rhs.m_Flags;
			this->m_MachineType = rhs.m_MachineType;
			this->m_FinalStates = rhs.m_FinalStates;

			return *this;
		}

		const FSMStateSetType& getFinalStates() const { return m_FinalStates; };
		FlagsType getFlags() const { return m_Flags; };
		FSMType getMachineType() const { return m_MachineType; };
	};

	struct Indecies {
		IndexType start = 0;
		IndexType end = 0;

		std::string toString() const {
			return std::format("({}, {})", start, end);
		}
		operator std::string() const {
			return toString();
		}

		bool operator==(const Indecies&) const = default;

		Indecies operator+(const size_t num) const {
			return Indecies{ start + num, end + num };
		}
	};
	struct FSMResult {
		// DATA MEMBERS AND NESTED TYPES
		bool accepted = false;
		FSMStateSetType finalState = { FiniteStateMachine<FSMStateType>::START_STATE };
		Indecies indecies;
		const std::string_view input;

		// UTILITY FUNCTIONS
		size_t size() const {
			return indecies.end - indecies.start;
		}
		std::string_view getMatch() const {
			return this->input.substr(indecies.start, this->size());
		}
		Indecies getIndecies() const {
			return indecies;
		}
	};

	// @brief represents a single matched substring.
	template<typename T = FSMStateType>
	struct Substring {
		std::vector<T> matchedStates;
		Indecies indecies;

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

