#ifndef EPIC_INDEX_NEWINDEX_H_
#define EPIC_INDEX_NEWINDEX_H_

#include "RawPublicGood.h"

namespace epic::index {

/**
 * Add here a description of the new power index for the Doxygen documentation generator.
 */
class NewIndex : public RawPublicGood {
/*
 * Override the RawPublicGood methods as required:
 */
public:
	NewIndex(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;

	// longUInt getMemoryRequirement() override; // only if your memory requirement differs from the RawPublicGood
};

} // namespace epic::index

#endif //EPIC_INDEX_NEWINDEX_H_
