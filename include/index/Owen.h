#ifndef EPIC_INDEX_OWEN_H_
#define EPIC_INDEX_OWEN_H_

#include "PowerIndexWithPrecoalitions.h"
#include "ItfUpperBoundApproximation.h"
#include "Array.h"

namespace epic::index {

class Owen : public PowerIndexWithPrecoalitions {
public:
	Owen(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);
	~Owen() override;

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
	longUInt getMemoryRequirement() override;

private:
	/**
	 * temporary variable - avoids temporary allocations inside the loops.
	 */
	lint::LargeNumber mTmp;

	/**
	 * temporary variable - avoids temporary allocations inside the loops.
	 */
	bigInt mBigTmp;

	void updateInternalShapleyShubik(bigInt* internal_ssi, Array2dOffset<lint::LargeNumber>& cwi, longUInt precoalition, longUInt player, longUInt* weights, bigInt* factorial, bigInt& scale_factor);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_OWEN_H_ */
