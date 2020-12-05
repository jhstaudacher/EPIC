#include "index/Johnston.h"

#include <iostream>

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
		std::cout << "Raw Johnston: " << std::endl;
		for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
			std::cout << "Player " << mGame.playerIndexToNumber(i) << ": " << solution[i] << std::endl;
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
