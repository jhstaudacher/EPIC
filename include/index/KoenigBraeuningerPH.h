#ifndef EPIC_INDEX_KOENIGBRAEUNINGERPH_H_
#define EPIC_INDEX_KOENIGBRAEUNINGERPH_H_

#include "ItfUpperBoundApproximation.h"
#include "RawPublicHelpTheta.h"

namespace epic::index {

class KoenigBraeuningerPH : public RawPublicHelpTheta {
public:
	/**
	 * @param g The Game for which the KönigBräuninger index should later be calculated.
	 */
	KoenigBraeuningerPH(Game* g);

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_KOENIGBRAEUNINGERPH_H_ */
