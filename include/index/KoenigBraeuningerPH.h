#ifndef EPIC_INDEX_KOENIGBRAEUNINGERPH_H_
#define EPIC_INDEX_KOENIGBRAEUNINGERPH_H_

#include "ItfUpperBoundApproximation.h"
#include "RawPublicHelpTheta.h"

namespace epic::index {

class KoenigBraeuningerPH : public RawPublicHelpTheta {
public:
	/**
	 * Construct the KönigBräuninger object
	 *
	 * @param g The Game for which the KönigBräuninger index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	KoenigBraeuningerPH(Game* g);

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_KOENIGBRAEUNINGERPH_H_ */
