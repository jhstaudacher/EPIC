#include "index/KoenigBraeuninger.h"

#include "Logging.h"

#include <iostream>

epic::index::KoenigBraeuninger::KoenigBraeuninger(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawBanzhaf(g, approx, int_representation) {
	if (mGame.getNumberOfNullPlayers() > 0 && mGame.getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::invalidFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::KoenigBraeuninger::calculate() {
	// n_sp[x]: number of times player x is a swing player.
	auto n_sp = new lint::LargeNumber[mNonZeroPlayerCount];
	mCalculator->allocInit_largeNumberArray(n_sp, mNonZeroPlayerCount);

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

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		bigInt tmp;

		mCalculator->to_bigInt(&tmp, total_wc);
		bigFloat float_total_wc = tmp;

		for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
			mCalculator->to_bigInt(&tmp, n_sp[i]);

			solution[i] = (tmp + float_total_wc);
			solution[i] /= float_total_wc;
			solution[i] *= (double)0.5;
		}
		for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
			solution[i] = 0.5;
		}
	}

	mCalculator->free_largeNumber(total_wc);
	// delete n_sp[]
	mCalculator->free_largeNumberArray(n_sp);
	delete[] n_sp;
	return solution;
}

std::string epic::index::KoenigBraeuninger::getFullName() {
	return "KoenigBraeuninger (with Dubey-Shapley identity)";
}
