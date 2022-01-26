#include "index/AbsoluteBanzhaf.h"

#include "Array.h"
#include "Logging.h"
#include "lint/GlobalCalculator.h"

epic::index::AbsoluteBanzhaf::AbsoluteBanzhaf()
	: RawBanzhaf() {}

std::vector<epic::bigFloat> epic::index::AbsoluteBanzhaf::calculate(Game* g) {
	// n_sp[x]: number of times player x is a swing player
	auto n_sp = new lint::LargeNumber[g->getNumberOfNonZeroPlayers()];
	gCalculator->allocInit_largeNumberArray(n_sp, g->getNumberOfNonZeroPlayers());
	numberOfTimesPlayerIsSwingPlayer(g, n_sp);

	// solution[x]: the absolute Banzhaf index for player x
	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	{
		// 2^(n-1)
		bigInt tmp = bigInt(1) << g->getNumberOfNonZeroPlayers() - 1;
		bigFloat combinations = tmp;

		if (log::out.getLogLevel() <= log::info) {
			log::out << log::info << "Number of times each player is a swing player:" << log::endl;

			bigInt factor = bigInt(1) << g->getNumberOfPlayersWithWeight0(); // additional winning coalitions due to players of weight 0
			for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
				gCalculator->to_bigInt(&tmp, n_sp[i]);
				log::out << "Player " << g->playerIndexToNumber(i) << ": " << tmp * factor << log::endl;
				solution[i] = tmp / combinations;
			}
			for (longUInt i = g->getNumberOfNonZeroPlayers(); i < g->getNumberOfPlayers(); ++i) {
				log::out << "player " << g->playerIndexToNumber(i) << ": 0" << log::endl;
				// solution[i] = 0; (already zero-initialized)
			}
		} else {
			for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
				gCalculator->to_bigInt(&tmp, n_sp[i]);
				solution[i] = tmp / combinations;
			}
			// for (longUInt i =g->getNumberOfNonZeroPlayers(); i < g->getNumberOfPlayers(); ++i) solution[i] = 0; (already zero-initialized)
		}
	}

	/*
	 * DELETE
	 */
	gCalculator->free_largeNumberArray(n_sp);
	delete[] n_sp;

	return solution;
}

std::string epic::index::AbsoluteBanzhaf::getFullName() {
	return "AbsoluteBanzhaf";
}
