#pragma once
#include <string>
#include <iostream>
#include <map>
#include <algorithm>

#include "../src/FiniteStateMachine.h"
#include "../src/DFA.h"
#include "../utility/common.h"
#include "../utility/Logger.h"

struct FSMSharedInfo {

protected:

	using FSMStateSetType = m0st4fa::FSMStateSetType;
	using FSMTableType = m0st4fa::FSMTableType;
	using TranFn = m0st4fa::TransFn<FSMTableType>;
	using DFAType = m0st4fa::DeterFiniteAutomatan<TranFn>;
	using Result = m0st4fa::FSMResult;

	template<typename T>
	static constexpr void initTranFn_ab(T& fun) {
		// corresponding regex: /aa(a*ba?)+b(a|b)*/
		// corresponding regex: /aa*bb*/

		fun.resize(5);
		std::for_each(fun.begin(), fun.end(), [](auto& t) {
			t.resize(128);
			});
		fun[1]['a'] = 2;
		fun[1]['b'] = 3;
		fun[2]['a'] = 2;
		fun[2]['b'] = 3;
		fun[3]['b'] = 4;
		fun[4]['b'] = 4;
	}

	template<typename T>
	static constexpr void initTranFn_a(T& fun) {
		// corresponding regex: /aa(a*ba?)+b(a|b)*/
		// corresponding regex: /aa*bb*/

		fun.resize(3);
		std::for_each(fun.begin(), fun.end(), [](auto& t) {
			t.resize(128);
			});
		fun[1]['a'] = 2;
		fun[2]['a'] = 2;

	}

	template<typename T>
	static constexpr void initTranFn_id_eq_num(T& fun) {
		// corresponding regex: /\w+(\w|\d)*|=|\d+/

		fun.resize(5);
		std::for_each(fun.begin(), fun.end(), [](auto& t) {
			t.resize(128);
			});

		for (char c = 'a'; c <= 'z'; c++)
			fun[1][c] = 2;

		for (char c = 'a'; c <= 'z'; c++)
			fun[2][c] = 2;

		for (char c = '0'; c <= '9'; c++)
			fun[2][c] = 2;

		// /=/
		fun[1]['='] = 3;

		// /\d+/
		for (char c = '0'; c <= '9'; c++)
			fun[1][c] = fun[4][c] = 4;

	}

};

