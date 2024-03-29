
#include "fsm/FiniteStateMachine.h"

std::ostream& m0st4fa::fsm::operator<<(const std::ostream& os, const FSMResult& result)
{
	Logger logger;
	
	std::string temp = "{ ";
	temp += std::to_string(*result.finalState.begin());
	for (auto s : result.finalState) {
		if (s == *result.finalState.begin())
			continue;
		
		temp += (", " + std::to_string(s));
	}
	temp += " }";

	IndexType startIndex = result.indicies.start;
	IndexType endIndex = result.indicies.end;

	logger.log(LoggerInfo::LL_INFO, std::format("Accepted string: {}\nIndecies of the match: {{{}, {}}}\nFinal states reached: {}",
		result.accepted, startIndex, endIndex, temp.data()));
	std::string_view temp2{result.input.data() + startIndex, endIndex};
	logger.logDebug(std::format("Matched string: {}\n", temp2));

	return std::cout;
}

