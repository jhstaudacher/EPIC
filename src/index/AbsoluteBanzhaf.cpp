#include "index/AbsoluteBanzhaf.h"

#include "Array.h"

#include <iostream>

epic::index::AbsoluteBanzhaf::AbsoluteBanzhaf(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawBanzhaf(g, approx, int_representation) {
}

std::vector<epic::bigFloat> epic::index::AbsoluteBanzhaf::calculate() {
	// n_sp[x]: number of times player x is a swing player
	auto n_sp = new lint::LargeNumber[mNonZeroPlayerCount];
	mCalculator->allocInit_largeNumberArray(n_sp, mNonZeroPlayerCount);
	numberOfTimesPlayerIsSwingPlayer(n_sp);

	// solution[x]: the absolute Banzhaf index for player x
	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		// 2^(n-1)
		bigInt tmp = bigInt(1) << mNonZeroPlayerCount - 1;
		bigFloat combinations = tmp;

		if (mGame.getFlagOfVerbose()) {
			std::cout << "Number of times each player is a swing player:" << std::endl;

			bigInt factor = bigInt(1) << mGame.getNumberOfPlayersWithWeight0(); // additional winning coalitions due to players of weight 0
			for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
				mCalculator->to_bigInt(&tmp, n_sp[i]);
				std::cout << "Player " << mGame.playerIndexToNumber(i) << ": " << tmp * factor << std::endl;
				solution[i] = tmp / combinations;
			}
			for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
				std::cout << "player " << mGame.playerIndexToNumber(i) << ": 0" << std::endl;
				// solution[i] = 0; (already zero-initialized)
			}
		} else {
			for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
				mCalculator->to_bigInt(&tmp, n_sp[i]);
				solution[i] = tmp / combinations;
			}
			// for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) solution[i] = 0; (already zero-initialized)
		}
	}

	/*
	 * DELETE
	 */
	mCalculator->free_largeNumberArray(n_sp);
	delete[] n_sp;

	return solution;
}

std::string epic::index::AbsoluteBanzhaf::getFullName() {
	return "AbsoluteBanzhaf";
}
