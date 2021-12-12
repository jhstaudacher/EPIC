#include "index/AbsolutePowerIndexG.h"

#include "Array.h"
#include "Logging.h"
#include "lint/GlobalCalculator.h"

#include <iostream>

epic::index::AbsolutePowerIndexG::AbsolutePowerIndexG(Game* g) : RawBanzhaf() {
	if (g->getNumberOfNullPlayers() > 0 && !g->getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::missingFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::AbsolutePowerIndexG::calculate(Game* g) {
	// n_sp[x]: number of times player x is a swing player.
	auto n_sp = new lint::LargeNumber[g->getNumberOfPlayers()];
	gCalculator->allocInit_largeNumberArray(n_sp, g->getNumberOfPlayers());

	numberOfTimesPlayerIsSwingPlayer(g, n_sp);

	// n_wc[x]: number of winning coalitions of weight x.
	ArrayOffset<lint::LargeNumber> n_wc(g->getWeightSum() + 1, g->getQuota());
	gCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());

	numberOfWinningCoalitionsPerWeight(g, n_wc);

	// total_wc: the total number of winning coalitions.
	lint::LargeNumber total_wc;
	gCalculator->allocInit_largeNumber(total_wc);
	numberOfWinningCoalitions(g, n_wc, total_wc);

	bigInt tmp;
	gCalculator->to_bigInt(&tmp, total_wc);
	bigFloat float_total_wc = tmp;

	// delete n_wc[]
	gCalculator->free_largeNumberArray(n_wc.getArrayPointer());

	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	for (longUInt i = 0; i < g->getNumberOfPlayers(); ++i) {
		gCalculator->to_bigInt(&tmp, n_sp[i]);
		solution[i] = (double)0.5 * (tmp + float_total_wc);
		solution[i] /= float_total_wc;
	}

	gCalculator->free_largeNumber(total_wc);
	delete[] n_sp;
	return solution;
}

std::string epic::index::AbsolutePowerIndexG::getFullName() {
	return "AbsolutePowerIndexG";
}
