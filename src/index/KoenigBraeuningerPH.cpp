#include "index/KoenigBraeuningerPH.h"

#include "Logging.h"

#include <iostream>

epic::index::KoenigBraeuningerPH::KoenigBraeuningerPH(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawPublicHelpTheta(g, approx, int_representation) {
	if (mGame.getNumberOfNullPlayers() > 0 && mGame.getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::invalidFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::KoenigBraeuningerPH::calculate() {
	// big_wci[x]: number of winning coalitions for player x represented as bigInt
	auto big_wci = new bigInt[mGame.getNumberOfPlayers()];

	// big_total_wc: the total number of winning coalitions represented as bigInt
	bigFloat big_total_wc;

	winningCoalitionsForPlayer(big_wci, &big_total_wc);

	if (mGame.getFlagOfVerbose()) {
		std::cout << "Total number of winning coalitions: " << big_total_wc << std::endl
				  << std::endl;
		std::cout << "Number of winning coalitions a player belongs to: " << std::endl;
	}

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
		if (mGame.getFlagOfVerbose()) {
			std::cout << "Player " << mGame.playerIndexToNumber(i) << ": " << big_wci[i] << std::endl;
		}
		solution[i] = big_wci[i] / big_total_wc;
	}

	delete[] big_wci;

	return solution;
}

std::string epic::index::KoenigBraeuningerPH::getFullName() {
	return "KoenigBraeuninger PHA";
}
