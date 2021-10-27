#ifndef EPIC_INDEX_OWEN_H_
#define EPIC_INDEX_OWEN_H_

#include "ItfPowerIndex.h"
#include "ItfUpperBoundApproximation.h"
#include "Array.h"

namespace epic::index {

class Owen : public ItfPowerIndex {
public:
	Owen(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);
	~Owen() override;

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
	longUInt getMemoryRequirement() override;

private:
	longUInt* mPartW; // Partition weights, mPartW[x]: weightsum of the precoalition x
	longUInt mNbPart; // amount of precoalitions
	longUInt mMaxPartSize; // the maximum amount of players inside a single precoalition

	lint::LargeNumber mTmp;
	lint::LargeNumber mTmp2;

	void coalitionsCardinalityContainingPlayerFromAbove(Array2dOffset<lint::LargeNumber>& cw, Array2dOffset<lint::LargeNumber>& cc, longUInt n_player, longUInt player, longUInt* weights);
	void generalizedBackwardCountingPerWeightCardinality(Array2dOffset<lint::LargeNumber>& cw2, longUInt* weights, longUInt n);
	void updateInternalShapleyShubik(lint::LargeNumber* internal_ssi, Array2dOffset<lint::LargeNumber>& cwi, longUInt precoalition, longUInt player, longUInt* weights, lint::LargeNumber* factorial, lint::LargeNumber& scale_factor);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_OWEN_H_ */
