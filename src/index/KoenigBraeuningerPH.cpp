#include "index/KoenigBraeuningerPH.h"

#include "Logging.h"

epic::index::KoenigBraeuningerPH::KoenigBraeuningerPH(Game* g) : RawPublicHelpTheta(g) {}

std::vector<epic::bigFloat> epic::index::KoenigBraeuningerPH::calculate(Game* g) {
	// big_wci[x]: number of winning coalitions for player x represented as bigInt
	auto big_wci = new bigInt[g->getNumberOfPlayers()];

	// big_total_wc: the total number of winning coalitions represented as bigInt
	bigFloat big_total_wc;

	winningCoalitionsForPlayer(g, big_wci, &big_total_wc);

	log::out << log::info << "Total number of winning coalitions: " << big_total_wc << log::endl;
	log::out << log::info << "Number of winning coalitions a player belongs to: " << log::endl;

	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	for (longUInt i = 0; i < g->getNumberOfPlayers(); ++i) {
		log::out << "Player " << g->playerIndexToNumber(i) << ": " << big_wci[i] << log::endl;
		solution[i] = big_wci[i] / big_total_wc;
	}

	delete[] big_wci;

	return solution;
}

std::string epic::index::KoenigBraeuningerPH::getFullName() {
	return "KoenigBraeuninger (using RawPublicHelpTheta)";
}
