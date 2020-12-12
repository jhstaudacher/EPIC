#ifndef EPIC_INDEX_SINGLEVALUEW_H_
#define EPIC_INDEX_SINGLEVALUEW_H_

#include "ItfPowerIndex.h"

namespace epic::index {

class SingleValueW : public ItfPowerIndex {
public:
	SingleValueW(Game& g);

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
	longUInt getMemoryRequirement() override;
};

} // namespace epic::index

#endif //EPIC_INDEX_SINGLEVALUEW_H_
