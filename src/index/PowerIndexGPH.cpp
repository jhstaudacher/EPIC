#include "index/PowerIndexGPH.h"

#include "Array.h"
#include "Logging.h"

epic::index::PowerIndexGPH::PowerIndexGPH(Game* g)
	: RawPublicHelpTheta(g) {
	if (g->getNumberOfNullPlayers() > 0 && !g->getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::missingFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::PowerIndexGPH::calculate(Game* g) {
	auto wci = new bigInt[g->getNumberOfPlayers()];
	bigFloat total_wc;

	winningCoalitionsForPlayer(g, wci, &total_wc);

	// big_swc: sum of every player's number of winning coalitions represented as bigInt
	bigInt big_swc = 0;

	for (longUInt i = 0; i < g->getNumberOfPlayers(); ++i) {
		big_swc += wci[i];
	}

	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	bigFloat temp;
	bigFloat float_swc = big_swc;

	log::out << log::info << "Number of winning coalitions a player belongs to: " << log::endl;

	for (longUInt i = 0; i < g->getNumberOfPlayers(); ++i) {
		solution[i] = wci[i];
		log::out << "Player " << g->playerIndexToNumber(i) << ": " << wci[i] << log::endl;
		solution[i] /= float_swc;
	}

	delete[] wci;
	return solution;
}

std::string epic::index::PowerIndexGPH::getFullName() {
	return "PowerIndexG (using RawPublicHelpTheta)";
}

epic::longUInt epic::index::PowerIndexGPH::getMemoryRequirement(Game* g) {
	bigInt memory = 0;

	bigInt max_n_wci = 2;
	mpz_pow_ui(max_n_wci.get_mpz_t(), max_n_wci.get_mpz_t(), g->getNumberOfPlayers());
	memory += g->getNumberOfPlayers() * GMPHelper::size_of_int(max_n_wci); // big_wci (all players are in the maximum of winning coalitions 2^n)
	memory += GMPHelper::size_of_int(max_n_wci * g->getNumberOfPlayers()); // big_swc (sum of all big_wci)
	// exclude solutions vector since the PowerIndexWithWinningCoalitionsPerPlayer function already adds the wci array which is (numberOfPlayer * numberOfRemainder large)
	memory /= cMemUnit_factor;

	memory += RawPublicHelpTheta::getMemoryRequirement(g);

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}
	return ret;
}
