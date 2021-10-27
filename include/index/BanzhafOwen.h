#ifndef EPIC_INDEX_NEWINDEX_H_
#define EPIC_INDEX_NEWINDEX_H_

#include "PowerIndexWithPrecoalitions.h"

namespace epic::index {

/**
 * Add here a description of the new power index for the Doxygen documentation generator.
 */
class BanzhafOwen : public PowerIndexWithPrecoalitions {
/*
 * Implement the ItfPowerIndex interface:
 */
public:
	BanzhafOwen(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);
	~BanzhafOwen() override;

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
	longUInt getMemoryRequirement() override;
};

} // namespace epic::index

#endif //EPIC_INDEX_BANZHAFOWEN_H_
