#include "index/SingleValueWS.h"

epic::index::SingleValueWS::SingleValueWS(Game& g) : ItfPowerIndex(g) {

}

std::vector<epic::bigFloat> epic::index::SingleValueWS::calculate() {
	return {0.4};
}

std::string epic::index::SingleValueWS::getFullName() {
	return "Number of minimal winning coalitions of smallest cardinality";
}

epic::longUInt epic::index::SingleValueWS::getMemoryRequirement() {
	return 0;
}
