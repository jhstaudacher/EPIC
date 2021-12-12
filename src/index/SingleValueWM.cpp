#include "index/SingleValueWM.h"

epic::index::SingleValueWM::SingleValueWM() : DeeganPackel() {}

std::vector<epic::bigFloat> epic::index::SingleValueWM::calculate(Game* g) {
	bigInt total_mwc;
	numberOfMinimalWinningCoalitions(g, &total_mwc);

	return {static_cast<bigFloat>(total_mwc)};
}

std::string epic::index::SingleValueWM::getFullName() {
	return "Number of minimal winning coalitions";
}
