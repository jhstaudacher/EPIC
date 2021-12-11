#ifndef EPIC_INDEX_NEVISONPH_H_
#define EPIC_INDEX_NEVISONPH_H_

#include "ItfUpperBoundApproximation.h"
#include "RawPublicHelpTheta.h"

namespace epic::index {

class NevisonPH : public RawPublicHelpTheta {
public:
	/**
	 * Construct the Nevison object
	 *
	 * @param g The Game for which the Nevison index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	NevisonPH(Game& g);

	std::vector<bigFloat> calculate(Game& g) override;
	std::string getFullName() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_NEVISONPH_H_ */
