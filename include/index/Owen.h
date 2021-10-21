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
	void backwardCountingPerWeightAndCardinality(Array2dOffset<lint::LargeNumber>& cc, longUInt* partition_weights);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_OWEN_H_ */
