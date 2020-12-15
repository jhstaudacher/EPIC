#include "index/PowerIndexGPH.h"

#include "Array.h"
#include "Logging.h"

epic::index::PowerIndexGPH::PowerIndexGPH(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawPublicHelpTheta(g, approx, int_representation) {
	if (mGame.getNumberOfNullPlayers() > 0 && !mGame.getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::missingFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::PowerIndexGPH::calculate() {
	auto wci = new bigInt[mGame.getNumberOfPlayers()];
	bigFloat total_wc;

	winningCoalitionsForPlayer(wci, &total_wc);

	// big_swc: sum of every player's number of winning coalitions represented as bigInt
	bigInt big_swc = 0;

	for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
		big_swc += wci[i];
	}

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	bigFloat temp;
	bigFloat float_swc = big_swc;

	log::out << log::info << "Number of winning coalitions a player belongs to: " << log::endl;

	for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
		solution[i] = wci[i];
		log::out << "Player " << mGame.playerIndexToNumber(i) << ": " << wci[i] << log::endl;
		solution[i] /= float_swc;
	}

	delete[] wci;
	return solution;
}

std::string epic::index::PowerIndexGPH::getFullName() {
	return "PowerIndexG (using RawPublicHelpTheta)";
}

epic::longUInt epic::index::PowerIndexGPH::getMemoryRequirement() {
	bigInt memory = 0;

	memory += RawPublicHelpTheta::getMemoryRequirement();
	bigInt max_n_wci = 2;
	mpz_pow_ui(max_n_wci.get_mpz_t(), max_n_wci.get_mpz_t(), mGame.getNumberOfPlayers());
	memory += mGame.getNumberOfPlayers() * GMPHelper::size_of_int(max_n_wci); // big_wci (all players are in the maximum of winning coalitions 2^n)
	memory += GMPHelper::size_of_int(max_n_wci * mGame.getNumberOfPlayers()); // big_swc (sum of all big_wci)
	// exclude solutions vector since the PowerIndexWithWinningCoalitionsPerPlayer function already adds the wci array which is (numberOfPlayer * numberOfRemainder large)

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}
	return ret;
}
