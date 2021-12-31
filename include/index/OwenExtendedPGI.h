#ifndef EPIC_INDEX_OWENEXTENDEDPGI_H_
#define EPIC_INDEX_OWENEXTENDEDPGI_H_

#include "PowerIndexWithPrecoalitions.h"

namespace epic::index {

class OwenExtendedPGI : public PowerIndexWithPrecoalitions {
public:
	OwenExtendedPGI();

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game* g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;

private:
	void updateInterm(Game* g, lint::LargeNumber interm[], longUInt weight_limit);
};

} // namespace epic::index

#endif // EPIC_INDEX_OWENEXTENDEDPGI_H_
