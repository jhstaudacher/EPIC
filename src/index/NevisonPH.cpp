#include "index/NevisonPH.h"

#include "Logging.h"

epic::index::NevisonPH::NevisonPH(Game* g) : RawPublicHelpTheta(g) {}

std::vector<epic::bigFloat> epic::index::NevisonPH::calculate(Game* g) {
	// big_wci[x]: same as wci but represented as bigInt
	auto big_wci = new bigInt[g->getNumberOfPlayers()];

	winningCoalitionsForPlayer(g, big_wci);

	log::out << log::info << "Number of winning coalitions a player belongs to: " << log::endl;

	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	{
		bigInt factor = bigInt(1) << g->getNumberOfPlayersWithWeight0(); // additional winning coalitions due to players of weight 0
		bigInt tmp = bigInt(1) << g->getNumberOfPlayers() - 1;
		bigFloat combinations = tmp;

		for (longUInt i = 0; i < g->getNumberOfPlayers(); ++i) {
			log::out << "Player " << g->playerIndexToNumber(i) << ": " << big_wci[i] * factor << log::endl;
			solution[i] = (big_wci[i] * factor) / combinations;
		}
	}

	delete[] big_wci;

	return solution;
}

std::string epic::index::NevisonPH::getFullName() {
	return "Nevison (using RawPublicHelpTheta)";
}
