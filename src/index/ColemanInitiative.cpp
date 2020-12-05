#include "index/ColemanInitiative.h"

#include <iostream>

epic::index::ColemanInitiative::ColemanInitiative(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawBanzhaf(g, approx, int_representation) {
}

std::vector<epic::bigFloat> epic::index::ColemanInitiative::calculate() {
	// n_wc[x]: number of winning coalitions of weight x.
	ArrayOffset<lint::LargeNumber> n_wc(mGame.getWeightSum() + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeight(n_wc);

	lint::LargeNumber total_wc;
	mCalculator->allocInit_largeNumber(total_wc);
	numberOfWinningCoalitions(n_wc, total_wc);

	// n_sp[x]: number of times player x is a swing player
	auto n_sp = new lint::LargeNumber[mNonZeroPlayerCount];
	mCalculator->allocInit_largeNumberArray(n_sp, mNonZeroPlayerCount);
	numberOfTimesPlayerIsSwingPlayer(n_wc, n_sp);

	mCalculator->free_largeNumberArray(n_wc.getArrayPointer());

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		bigInt big_total_wc;
		mCalculator->to_bigInt(&big_total_wc, total_wc);

		// div = n_combinations - big_total_wc
		bigFloat div = (bigInt(1) << mNonZeroPlayerCount) - big_total_wc;
		bigInt sp;

		if (mGame.getFlagOfVerbose()) {
			bigInt factor = bigInt(1) << mGame.getNumberOfPlayersWithWeight0();

			std::cout << "Total number of winning coalitions: " << big_total_wc * factor << std::endl
					  << std::endl;
			std::cout << "Number of times each player is a swing player:" << std::endl;

			for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
				mCalculator->to_bigInt(&sp, n_sp[i]);
				solution[i] = sp;
				solution[i] /= div;
				std::cout << "Player " << mGame.playerIndexToNumber(i) << ": " << sp * factor << std::endl;
			}
			for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
				std::cout << "Player " << mGame.playerIndexToNumber(i) << ": 0" << std::endl;
				// solution[i] = 0; (already zero-initialized)
			}
		} else {
			for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
				mCalculator->to_bigInt(&sp, n_sp[i]);
				solution[i] = sp;
				solution[i] /= div;
			}
			// for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) solution[i] = 0; (already zero-initialized)
		}
	}

	mCalculator->free_largeNumberArray(n_sp);
	delete[] n_sp;
	mCalculator->free_largeNumber(total_wc);

	return solution;
}

std::string epic::index::ColemanInitiative::getFullName() {
	return "ColemanInitiative";
}
