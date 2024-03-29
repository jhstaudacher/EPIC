#ifndef EPIC_INDEX_SYMMETRICCOALITIONALBANZHAFBELOW_H_
#define EPIC_INDEX_SYMMETRICCOALITIONALBANZHAFBELOW_H_

#include "PowerIndexWithPrecoalitions.h"

namespace epic::index {

/**
 * Add here a description of the new power index for the Doxygen documentation generator.
 */
class SymmetricCoalitionalBanzhafBelow : public PowerIndexWithPrecoalitions {
	/*
 * Implement the ItfPowerIndex interface:
 */
public:
	SymmetricCoalitionalBanzhafBelow();

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

	void updateInternalShapleyShubik(PrecoalitionGame* g, bigInt* internal_ssi, Array2d<lint::LargeNumber>& cwi, longUInt precoalition, longUInt player, std::vector<longUInt>& weights, bigInt* factorial);
};

} // namespace epic::index

#endif //EPIC_INDEX_SYMMETRICCOALITIONALBANZHAFBELOW_H_
