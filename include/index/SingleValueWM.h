#ifndef EPIC_INDEX_SINGLEVALUEWM_H_
#define EPIC_INDEX_SINGLEVALUEWM_H_

#include "ItfPowerIndex.h"

namespace epic::index {

class SingleValueWM : public ItfPowerIndex {
public:
	SingleValueWM(Game& g);

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
	longUInt getMemoryRequirement() override;
};

} // namespace epic::index

#endif //EPIC_INDEX_SINGLEVALUEWM_H_
