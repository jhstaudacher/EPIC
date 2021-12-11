#include "index/ColemanInitiative.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

epic::index::ColemanInitiative::ColemanInitiative() : RawBanzhaf() {}

std::vector<epic::bigFloat> epic::index::ColemanInitiative::calculate(Game& g) {
	// n_wc[x]: number of winning coalitions of weight x.
	ArrayOffset<lint::LargeNumber> n_wc(g.getWeightSum() + 1, g.getQuota());
	gCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeight(g, n_wc);

	lint::LargeNumber total_wc;
	gCalculator->allocInit_largeNumber(total_wc);
	numberOfWinningCoalitions(g, n_wc, total_wc);

	// n_sp[x]: number of times player x is a swing player
	auto n_sp = new lint::LargeNumber[g.getNumberOfNonZeroPlayers()];
	gCalculator->allocInit_largeNumberArray(n_sp, g.getNumberOfNonZeroPlayers());
	numberOfTimesPlayerIsSwingPlayer(g, n_wc, n_sp);

	gCalculator->free_largeNumberArray(n_wc.getArrayPointer());

	std::vector<bigFloat> solution(g.getNumberOfPlayers());
	{
		bigInt big_total_wc;
		gCalculator->to_bigInt(&big_total_wc, total_wc);

		// div = n_combinations - big_total_wc
		bigFloat div = (bigInt(1) << g.getNumberOfNonZeroPlayers()) - big_total_wc;
		bigInt sp;

		if (log::out.getLogLevel() <= log::info) {
			bigInt factor = bigInt(1) << g.getNumberOfPlayersWithWeight0();

			log::out << log::info << "Total number of winning coalitions: " << big_total_wc * factor << log::endl;
			log::out << log::info << "Number of times each player is a swing player:" << log::endl;

			for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) {
				gCalculator->to_bigInt(&sp, n_sp[i]);
				solution[i] = sp;
				solution[i] /= div;
				log::out << "Player " << g.playerIndexToNumber(i) << ": " << sp * factor << log::endl;
			}
			for (longUInt i = g.getNumberOfNonZeroPlayers(); i < g.getNumberOfPlayers(); ++i) {
				log::out << "Player " << g.playerIndexToNumber(i) << ": 0" << log::endl;
				// solution[i] = 0; (already zero-initialized)
			}
		} else {
			for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) {
				gCalculator->to_bigInt(&sp, n_sp[i]);
				solution[i] = sp;
				solution[i] /= div;
			}
			// for (longUInt i = g.getNumberOfNonZeroPlayers(); i < g.getNumberOfPlayers(); ++i) solution[i] = 0; (already zero-initialized)
		}
	}

	gCalculator->free_largeNumberArray(n_sp);
	delete[] n_sp;
	gCalculator->free_largeNumber(total_wc);

	return solution;
}

std::string epic::index::ColemanInitiative::getFullName() {
	return "ColemanInitiative";
}
