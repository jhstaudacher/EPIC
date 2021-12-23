#ifndef EPIC_INDEX_NEVISONPH_H_
#define EPIC_INDEX_NEVISONPH_H_

#include "ItfUpperBoundApproximation.h"
#include "RawPublicHelpTheta.h"

namespace epic::index {

class NevisonPH : public RawPublicHelpTheta {
public:
	/**
	 * @param g The Game for which the Nevison index should later be calculated.
	 */
	NevisonPH(Game* g);

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_NEVISONPH_H_ */
