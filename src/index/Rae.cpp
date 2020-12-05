#include "index/Rae.h"

#include "Logging.h"

#include <iostream>

epic::index::Rae::Rae(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawBanzhaf(g, approx, int_representation) {
	if (mGame.getNumberOfNullPlayers() > 0 && mGame.getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::invalidFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::Rae::calculate() {
	// n_sp[x]: number of times player x is a swing player
	auto n_sp = new lint::LargeNumber[mNonZeroPlayerCount];
	mCalculator->allocInit_largeNumberArray(n_sp, mNonZeroPlayerCount);
	numberOfTimesPlayerIsSwingPlayer(n_sp);

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		// number of different coalitions = 2^n
		bigFloat n_combinations = bigInt(1) << mNonZeroPlayerCount;
		bigInt sp;
		bigFloat gamma = 0.5;

		if (mGame.getFlagOfVerbose()) {
			bigInt factor = bigInt(1) << mGame.getNumberOfPlayersWithWeight0();

			std::cout << "Number of times each player is a swing player:" << std::endl;

			for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
				mCalculator->to_bigInt(&sp, n_sp[i]);
				solution[i] = (sp / n_combinations) + gamma;
				std::cout << "Player " << mGame.playerIndexToNumber(i) << ": " << sp * factor << std::endl;
			}
			for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
				solution[i] = 0.5;
				std::cout << "Player " << mGame.playerIndexToNumber(i) << ": 0" << std::endl;
			}
		} else {
			for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
				mCalculator->to_bigInt(&sp, n_sp[i]);
				solution[i] = (sp / n_combinations) + gamma;
			}
			for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
				solution[i] = 0.5;
			}
		}
	}

	mCalculator->free_largeNumberArray(n_sp);
	delete[] n_sp;

	return solution;
}

std::string epic::index::Rae::getFullName() {
	return "Rae";
}
