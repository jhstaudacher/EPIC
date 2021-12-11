#ifndef EPIC_INDEX_OWEN_H_
#define EPIC_INDEX_OWEN_H_

#include "Array.h"
#include "ItfUpperBoundApproximation.h"
#include "PowerIndexWithPrecoalitions.h"

namespace epic::index {

class Owen : public PowerIndexWithPrecoalitions {
public:
	Owen();
	~Owen();

	std::vector<bigFloat> calculate(Game& g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game& g) override;
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

	void updateInternalShapleyShubik(Game& g, bigInt* internal_ssi, Array2dOffset<lint::LargeNumber>& cwi, longUInt precoalition, longUInt player, std::vector<longUInt>& weights, bigInt* factorial, bigInt& scale_factor);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_OWEN_H_ */
