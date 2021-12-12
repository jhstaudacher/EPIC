#include "index/Nevison.h"

#include "Array.h"
#include "Logging.h"
#include "lint/GlobalCalculator.h"

#include <iostream>

epic::index::Nevison::Nevison(Game* g) : RawBanzhaf() {
	if (g->getNumberOfNullPlayers() > 0 && g->getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::invalidFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::Nevison::calculate(Game* g) {
	// n_sp[x]: number of times player x is a swing player.
	auto n_sp = new lint::LargeNumber[g->getNumberOfNonZeroPlayers()];
	gCalculator->allocInit_largeNumberArray(n_sp, g->getNumberOfNonZeroPlayers());
	numberOfTimesPlayerIsSwingPlayer(g, n_sp);

	// n_wc[x]: number of winning coalitions of weight x.
	ArrayOffset<lint::LargeNumber> n_wc(g->getWeightSum() + 1, g->getQuota());
	gCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeight(g, n_wc);

	// total_wc: the total number of winning coalitions.
	lint::LargeNumber total_wc;
	gCalculator->allocInit_largeNumber(total_wc);
	numberOfWinningCoalitions(g, n_wc, total_wc);

	// delete n_wc[]
	gCalculator->free_largeNumberArray(n_wc.getArrayPointer());

	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	{
		/*
		 * players of weight > 0
		 */
		bigInt tmp = bigInt(1) << g->getNumberOfNonZeroPlayers() - 1;
		bigFloat combinations = tmp;

		gCalculator->to_bigInt(&tmp, total_wc);
		bigFloat float_total_wc = tmp;

		for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
			gCalculator->to_bigInt(&tmp, n_sp[i]);

			solution[i] = tmp + float_total_wc;
			solution[i] *= double(0.5);
			solution[i] /= combinations;
		}

		/*
		 * players of weight 0
		 *
		 * power index = (number of winning coalitions (including Null-Player)) / 2^n = (total_wc * 2^(number of weight 0 players)) / 2^n = total_wc / 2^(number of weight > 0 players)
		 */
		bigFloat factor = bigFloat(1) << g->getNumberOfNonZeroPlayers(); // 2^g->getNumberOfNonZeroPlayers()
		bigFloat indexWeight0 = float_total_wc / factor;
		for (longUInt i = g->getNumberOfNonZeroPlayers(); i < g->getNumberOfPlayers(); ++i) {
			solution[i] = indexWeight0;
		}
	}

	gCalculator->free_largeNumber(total_wc);
	gCalculator->free_largeNumberArray(n_sp);
	delete[] n_sp;

	return solution;
}

std::string epic::index::Nevison::getFullName() {
	return "Nevison (with Dubey-Shapley identity)";
}
