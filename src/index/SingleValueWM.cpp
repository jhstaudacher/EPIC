#include "index/SingleValueWM.h"

epic::index::SingleValueWM::SingleValueWM(Game& g) : ItfPowerIndex(g) {

}

std::vector<epic::bigFloat> epic::index::SingleValueWM::calculate() {
	return {0.2};
}

std::string epic::index::SingleValueWM::getFullName() {
	return "Number of minimal winning coalitions";
}

epic::longUInt epic::index::SingleValueWM::getMemoryRequirement() {
	return 0;
}
