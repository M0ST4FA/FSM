#pragma once
#include <string>
#include <iostream>
#include <map>
#include <algorithm>

#include "fsm/DFA.h"
#include "utility/common.h"
#include "utility/Logger.h"
#include "universal.h"

struct FSMSharedInfo {

protected:

	using FSMStateSetType = m0st4fa::fsm::FSMStateSetType;
	using FSMTableType = m0st4fa::fsm::FSMTable;
	using TranFn = m0st4fa::fsm::TransFn<FSMTableType>;
	using DFAType = m0st4fa::fsm::DeterFiniteAutomaton<TranFn>;
	using Result = m0st4fa::fsm::FSMResult;

	template<typename T = FSMTableType>
	static constexpr void initTranFn_ab(T& fun) {
		// corresponding regex: /aa(a*ba?)+b(a|b)*/
		// corresponding regex: /aa*bb*/

		fun(1, 'a') = 2;
		fun(1, 'b') = 3;
		fun(2, 'a') = 2;
		fun(2, 'b') = 3;
		fun(3, 'b') = 4;
		fun(4, 'b') = 4;

	}

	template<typename T>
	static constexpr void initTranFn_a(T& fun) {
		// corresponding regex: /aa(a*ba?)+b(a|b)*/
		// corresponding regex: /aa*bb*/
		
		fun(1, 'a')= 2;
		fun(2, 'a')= 2;

	}

	template<typename T>
	static constexpr void initTranFn_id_eq_num(T& fun) {
		// corresponding regex: /\w+(\w|\d)*|=|\d+/

		

		for (char c = 'a'; c <= 'z'; c++)
			fun(1, c) = 2;

		for (char c = 'a'; c <= 'z'; c++)
			fun(2, c) = 2;

		for (char c = '0'; c <= '9'; c++)
			fun(2, c) = 2;

		// /=/
		fun(1, '=') = 3;

		// /\d+/
		for (char c = '0'; c <= '9'; c++)
			fun(1, c) = fun(4, c) = 4;

	}

};

