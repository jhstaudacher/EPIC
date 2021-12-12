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
	BanzhafOwen();
	~BanzhafOwen();

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game* g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;

private:
	lint::LargeNumber mTmp;
	bigInt mBigTmp;
};

} // namespace epic::index

#endif //EPIC_INDEX_BANZHAFOWEN_H_
