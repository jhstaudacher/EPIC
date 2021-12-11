#include "index/PublicHelpThetaPH.h"

#include "Logging.h"

epic::index::PublicHelpThetaPH::PublicHelpThetaPH(Game& g) : RawPublicHelpTheta(g) {}

std::vector<epic::bigFloat> epic::index::PublicHelpThetaPH::calculate(Game& g) {
	auto big_wci = new bigInt[g.getNumberOfPlayers()];

	winningCoalitionsForPlayer(g, big_wci);

	// big_swc: sum of every player's number of winning coalitions represented as bigInt per shapley dubey identity
	bigInt big_swc = 0;

	for (longUInt i = 0; i < g.getNumberOfPlayers(); ++i) {
		big_swc += big_wci[i];
	}

	bigFloat float_swc = big_swc;

	std::vector<bigFloat> solution(g.getNumberOfPlayers());
	{
		if (log::out.getLogLevel() <= log::info) {
			bigInt factor = bigInt(1) << g.getNumberOfPlayersWithWeight0(); // additional winning coalitions due to players of weight 0

			log::out << log::info << "Number of winning coalitions a player belongs to: " << log::endl;

			for (longUInt i = 0; i < g.getNumberOfPlayers(); ++i) {
				solution[i] = big_wci[i] / float_swc;
				log::out << "Player " << g.playerIndexToNumber(i) << ": " << big_wci[i] * factor << log::endl;
			}

		} else {
			for (longUInt i = 0; i < g.getNumberOfPlayers(); ++i) {
				solution[i] = big_wci[i] / float_swc;
			}
		}
	}

	delete[] big_wci;
	return solution;
}

std::string epic::index::PublicHelpThetaPH::getFullName() {
	return "PublicHelpTheta (using RawPublicHelpTheta)";
}
