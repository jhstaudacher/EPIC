#ifndef EPIC_INDEX_SINGLEVALUEWNP_H_
#define EPIC_INDEX_SINGLEVALUEWNP_H_

#include "ItfPowerIndex.h"

namespace epic::index {

class SingleValueWNP : public ItfPowerIndex {
public:
	SingleValueWNP(Game& g);

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
	longUInt getMemoryRequirement() override;
};

} // namespace epic::index

#endif //EPIC_INDEX_SINGLEVALUEWNP_H_
