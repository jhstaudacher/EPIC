#include "index/Banzhaf.h"

#include "Array.h"
#include "Logging.h"
#include "lint/GlobalCalculator.h"

epic::index::Banzhaf::Banzhaf() : RawBanzhaf() {}

std::vector<epic::bigFloat> epic::index::Banzhaf::calculate(Game* g) {
	// n_sp[x]: number of times player x is a swing player
	auto n_sp = new lint::LargeNumber[g->getNumberOfNonZeroPlayers()];
	gCalculator->allocInit_largeNumberArray(n_sp, g->getNumberOfNonZeroPlayers());
	numberOfTimesPlayerIsSwingPlayer(g, n_sp);

	// total_n_sp: The sum of all n_sp's
	lint::LargeNumber total_n_sp;
	gCalculator->allocInit_largeNumber(total_n_sp);
	numberOfSwingPlayer(g, n_sp, total_n_sp);

	// solution[x]: the relative banzhaf index for player x
	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	{
		bigInt tmp;

		gCalculator->to_bigInt(&tmp, total_n_sp);
		bigFloat float_total_n_sp = tmp;

		if (log::out.getLogLevel() <= log::info) {
			bigInt factor = bigInt(1) << g->getNumberOfPlayersWithWeight0(); // additional winning coalitions due to players of weight 0

			log::out << log::info << "Total number of swings: " << float_total_n_sp * factor << log::endl;
			log::out << log::info << "Raw Banzhaf:" << log::endl;

			for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
				gCalculator->to_bigInt(&tmp, n_sp[i]);
				log::out << "Player " << g->playerIndexToNumber(i) << ": " << tmp * factor << log::endl;
				solution[i] = tmp / float_total_n_sp;
			}
			for (longUInt i = g->getNumberOfNonZeroPlayers(); i < g->getNumberOfPlayers(); ++i) {
				log::out << "Player " << g->playerIndexToNumber(i) << ": 0" << log::endl;
				// solution[i] = 0; (already zero-initialized)
			}
		} else {
			for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
				gCalculator->to_bigInt(&tmp, n_sp[i]);
				solution[i] = tmp / float_total_n_sp;
			}
			// for (longUInt i = g->getNumberOfNonZeroPlayers(); i < g->getNumberOfPlayers(); ++i) solution[i] = 0; (already zero-initialized)
		}
	}

	/*
	 * DELETE
	 */
	gCalculator->free_largeNumber(total_n_sp);
	gCalculator->free_largeNumberArray(n_sp);
	delete[] n_sp;

	return solution;
}

std::string epic::index::Banzhaf::getFullName() {
	return "Banzhaf";
}
