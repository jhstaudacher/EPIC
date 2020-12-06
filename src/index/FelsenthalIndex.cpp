#include "index/FelsenthalIndex.h"

#include <iostream>

epic::index::FelsenthalIndex::FelsenthalIndex(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawFelsenthal(g, approx, int_representation) {
}

std::vector<epic::bigFloat> epic::index::FelsenthalIndex::calculate() {
	// if a veto player exists, that player gets assigned 1 while all other players get assigned 0
	// note: the weights have to be sorted decreasingly
	if (mGame.getWeights()[0] >= mGame.getQuota()) {
		std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
		{
			solution[0] = 1.0;

			for (longUInt i = 1; i < mGame.getNumberOfPlayers(); ++i) {
				solution[i] = 0.0;
			}
		}
		return solution;
	}
	// mwcs: number of minimal winning coalitions for each player i
	auto mwcs = new lint::LargeNumber[mNonZeroPlayerCount];
	mCalculator->allocInit_largeNumberArray(mwcs, mNonZeroPlayerCount);
	minimal_winning_coalitions_of_least_size(mwcs);

	// sum_mwcs: number of all minimal winning coalitions
	lint::LargeNumber sum_mwcs;
	mCalculator->allocInit_largeNumber(sum_mwcs);

	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
		mCalculator->plusEqual(sum_mwcs, mwcs[i]);
	}

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		bigInt int_tmp;
		bigFloat arbitrary_sum_mwcs;

		mCalculator->to_bigInt(&int_tmp, sum_mwcs);
		arbitrary_sum_mwcs = int_tmp;

		if (mGame.getFlagOfVerbose()) {
			std::cout << "Total number of minimal winning coalitions of least size: " << arbitrary_sum_mwcs << std::endl
					  << std::endl;
			std::cout << "Number of minimal winning coalitions of least size individual players belong to: " << std::endl;
		}

		// compute Felsenthal-index: Number of minimal winning coalitions of player i / (SUM(Number of minimal winning coalitions of all players))
		if (arbitrary_sum_mwcs == 0) {
			std::cout << "warning: sum of minimal winning coalitions of least size is zero!" << std::endl;
		} else {
			for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
				mCalculator->to_bigInt(&int_tmp, mwcs[i]);
				if (mGame.getFlagOfVerbose()) {
					std::cout << "Player " << mGame.playerIndexToNumber(i) << ": " << int_tmp << std::endl;
				}
				solution[i] = (bigFloat)int_tmp / arbitrary_sum_mwcs;
			}

			for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
				solution[i] = 0;
			}
		}
	}

	/*
	 * DELETE
	 */
	mCalculator->free_largeNumber(sum_mwcs);
	mCalculator->free_largeNumberArray(mwcs);
	delete[] mwcs;

	return solution;
}

std::string epic::index::FelsenthalIndex::getFullName() {
	return "Felsenthal Index";
}

epic::longUInt epic::index::FelsenthalIndex::getMemoryRequirement() {
	bigInt memory = RawFelsenthal::getMemoryRequirement();
	memory += mCalculator->getLargeNumberSize(); // sum_mwcs

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}
