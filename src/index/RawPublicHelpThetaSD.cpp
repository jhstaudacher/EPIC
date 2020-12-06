#include "index/RawPublicHelpThetaSD.h"

#include "Logging.h"

epic::index::RawPublicHelpThetaSD::RawPublicHelpThetaSD(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawBanzhaf(g, approx, int_representation) {
	if (mGame.getNumberOfNullPlayers() > 0 && mGame.getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::invalidFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::RawPublicHelpThetaSD::calculate() {
	auto big_wci = new bigInt[mGame.getNumberOfPlayers()];

	winningCoalitionsForPlayer(big_wci);

	bigInt factor = bigInt(1) << mGame.getNumberOfPlayersWithWeight0(); // additional winning coalitions due to players of weight 0
	std::vector<epic::bigFloat> solution(mGame.getNumberOfPlayers());
	for (longUInt i = 0; i < mGame.getNumberOfPlayers(); i++) {
		solution[i] = big_wci[i] * factor;
	}

	delete[] big_wci;
	return solution;
}

std::string epic::index::RawPublicHelpThetaSD::getFullName() {
	return "RawPublicHelpTheta (with Dubey-Shapley identity)";
}

void epic::index::RawPublicHelpThetaSD::winningCoalitionsForPlayer(bigInt big_wci[]) {
	// n_sp[x]: number of times player x is a swing player.
	auto n_sp = new lint::LargeNumber[mGame.getNumberOfPlayers()];
	mCalculator->allocInit_largeNumberArray(n_sp, mGame.getNumberOfPlayers());

	numberOfTimesPlayerIsSwingPlayer(n_sp);

	// n_wc[x]: number of winning coalitions of weight x.
	ArrayOffset<lint::LargeNumber> n_wc(mGame.getWeightSum() + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());

	numberOfWinningCoalitionsPerWeight(n_wc);

	// total_wc: the total number of winning coalitions.
	lint::LargeNumber total_wc;
	mCalculator->allocInit_largeNumber(total_wc);

	numberOfWinningCoalitions(n_wc, total_wc);

	// delete n_wc[]
	mCalculator->free_largeNumberArray(n_wc.getArrayPointer());

	bigInt tmp;

	mCalculator->to_bigInt(&tmp, total_wc);
	bigFloat float_total_wc = tmp;

	for (longUInt i = 0; i < mGame.getNumberOfPlayers(); i++) {
		mCalculator->to_bigInt(&tmp, n_sp[i]);

		big_wci[i] = double(0.5) * (tmp + float_total_wc);
	}

	mCalculator->free_largeNumber(total_wc);
	// delete n_sp[]
	mCalculator->free_largeNumberArray(n_sp);
	delete[] n_sp;
}
