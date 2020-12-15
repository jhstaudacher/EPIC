#include "index/PublicHelpThetaPH.h"

#include "Logging.h"

epic::index::PublicHelpThetaPH::PublicHelpThetaPH(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawPublicHelpTheta(g, approx, int_representation) {
	if (mGame.getNumberOfNullPlayers() > 0 && mGame.getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::invalidFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::PublicHelpThetaPH::calculate() {
	auto big_wci = new bigInt[mGame.getNumberOfPlayers()];

	winningCoalitionsForPlayer(big_wci);

	// big_swc: sum of every player's number of winning coalitions represented as bigInt per shapley dubey identity
	bigInt big_swc = 0;

	for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
		big_swc += big_wci[i];
	}

	bigFloat float_swc = big_swc;

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		if (log::out.getLogLevel() <= log::info) {
			bigInt factor = bigInt(1) << mGame.getNumberOfPlayersWithWeight0(); // additional winning coalitions due to players of weight 0

			log::out << log::info << "Number of winning coalitions a player belongs to: " << log::endl;

			for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
				solution[i] = big_wci[i] / float_swc;
				log::out << "Player " << mGame.playerIndexToNumber(i) << ": " << big_wci[i] * factor << log::endl;
			}

		} else {
			for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
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
