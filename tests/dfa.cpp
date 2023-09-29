#include "fsm.h"
#include "gtest/gtest.h"

using FSMStateSetType = m0st4fa::fsm::FSMStateSetType;
using TableType = m0st4fa::fsm::FSMTable;
using TranFn = m0st4fa::fsm::TransFn<TableType>;
using DFAType = m0st4fa::fsm::DeterFiniteAutomaton<TranFn>;
using Result = m0st4fa::fsm::FSMResult;

INSTANTIATE_TYPED_TEST_SUITE_P(DFATests, FSMTests, DFAType);