#include "index/RawBanzhafBelow.h"

#include <iostream>

epic::index::RawBanzhafBelow::RawBanzhafBelow(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: ItfPowerIndex(g) {
	bigInt max_value = approx->upperBound_totalNumberOfSwingPlayer();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);
}

epic::index::RawBanzhafBelow::~RawBanzhafBelow() {
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::RawBanzhafBelow::calculate() {
	// n_sp[x]: number of times player x is a swing player
	auto n_sp = new lint::LargeNumber[mNonZeroPlayerCount];
	mCalculator->allocInit_largeNumberArray(n_sp, mNonZeroPlayerCount);
	numberOfTimesPlayerIsSwingPlayer(n_sp);

	// solution[x]: the relative banzhaf index for player x
	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		bigInt tmp;
		bigInt factor = bigInt(1) << mGame.getNumberOfPlayersWithWeight0(); // additional winning coalitions due to players of weight 0

		for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
			mCalculator->to_bigInt(&tmp, n_sp[i]);
			solution[i] = tmp * factor;
		}
		for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
			solution[i] = 0;
		}
	}

	/*
	 * DELETE
	 */
	mCalculator->free_largeNumberArray(n_sp);
	delete[] n_sp;

	return solution;
}

std::string epic::index::RawBanzhafBelow::getFullName() {
	return "RawBanzhafBelow";
}

epic::longUInt epic::index::RawBanzhafBelow::getMemoryRequirement() {
	bigInt memory = mNonZeroPlayerCount * mCalculator->getLargeNumberSize();						 // n_sp;
	memory += (mGame.getWeightSum() + 1 - mGame.getQuota()) * mCalculator->getLargeNumberSize() * 2; // n_wc + helper
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

void epic::index::RawBanzhafBelow::numberOfTimesPlayerIsSwingPlayer(lint::LargeNumber n_sp[]) {
	// n_lc[x]: number of losing coalitions of weight x
	auto n_lc = new lint::LargeNumber[mGame.getQuota()];
	mCalculator->allocInit_largeNumberArray(n_lc, mGame.getQuota());
	numberOfLosingCoalitionsPerWeight(n_lc);

	numberOfTimesPlayerIsSwingPlayer(n_lc, n_sp);

	mCalculator->free_largeNumberArray(n_lc);
}

void epic::index::RawBanzhafBelow::numberOfTimesPlayerIsSwingPlayer(lint::LargeNumber n_lc[], lint::LargeNumber n_sp[]) {
	// helper:: helper array for n_lc
	auto helper = new lint::LargeNumber[mGame.getQuota()];
	mCalculator->allocInit_largeNumberArray(helper, mGame.getQuota());

	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
		// wi: weight of player i (the current player)
		longUInt wi = mGame.getWeights()[i];

		longUInt m = std::min(wi, mGame.getQuota());
		for (longUInt k = 0; k < m; ++k) {
			mCalculator->assign(helper[k], n_lc[k]);
		}

		for (longUInt k = wi; k < mGame.getQuota(); ++k) {
			mCalculator->minus(helper[k], n_lc[k], helper[k - wi]);
		}

		m = wi > mGame.getQuota() ? 0UL : mGame.getQuota() - wi;
		for (longUInt k = m; k < mGame.getQuota(); ++k) {
			mCalculator->plusEqual(n_sp[i], helper[k]);
		}
	}

	mCalculator->free_largeNumberArray(helper);
}

void epic::index::RawBanzhafBelow::numberOfSwingPlayer(lint::LargeNumber n_sp[], lint::LargeNumber& total_sp) {
	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
		mCalculator->plusEqual(total_sp, n_sp[i]);
	}
}

void epic::index::RawBanzhafBelow::numberOfLosingCoalitionsPerWeight(lint::LargeNumber n_lc[]) {
	// initialize the empty coalition (weight = 0) with 1, since it is always losing
	mCalculator->assign_one(n_lc[0]);

	// For each player...
	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
		// wi: weight of player i
		longUInt wi = mGame.getWeights()[i];

		/* Reverse-iterate over the array starting at the value of game.getQuota() - weight of player i - 1
		 *	-> this value is the starting position in the array from where on player i is no swing player. Then iterate over all possible combinations of the existing losing coalitions in the array including the new player.
		 *
		 * If a losing coalition exists with weight k, at least one coalition with k + weight_of_player_i exists and is losing. (in range of quota - weight_of_player_i - 1 to 0). This works by adding the number of losing coalitions of weight k to the number of possible losing coalitions of weight k + weight of player i.
		 */
		for (longInt k = mGame.getQuota() - wi - 1; k >= 0; --k) {
			mCalculator->plusEqual(n_lc[k + wi], n_lc[k]);
		}
	}
}
