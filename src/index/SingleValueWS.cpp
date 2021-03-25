#include "index/SingleValueWS.h"

epic::index::SingleValueWS::SingleValueWS(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawFelsenthal(g, approx, int_representation) {
}

std::vector<epic::bigFloat> epic::index::SingleValueWS::calculate() {
	// mwcs: number of minimal winning coalitions for each player i
	auto mwcs = new lint::LargeNumber[mNonZeroPlayerCount];
	mCalculator->allocInit_largeNumberArray(mwcs, mNonZeroPlayerCount);
	minimal_winning_coalitions_of_least_size(mwcs);

	// sum_mwcs: number of all minimal winning coalitions
	lint::LargeNumber sum_mwcs;
	mCalculator->allocInit_largeNumber(sum_mwcs);

	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
		mCalculator->plusEqual(sum_mwcs, mwcs[i]);
	}

	mCalculator->free_largeNumberArray(mwcs);
	delete[] mwcs;

	std::vector<bigFloat> solution(1);
	bigInt big_sum_mwcs;
	mCalculator->to_bigInt(&big_sum_mwcs, sum_mwcs);

	mCalculator->free_largeNumber(sum_mwcs);

	return {static_cast<bigFloat>(big_sum_mwcs)};
}

std::string epic::index::SingleValueWS::getFullName() {
	return "Number of minimal winning coalitions of smallest cardinality";
}

epic::longUInt epic::index::SingleValueWS::getMemoryRequirement() {
	bigInt memory = RawFelsenthal::getMemoryRequirement();
	memory += mCalculator->getLargeNumberSize(); // sum_mwcs

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}
