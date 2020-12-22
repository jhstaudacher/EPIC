#include "index/NevisonPH.h"

#include "Logging.h"

epic::index::NevisonPH::NevisonPH(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawPublicHelpTheta(g, approx, int_representation) {
	if (mGame.getNumberOfNullPlayers() > 0 && mGame.getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::invalidFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::NevisonPH::calculate() {
	// big_wci[x]: same as wci but represented as bigInt
	auto big_wci = new bigInt[mGame.getNumberOfPlayers()];

	winningCoalitionsForPlayer(big_wci);

	log::out << log::info << "Number of winning coalitions a player belongs to: " << log::endl;

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		bigInt factor = bigInt(1) << mGame.getNumberOfPlayersWithWeight0(); // additional winning coalitions due to players of weight 0
		bigInt tmp = bigInt(1) << mGame.getNumberOfPlayers() - 1;
		bigFloat combinations = tmp;

		for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
			log::out << "Player " << mGame.playerIndexToNumber(i) << ": " << big_wci[i] * factor << log::endl;
			solution[i] = (big_wci[i] * factor) / combinations;
		}
	}

	delete[] big_wci;

	return solution;
}

std::string epic::index::NevisonPH::getFullName() {
	return "Nevison (using RawPublicHelpTheta)";
}
