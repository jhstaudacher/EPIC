#ifndef EPIC_INDEX_THREATPGI1_H_
#define EPIC_INDEX_THREATPGI1_H_

#include "PowerIndexWithPrecoalitions.h"

namespace epic::index {

class ThreatPGI1 : public PowerIndexWithPrecoalitions {
public:
	ThreatPGI1();

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game* g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;
};

} // namespace epic::index

#endif 
