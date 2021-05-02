#include "index/RawPublicGood.h"

#include "Array.h"

#include <iostream>

epic::index::RawPublicGood::RawPublicGood(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: ItfPowerIndex(g) {
	bigInt max_value = approx->upperBound_totalNumberOfSwingPlayer();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::multiplication, int_representation);
}

epic::index::RawPublicGood::~RawPublicGood() {
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::RawPublicGood::calculate() {
	auto mwc = new lint::LargeNumber[mNonZeroPlayerCount];
	mCalculator->allocInit_largeNumberArray(mwc, mNonZeroPlayerCount);
	calculateMinimalWinningCoalitionsPerPlayer(mwc);

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	bigInt big_mwc;

	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
		mCalculator->to_bigInt(&big_mwc, mwc[i]);
		solution[i] = big_mwc;
	}
	for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
		solution[i] = 0;
	}

	mCalculator->free_largeNumberArray(mwc);
	delete[] mwc;

	return solution;
}

std::string epic::index::RawPublicGood::getFullName() {
	return "RawPublicGood";
}

epic::longUInt epic::index::RawPublicGood::getMemoryRequirement() {
	bigInt memory = mNonZeroPlayerCount * mCalculator->getLargeNumberSize(); // mwc

	// The memory needed inside the calculateMinimalWinningCoalitionsPerPlayer() function
	memory += mGame.getQuota() * mCalculator->getLargeNumberSize();			  // f
	memory += (mGame.getQuota() + 1) * mCalculator->getLargeNumberSize() * 2; // b, b_helper
	// don't add solution vector since it gets allocated after the previous arrays are already freed.
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

void epic::index::RawPublicGood::calculateFVector(lint::LargeNumber f[]) {
	// Initialize the empty coalition with 1 since we know, that the empty coalition is always losing.
	mCalculator->assign_one(f[0]);

	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
		longUInt wi = mGame.getWeights()[i];

		// Players are veto players
		if (wi < mGame.getQuota()) {
			for (longUInt k = mGame.getQuota() - wi - 1; k < mGame.getQuota(); --k) {
				mCalculator->plusEqual(f[k + wi], f[k]);
			}
		}
	}
}

void epic::index::RawPublicGood::calculateMinimalWinningCoalitionsPerPlayer(lint::LargeNumber mwc[]) {
	auto f = new lint::LargeNumber[mGame.getQuota()];
	mCalculator->allocInit_largeNumberArray(f, mGame.getQuota());
	calculateFVector(f);

	auto b = new lint::LargeNumber[mGame.getQuota() + 1];
	mCalculator->allocInit_largeNumberArray(b, mGame.getQuota() + 1);

	lint::LargeNumber tmp;
	mCalculator->alloc_largeNumber(tmp);

	// Last Player
	{
		longUInt last_player = mNonZeroPlayerCount - 1;
		longUInt w_last = mGame.getWeights()[last_player];

		if (w_last < mGame.getQuota()) {
			for (longUInt i = 0; i <= w_last; ++i) {
				mCalculator->assign_one(b[i]);
			}

			for (longUInt i = w_last; i < mGame.getQuota(); ++i) {
				mCalculator->minusEqual(f[i], f[i - w_last]);
			}
		}

		for (longUInt i = 0; i < mGame.getQuota(); ++i) {
			mCalculator->mul(tmp, f[i], b[mGame.getQuota() - i]);
			mCalculator->plusEqual(mwc[last_player], tmp);
		}
	}

	auto b_helper = new lint::LargeNumber[mGame.getQuota() + 1];
	mCalculator->allocInit_largeNumberArray(b_helper, mGame.getQuota() + 1);

	// For each player (except the last one)
	for (longUInt i = mNonZeroPlayerCount - 2; i < mNonZeroPlayerCount; --i) {
		longUInt wi = mGame.getWeights()[i];
		longUInt wi_plus1 = mGame.getWeights()[i + 1];

		for (longUInt k = 0; k <= mGame.getQuota(); ++k) {
			mCalculator->assign(b_helper[k], b[k]);
		}

		for (longUInt k = 0; k <= wi && k <= mGame.getQuota(); ++k) {
			mCalculator->assign_one(b[k]);
		}

		if (wi < mGame.getQuota()) {
			for (longUInt k = wi; k < mGame.getQuota(); ++k) {
				mCalculator->minusEqual(f[k], f[k - wi]);
			}

			for (longUInt k = wi + 1; k <= mGame.getQuota(); ++k) {
				mCalculator->plus(b[k], b_helper[k - wi + wi_plus1], b_helper[k - wi]);
			}
		}

		for (longUInt k = 0; k < mGame.getQuota(); ++k) {
			mCalculator->mul(tmp, f[k], b[mGame.getQuota() - k]);
			mCalculator->plusEqual(mwc[i], tmp);
		}
	}

	// Handling veto players: As they are ignored in the above calculation, we can just look through the player base here and add 1 to all of them, since each one veto player is ONLY ever in 1 minimal winning coalition
	for (longUInt i = 0; i < mNonZeroPlayerCount; i++) {
		// If player is veto player:
		if (mGame.getVetoPlayerVector()[i]) {
			mCalculator->assign(mwc[i], 1ul);
		}
	}

	/**
	 * DELETE
	 */
	mCalculator->free_largeNumberArray(b_helper);
	delete[] b_helper;

	mCalculator->free_largeNumberArray(b);
	delete[] b;

	mCalculator->free_largeNumberArray(f);
	delete[] f;

	mCalculator->free_largeNumber(tmp);
}
