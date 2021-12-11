#include "index/Rae.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

epic::index::Rae::Rae(Game& g) : RawBanzhaf() {
	if (g.getNumberOfNullPlayers() > 0 && g.getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::invalidFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::Rae::calculate(Game& g) {
	// n_sp[x]: number of times player x is a swing player
	auto n_sp = new lint::LargeNumber[g.getNumberOfNonZeroPlayers()];
	gCalculator->allocInit_largeNumberArray(n_sp, g.getNumberOfNonZeroPlayers());
	numberOfTimesPlayerIsSwingPlayer(g, n_sp);

	std::vector<bigFloat> solution(g.getNumberOfPlayers());
	{
		// number of different coalitions = 2^n
		bigFloat n_combinations = bigInt(1) << g.getNumberOfNonZeroPlayers();
		bigInt sp;
		bigFloat gamma = 0.5;

		if (log::out.getLogLevel() <= log::info) {
			bigInt factor = bigInt(1) << g.getNumberOfPlayersWithWeight0();

			log::out << log::info << "Number of times each player is a swing player:" << log::endl;

			for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) {
				gCalculator->to_bigInt(&sp, n_sp[i]);
				solution[i] = (sp / n_combinations) + gamma;
				log::out << "Player " << g.playerIndexToNumber(i) << ": " << sp * factor << log::endl;
			}
			for (longUInt i = g.getNumberOfNonZeroPlayers(); i < g.getNumberOfPlayers(); ++i) {
				solution[i] = 0.5;
				log::out << "Player " << g.playerIndexToNumber(i) << ": 0" << log::endl;
			}
		} else {
			for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) {
				gCalculator->to_bigInt(&sp, n_sp[i]);
				solution[i] = (sp / n_combinations) + gamma;
			}
			for (longUInt i = g.getNumberOfNonZeroPlayers(); i < g.getNumberOfPlayers(); ++i) {
				solution[i] = 0.5;
			}
		}
	}

	gCalculator->free_largeNumberArray(n_sp);
	delete[] n_sp;

	return solution;
}

std::string epic::index::Rae::getFullName() {
	return "Rae";
}
