#ifndef EPIC_INDEX_SYMMETRICCOALITIONALBANZHAF_H_
#define EPIC_INDEX_SYMMETRICCOALITIONALBANZHAF_H_

#include "PowerIndexWithPrecoalitions.h"

namespace epic::index {

class SymmetricCoalitionalBanzhaf : public PowerIndexWithPrecoalitions {
public:
	SymmetricCoalitionalBanzhaf();

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game* g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;

private:
	lint::LargeNumber mTmp; // temporary variable - avoids temporary allocations inside the loops.

	bigInt mBigTmp; // temporary variable - avoids temporary allocations inside the loops.

	void updateInternalShapleyShubik(PrecoalitionGame* g, bigInt* internal_ssi, Array2dOffset<lint::LargeNumber>& cwi, longUInt precoalition, longUInt player, std::vector<longUInt>& weights, bigInt* factorial);
};

} // namespace epic::index

#endif //EPIC_INDEX_SYMMETRICCOALITIONALBANZHAF_H_
