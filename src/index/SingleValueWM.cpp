#include "index/SingleValueWM.h"

epic::index::SingleValueWM::SingleValueWM(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: DeeganPackel(g, approx, int_representation) {
}

std::vector<epic::bigFloat> epic::index::SingleValueWM::calculate() {
	bigInt total_mwc;
	numberOfMinimalWinningCoalitions(&total_mwc);

	return {static_cast<bigFloat>(total_mwc)};
}

std::string epic::index::SingleValueWM::getFullName() {
	return "Number of minimal winning coalitions";
}
