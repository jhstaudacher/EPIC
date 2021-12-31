#include "index/KoenigBraeuninger.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

#include <iostream>

epic::index::KoenigBraeuninger::KoenigBraeuninger(Game* g)
	: RawBanzhaf() {
	if (g->getNumberOfNullPlayers() > 0 && g->getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::invalidFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::KoenigBraeuninger::calculate(Game* g) {
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
		bigInt tmp;

		gCalculator->to_bigInt(&tmp, total_wc);
		bigFloat float_total_wc = tmp;

		for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
			gCalculator->to_bigInt(&tmp, n_sp[i]);

			solution[i] = (tmp + float_total_wc);
			solution[i] /= float_total_wc;
			solution[i] *= (double)0.5;
		}
		for (longUInt i = g->getNumberOfNonZeroPlayers(); i < g->getNumberOfPlayers(); ++i) {
			solution[i] = 0.5;
		}
	}

	gCalculator->free_largeNumber(total_wc);
	// delete n_sp[]
	gCalculator->free_largeNumberArray(n_sp);
	delete[] n_sp;
	return solution;
}

std::string epic::index::KoenigBraeuninger::getFullName() {
	return "KoenigBraeuninger (with Dubey-Shapley identity)";
}
