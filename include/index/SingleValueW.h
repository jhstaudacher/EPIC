#ifndef EPIC_INDEX_SINGLEVALUEW_H_
#define EPIC_INDEX_SINGLEVALUEW_H_

#include "RawBanzhaf.h"

namespace epic::index {

class SingleValueW : public RawBanzhaf {
public:
	SingleValueW(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
};

} // namespace epic::index

#endif //EPIC_INDEX_SINGLEVALUEW_H_
