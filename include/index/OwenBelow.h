#ifndef EPIC_INDEX_OWENBELOW_H_
#define EPIC_INDEX_OWENBELOW_H_

#include "Array.h"
#include "ItfUpperBoundApproximation.h"
#include "PowerIndexWithPrecoalitions.h"

namespace epic::index {

class OwenBelow : public PowerIndexWithPrecoalitions {
public:
	OwenBelow();

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game* g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;

private:
	/**
	 * temporary variable - avoids temporary allocations inside the loops.
	 */
	lint::LargeNumber mTmp;

	/**
	 * temporary variable - avoids temporary allocations inside the loops.
	 */
	bigInt mBigTmp;

	void updateInternalShapleyShubik(PrecoalitionGame* g, bigInt* internal_ssi, Array2d<lint::LargeNumber>& cwoi, longUInt precoalition, longUInt player, std::vector<longUInt>& weights, bigInt* factorial, bigInt& scale_factor, int s);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_OWENBELOW_H_ */
