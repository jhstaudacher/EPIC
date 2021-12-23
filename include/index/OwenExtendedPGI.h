#ifndef EPIC_INDEX_OWENEXTENDEDPGI_H_
#define EPIC_INDEX_OWENEXTENDEDPGI_H_

#include "PowerIndexWithPrecoalitions.h"

namespace epic::index {

/**
 * Add here a description of the new power index for the Doxygen documentation generator.
 */
class OwenExtendedPGI : public PowerIndexWithPrecoalitions {
	/*
 * Implement the ItfPowerIndex interface:
 */
public:
	OwenExtendedPGI();

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game* g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;

//private:
//	bigInt mBigTmp;
};

} // namespace epic::index

#endif 
