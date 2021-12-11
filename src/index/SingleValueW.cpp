#include "index/SingleValueW.h"

#include "lint/GlobalCalculator.h"

epic::index::SingleValueW::SingleValueW() : RawBanzhaf() {}

std::vector<epic::bigFloat> epic::index::SingleValueW::calculate(Game& g) {
	// n_wc[x]: number of winning coalitions of weight x.
	ArrayOffset<lint::LargeNumber> n_wc(g.getWeightSum() + 1, g.getQuota());
	gCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeight(g, n_wc);

	// total_wc: the number of winning coalitions
	lint::LargeNumber total_wc;
	gCalculator->allocInit_largeNumber(total_wc);
	numberOfWinningCoalitions(g, n_wc, total_wc);

	gCalculator->free_largeNumberArray(n_wc.getArrayPointer());

	std::vector<bigFloat> solution(1);
	{
		bigInt big_total_wc;
		gCalculator->to_bigInt(&big_total_wc, total_wc);

		bigInt factor = bigInt(1) << g.getNumberOfPlayersWithWeight0();
		solution[0] = big_total_wc * factor;
	}

	gCalculator->free_largeNumber(total_wc);

	return solution;
}

std::string epic::index::SingleValueW::getFullName() {
	return "Number of winning coalitions";
}
