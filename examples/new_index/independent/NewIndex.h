#ifndef EPIC_INDEX_NEWINDEX_H_
#define EPIC_INDEX_NEWINDEX_H_

#include "ItfPowerIndex.h"
#include "ItfUpperBoundApproximation.h"

namespace epic::index {

/**
 * Add here a description of the new power index for the Doxygen documentation generator.
 */
class NewIndex : public ItfPowerIndex {
	/*
 * Implement the ItfPowerIndex interface:
 */
public:
	NewIndex(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);
	~NewIndex() override;

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
	longUInt getMemoryRequirement() override;
};

} // namespace epic::index

#endif //EPIC_INDEX_NEWINDEX_H_
