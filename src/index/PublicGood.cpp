#include "index/PublicGood.h"

#include "Array.h"

#include <iostream>

epic::index::PublicGood::PublicGood(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawPublicGood(g, approx, int_representation) {
}

std::vector<epic::bigFloat> epic::index::PublicGood::calculate() {
	auto mwc = new lint::LargeNumber[mNonZeroPlayerCount];
	mCalculator->allocInit_largeNumberArray(mwc, mNonZeroPlayerCount);

	calculateMinimalWinningCoalitionsPerPlayer(mwc);

	bigFloat big_mwc_sum;
	{
		lint::LargeNumber mwc_sum;
		mCalculator->allocInit_largeNumber(mwc_sum);

		for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
			mCalculator->plusEqual(mwc_sum, mwc[i]);
		}

		bigInt tmp;
		mCalculator->to_bigInt(&tmp, mwc_sum);
		big_mwc_sum = tmp;

		mCalculator->free_largeNumber(mwc_sum);
	}

	if (mGame.getFlagOfVerbose()) {
		std::cout << "Total number of minimal winning coalitions: " << big_mwc_sum << std::endl
				  << std::endl;
		std::cout << "Number of winning coalitions individual players belong to: " << std::endl;
	}

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		bigInt big_mwc;

		for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
			mCalculator->to_bigInt(&big_mwc, mwc[i]);
			solution[i] = big_mwc;
			if (mGame.getFlagOfVerbose()) {
				std::cout << "Player " << mGame.playerIndexToNumber(i) << ": " << big_mwc << std::endl;
			}
			solution[i] /= big_mwc_sum;
		}
		for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
			solution[i] = 0;
			if (mGame.getFlagOfVerbose()) {
				std::cout << "Player " << mGame.playerIndexToNumber(i) << ": " << 0 << std::endl;
			}
		}
	}

	mCalculator->free_largeNumberArray(mwc);
	delete[] mwc;

	return solution;
}

std::string epic::index::PublicGood::getFullName() {
	return "PublicGood";
}
