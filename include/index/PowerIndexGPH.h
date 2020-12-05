#ifndef EPIC_INDEX_POWERINDEXGPH_H_
#define EPIC_INDEX_POWERINDEXGPH_H_

#include "ItfUpperBoundApproximation.h"
#include "RawPublicHelpTheta.h"
#include "lint/ItfLargeNumberCalculator.h"

namespace epic::index {

class PowerIndexGPH : public RawPublicHelpTheta {
public:
	/**
	 * Construct the PowerIndexG object
	 * This index is basically Public Good, but not calculated with minimal winning coalitions alone, but with all null player free winning coalitions
	 * This index makes only sense, if null players are filtered out. See Alvarez-Mozos, M., Ferreira, F., Alonso-Meijide, J.M. and Pinto, A.A. (2015) Characterizations of power indices based on null player free winning coalitions for more information, especially the index g definition 4.2
	 *
	 * @param g The Game for which the PowerIndexG index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	PowerIndexGPH(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
	longUInt getMemoryRequirement() override;
};

} // namespace epic::index

#endif /* EPIC_INDEX_POWERINDEXGPH_H_ */
