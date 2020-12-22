#include "index/AbsolutePublicGood.h"

#include "Array.h"
#include "Logging.h"

#include <iostream>

epic::index::AbsolutePublicGood::AbsolutePublicGood(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawPublicGood(g, approx, int_representation) {
}

std::vector<epic::bigFloat> epic::index::AbsolutePublicGood::calculate() {
	bigInt total_mwc;
	numberOfMinimalWinningCoalitions(&total_mwc);
	if (total_mwc <= 0) {
		throw std::invalid_argument("Number of minimal winning coalitions is less than or equal to zero. No calculation possible. Please, check your input.");
	}

	auto mwc = new lint::LargeNumber[mNonZeroPlayerCount];
	mCalculator->allocInit_largeNumberArray(mwc, mNonZeroPlayerCount);	

	calculateMinimalWinningCoalitionsPerPlayer(mwc);

	log::out << log::info << "Total number of minimal winning coalitions: " << total_mwc << log::endl;
	log::out << log::info << "Number of minimal winning coalitions individual players belong to: " << log::endl;


	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		bigInt big_mwc;

		for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
			mCalculator->to_bigInt(&big_mwc, mwc[i]);
			solution[i] = big_mwc;
			log::out << "Player " << mGame.playerIndexToNumber(i) << ": " << big_mwc << log::endl;
			solution[i] /= total_mwc;
		}
		for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
			solution[i] = 0;
			log::out << "Player " << mGame.playerIndexToNumber(i) << ": 0" << log::endl;
		}
	}

	mCalculator->free_largeNumberArray(mwc);
	delete[] mwc;

	return solution;
}

void epic::index::AbsolutePublicGood::numberOfMinimalWinningCoalitions(bigInt* total_mwc) {
	// wc[x]: winning coalitions with weight x
	auto wc = new lint::LargeNumber[mGame.getQuota() + 1];
	mCalculator->allocInit_largeNumberArray(wc, mGame.getQuota() + 1);

	// Initialise the empty coalition with 1, since we know, that the empty coalition is always losing
	mCalculator->assign_one(wc[0]);

	longUInt w_first = mGame.getWeights()[0];
	for (longUInt x = mGame.getQuota() - w_first; x <= mGame.getQuota(); --x) {
		mCalculator->plusEqual(wc[x + w_first], wc[x]);
	}

	// sum_minwc[i]: sum of how many times player i is smallest player in a minimum winning coalition
	auto sum_minwc = new lint::LargeNumber[mNonZeroPlayerCount];
	mCalculator->allocInit_largeNumberArray(sum_minwc, mNonZeroPlayerCount);

	// only look at players of weight > 0
	for (longUInt i = 1; i < mNonZeroPlayerCount; ++i) {
		longUInt wi = mGame.getWeights()[i];

		if (wi < mGame.getQuota()) {
			for (longUInt x = mGame.getQuota() - wi; x <= mGame.getQuota() - 1; ++x) {
				mCalculator->plusEqual(sum_minwc[i], wc[x]);
			}
		}

		for (longInt x = mGame.getQuota() - wi + 1; x > 0; --x) {
			mCalculator->plusEqual(wc[x + wi - 1], wc[x - 1]);
		}
	}

	//if one of the player has a weight >= quota
	for (longUInt i = 0; i < mNonZeroPlayerCount && mGame.getWeights()[i] >= mGame.getQuota(); ++i) {
		mCalculator->increment(sum_minwc[i]);
	}

	// sum up the minimal winning coalitions
	bigInt big_tmp; // number of minimal winning coalitions of player i
	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
		big_tmp = 0;
		mCalculator->to_bigInt(&big_tmp, sum_minwc[i]);
		*total_mwc += big_tmp;
	}

	/*
	 * DELETE
	 */
	mCalculator->free_largeNumberArray(wc);
	delete[] wc;

	mCalculator->free_largeNumberArray(sum_minwc);
	delete[] sum_minwc;
}

std::string epic::index::AbsolutePublicGood::getFullName() {
	return "AbsolutePublicGood";
}


