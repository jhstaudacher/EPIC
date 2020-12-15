#include "index/Johnston.h"

#include "Logging.h"

epic::index::Johnston::Johnston(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawJohnston(g, approx, int_representation) {
}

std::vector<epic::bigFloat> epic::index::Johnston::calculate() {
	std::vector<bigFloat> solution = RawJohnston::calculate();

	bigFloat raw_j_sum = 0;
	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) { // sum over all player with weight greater than zero (rawJohnston-value greater than zero)
		raw_j_sum += solution[i];
	}

	if (mGame.getFlagOfVerbose()) {
		log::out << log::info << "Raw Johnston: " << log::endl;
		for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
			log::out << "Player " << mGame.playerIndexToNumber(i) << ": " << solution[i] << log::endl;
		}
	}

	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
		solution[i] /= raw_j_sum;
	}

	return solution;
}

std::string epic::index::Johnston::getFullName() {
	return "Johnston";
}
