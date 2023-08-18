#include "fsm.h"
#include "gtest/gtest.h"

using FSMStateSetType = m0st4fa::FSMStateSetType;
using TableType = m0st4fa::FSMTable;
using TranFn = m0st4fa::TransFn<TableType>;
using DFAType = m0st4fa::DeterFiniteAutomatan<TranFn>;
using Result = m0st4fa::FSMResult;

INSTANTIATE_TYPED_TEST_SUITE_P(DFATests, FSMTests, DFAType);