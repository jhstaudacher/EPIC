#ifndef EPIC_INDEX_SINGLEVALUEWS_H_
#define EPIC_INDEX_SINGLEVALUEWS_H_

#include "ItfPowerIndex.h"

namespace epic::index {

class SingleValueWS : public ItfPowerIndex {
public:
	SingleValueWS(Game& g);

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
	longUInt getMemoryRequirement() override;
};

} // namespace epic::index

#endif //EPIC_INDEX_SINGLEVALUEWS_H_
