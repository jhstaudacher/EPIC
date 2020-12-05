#include "index/PowerIndexG.h"

#include "Array.h"
#include "Logging.h"

#include <iostream>

epic::index::PowerIndexG::PowerIndexG(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawPowerIndexG(g, approx, int_representation) {
}

std::vector<epic::bigFloat> epic::index::PowerIndexG::calculate() {
	auto solution = RawPowerIndexG::calculate();

	bigFloat sum_swc = 0;
	for (const auto& it : solution) {
		sum_swc += it;
	}

	if (mGame.getFlagOfVerbose()) {
		std::cout << "Total number of winning coalitions: " << sum_swc << std::endl
				  << std::endl;
		std::cout << "Number of winning coalitions a player belongs to: " << std::endl;
		for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
			std::cout << "Player " << mGame.playerIndexToNumber(i) << ": " << solution[i] << std::endl;
		}
	}

	for (auto& it : solution) {
		it /= sum_swc;
	}

	return solution;
}

std::string epic::index::PowerIndexG::getFullName() {
	return "PowerIndexG";
}
