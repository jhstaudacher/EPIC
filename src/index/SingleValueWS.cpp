#include "index/SingleValueWS.h"

#include "lint/GlobalCalculator.h"

epic::index::SingleValueWS::SingleValueWS(Game* g) : RawFelsenthal(g) {}

std::vector<epic::bigFloat> epic::index::SingleValueWS::calculate(Game* g) {
	// mwcs: number of minimal winning coalitions for each player i
	auto mwcs = new lint::LargeNumber[g->getNumberOfNonZeroPlayers()];
	gCalculator->allocInit_largeNumberArray(mwcs, g->getNumberOfNonZeroPlayers());
	minimal_winning_coalitions_of_least_size(g, mwcs);

	// sum_mwcs: number of all minimal winning coalitions
	lint::LargeNumber sum_mwcs;
	gCalculator->allocInit_largeNumber(sum_mwcs);

	for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
		gCalculator->plusEqual(sum_mwcs, mwcs[i]);
	}

	gCalculator->free_largeNumberArray(mwcs);
	delete[] mwcs;

	std::vector<bigFloat> solution(1);
	bigInt big_sum_mwcs;
	gCalculator->to_bigInt(&big_sum_mwcs, sum_mwcs);

	gCalculator->free_largeNumber(sum_mwcs);

	return {static_cast<bigFloat>(big_sum_mwcs)};
}

std::string epic::index::SingleValueWS::getFullName() {
	return "Number of minimal winning coalitions of smallest cardinality";
}

epic::longUInt epic::index::SingleValueWS::getMemoryRequirement(Game* g) {
	bigInt memory = gCalculator->getLargeNumberSize(); // sum_mwcs
	memory /= cMemUnit_factor;

	memory += RawFelsenthal::getMemoryRequirement(g);

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}
