#include "index/SingleValueW.h"

epic::index::SingleValueW::SingleValueW(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawBanzhaf(g, approx, int_representation) {
}

std::vector<epic::bigFloat> epic::index::SingleValueW::calculate() {
	// n_wc[x]: number of winning coalitions of weight x.
	ArrayOffset<lint::LargeNumber> n_wc(mGame.getWeightSum() + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeight(n_wc);

	// total_wc: the number of winning coalitions
	lint::LargeNumber total_wc;
	mCalculator->allocInit_largeNumber(total_wc);
	numberOfWinningCoalitions(n_wc, total_wc);

	mCalculator->free_largeNumberArray(n_wc.getArrayPointer());

	std::vector<bigFloat> solution(1);
	{
		bigInt big_total_wc;
		mCalculator->to_bigInt(&big_total_wc, total_wc);

		bigInt factor = bigInt(1) << mGame.getNumberOfPlayersWithWeight0();
		solution[0] = big_total_wc * factor;
	}

	mCalculator->free_largeNumber(total_wc);

	return solution;
}

std::string epic::index::SingleValueW::getFullName() {
	return "Number of winning coalitions";
}
