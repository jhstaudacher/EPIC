#include "index/PublicGood.h"

#include "lint/GlobalCalculator.h"
#include "Array.h"
#include "Logging.h"

std::vector<epic::bigFloat> epic::index::PublicGood::calculate(Game* g) {
	std::vector<bigFloat> solution;
	calculate(g, solution);
	return solution;
}

void epic::index::PublicGood::calculate(Game* g, std::vector<bigFloat>& solution) {
	auto mwc = new lint::LargeNumber[g->getNumberOfNonZeroPlayers()];
	gCalculator->allocInit_largeNumberArray(mwc, g->getNumberOfNonZeroPlayers());

	calculateMinimalWinningCoalitionsPerPlayer(g, mwc);

	bigFloat big_mwc_sum;
	{
		lint::LargeNumber mwc_sum;
		gCalculator->allocInit_largeNumber(mwc_sum);

		for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
			gCalculator->plusEqual(mwc_sum, mwc[i]);
		}

		bigInt tmp;
		gCalculator->to_bigInt(&tmp, mwc_sum);
		big_mwc_sum = tmp;

		gCalculator->free_largeNumber(mwc_sum);
	}

	log::out << log::info << "Number of minimal winning coalitions individual players belong to: " << log::endl;

	solution.reserve(g->getNumberOfPlayers());
	bigInt big_mwc;

	for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
		gCalculator->to_bigInt(&big_mwc, mwc[i]);
		solution[i] = big_mwc;
		log::out << "Player " << g->playerIndexToNumber(i) << ": " << big_mwc << log::endl;
		solution[i] /= big_mwc_sum;
	}
	for (longUInt i = g->getNumberOfNonZeroPlayers(); i < g->getNumberOfPlayers(); ++i) {
		solution[i] = 0;
		log::out << "Player " << g->playerIndexToNumber(i) << ": 0" << log::endl;
	}

	gCalculator->free_largeNumberArray(mwc);
	delete[] mwc;
}

std::string epic::index::PublicGood::getFullName() {
	return "PublicGood";
}
