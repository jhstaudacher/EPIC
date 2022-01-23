#ifndef EPIC_INDEX_BANZHAFOWENBELOW_H_
#define EPIC_INDEX_BANZHAFOWENBELOW_H_

#include "PowerIndexWithPrecoalitions.h"

namespace epic::index {

/**
 * Add here a description of the new power index for the Doxygen documentation generator.
 */
class BanzhafOwenBelow : public PowerIndexWithPrecoalitions {
	/*
 * Implement the ItfPowerIndex interface:
 */
public:
	BanzhafOwenBelow(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);
	~BanzhafOwenBelow() override;

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
	longUInt getMemoryRequirement() override;

private:
	lint::LargeNumber mTmp;
	bigInt mBigTmp;
};

} // namespace epic::index

#endif //EPIC_INDEX_BANZHAFOWENBELOW_H_
