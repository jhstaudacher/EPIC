#include "index/Johnston.h"

#include "Logging.h"

epic::index::Johnston::Johnston(Game& g) : RawJohnston(g) {}

std::vector<epic::bigFloat> epic::index::Johnston::calculate(Game& g) {
	std::vector<bigFloat> solution = RawJohnston::calculate(g);

	bigFloat raw_j_sum = 0;
	for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) { // sum over all player with weight greater than zero (rawJohnston-value greater than zero)
		raw_j_sum += solution[i];
	}

	if (log::out.getLogLevel() <= log::info) {
		log::out << log::info << "Raw Johnston: " << log::endl;
		for (longUInt i = 0; i < g.getNumberOfPlayers(); ++i) {
			log::out << "Player " << g.playerIndexToNumber(i) << ": " << solution[i] << log::endl;
		}
	}

	for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) {
		solution[i] /= raw_j_sum;
	}

	return solution;
}

std::string epic::index::Johnston::getFullName() {
	return "Johnston";
}
