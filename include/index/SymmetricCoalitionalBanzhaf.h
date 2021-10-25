#ifndef EPIC_INDEX_SYMMETRICCOALITIONALBANZHAF_H_
#define EPIC_INDEX_SYMMETRICCOALITIONALBANZHAF_H_

#include "ItfPowerIndex.h"
#include "../ItfUpperBoundApproximation.h"
#include "PowerIndexWithWinningCoalitions.h"

namespace epic::index {

/**
 * Add here a description of the new power index for the Doxygen documentation generator.
 */
class SymmetricCoalitionalBanzhaf : public ItfPowerIndex {
/*
 * Implement the ItfPowerIndex interface:
 */
public:
	SymmetricCoalitionalBanzhaf(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);
	~SymmetricCoalitionalBanzhaf() override;

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
	longUInt getMemoryRequirement() override;

private:
	longUInt* mPartW; 		// Partition weights, mPartW[x]: weightsum of the precoalition x
	longUInt mNbPart; 		// amount of precoalitions
	longUInt mMaxPartSize; 	// the maximum amount of players inside a single precoalition

};

} // namespace epic::index

#endif //EPIC_INDEX_SYMMETRICCOALITIONALBANZHAF_H_
