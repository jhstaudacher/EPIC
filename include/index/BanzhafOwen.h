#ifndef EPIC_INDEX_BANZHAFOWEN_H_
#define EPIC_INDEX_BANZHAFOWEN_H_

#include "PowerIndexWithPrecoalitions.h"

namespace epic::index {

class BanzhafOwen : public PowerIndexWithPrecoalitions {
public:
	BanzhafOwen();

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game* g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;

private:
	bigInt mBigTmp;
};

} // namespace epic::index

#endif //EPIC_INDEX_BANZHAFOWEN_H_
