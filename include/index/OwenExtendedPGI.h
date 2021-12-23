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
    void forward_counting_per_weight_next_step(Game* g, lint::LargeNumber* ret_ptr, std::vector<epic::longUInt> weights, longUInt player_limit, bool first_step);
//	bigInt mBigTmp;
};

} // namespace epic::index

#endif 
