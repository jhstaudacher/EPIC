#include "index/DeeganPackel.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

epic::index::DeeganPackel::DeeganPackel() : RawDeeganPackel() {}

std::vector<epic::bigFloat> epic::index::DeeganPackel::calculate(Game* g) {
	// total_mwc: number of minimal winning coalitions.
	bigInt total_mwc;
	numberOfMinimalWinningCoalitions(g, &total_mwc);
	if (total_mwc <= 0) {
		throw std::invalid_argument("Number of minimal winning coalitions is less than or equal to zero. No calculation possible. Please, check your input.");
	}
	log::out << log::info << "Total number of minimal winning coalitions: " << total_mwc << log::endl;

	std::vector<bigFloat> solution = RawDeeganPackel::calculate(g);

	for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
		solution[i] /= total_mwc;
	}

	return solution;
}

std::string epic::index::DeeganPackel::getFullName() {
	return "DeeganPackel";
}

void epic::index::DeeganPackel::numberOfMinimalWinningCoalitions(Game* g, bigInt* total_mwc) {
	// wc[x]: winning coalitions with weight x
	auto wc = new lint::LargeNumber[g->getQuota() + 1];
	gCalculator->allocInit_largeNumberArray(wc, g->getQuota() + 1);

	// Initialise the empty coalition with 1, since we know, that the empty coalition is always losing
	gCalculator->assign_one(wc[0]);

	longUInt w_first = g->getWeights()[0];
	for (longUInt x = g->getQuota() - w_first; x <= g->getQuota(); --x) {
		gCalculator->plusEqual(wc[x + w_first], wc[x]);
	}

	// sum_minwc[i]: sum of how many times player i is smallest player in a minimum winning coalition
	auto sum_minwc = new lint::LargeNumber[g->getNumberOfNonZeroPlayers()];
	gCalculator->allocInit_largeNumberArray(sum_minwc, g->getNumberOfNonZeroPlayers());

	// only look at players of weight > 0
	for (longUInt i = 1; i < g->getNumberOfNonZeroPlayers(); ++i) {
		longUInt wi = g->getWeights()[i];

		if (wi < g->getQuota()) {
			for (longUInt x = g->getQuota() - wi; x <= g->getQuota() - 1; ++x) {
				gCalculator->plusEqual(sum_minwc[i], wc[x]);
			}
		}

		for (longInt x = g->getQuota() - wi + 1; x > 0; --x) {
			gCalculator->plusEqual(wc[x + wi - 1], wc[x - 1]);
		}
	}

	//if one of the player has a weight >= quota
	for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers() && g->getWeights()[i] >= g->getQuota(); ++i) {
		gCalculator->increment(sum_minwc[i]);
	}

	// sum up the minimal winning coalitions
	bigInt big_tmp; // number of minimal winning coalitions of player i
	for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
		big_tmp = 0;
		gCalculator->to_bigInt(&big_tmp, sum_minwc[i]);
		*total_mwc += big_tmp;
	}

	/*
	 * DELETE
	 */
	gCalculator->free_largeNumberArray(wc);
	delete[] wc;

	gCalculator->free_largeNumberArray(sum_minwc);
	delete[] sum_minwc;
}
