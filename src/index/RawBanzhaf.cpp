#include "index/RawBanzhaf.h"

#include <iostream>

epic::index::RawBanzhaf::RawBanzhaf(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: PowerIndexWithWinningCoalitions(g) {
	bigInt max_value = approx->upperBound_totalNumberOfSwingPlayer();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);
}

epic::index::RawBanzhaf::~RawBanzhaf() {
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::RawBanzhaf::calculate() {
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

std::string epic::index::RawBanzhaf::getFullName() {
	return "RawBanzhaf";
}

epic::longUInt epic::index::RawBanzhaf::getMemoryRequirement() {
	bigInt memory = mNonZeroPlayerCount * mCalculator->getLargeNumberSize();						 // n_sp;
	memory += (mGame.getWeightSum() + 1 - mGame.getQuota()) * mCalculator->getLargeNumberSize() * 2; // n_wc + helper

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

void epic::index::RawBanzhaf::numberOfTimesPlayerIsSwingPlayer(lint::LargeNumber n_sp[]) {
	// n_wc[x]: number of winning coalitions of weight x.
	ArrayOffset<lint::LargeNumber> n_wc(mGame.getWeightSum() + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeight(n_wc);

	numberOfTimesPlayerIsSwingPlayer(n_wc, n_sp);

	mCalculator->free_largeNumberArray(n_wc.getArrayPointer());
}

void epic::index::RawBanzhaf::numberOfTimesPlayerIsSwingPlayer(ArrayOffset<lint::LargeNumber>& n_wc, lint::LargeNumber n_sp[]) {
	// helper: helper array for n_wc
	ArrayOffset<lint::LargeNumber> helper(mGame.getWeightSum() + 1, mGame.getQuota());
	mCalculator->alloc_largeNumberArray(helper.getArrayPointer(), helper.getNumberOfElements());

	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
		// wi: weight of player i (the current player)
		int wi = mGame.getWeights()[i];

		// quota - 1 is the maximum only if i is a veto player
		longUInt m = std::max(mGame.getWeightSum() - wi, mGame.getQuota() - 1);
		/*
		 * Initialize helper array bz_helper for the values from weight sum to weightsum - wi + 1.
		 * bz_helper[x]: number of winning coalitions containing player x.
		 */
		for (longUInt k = mGame.getWeightSum(); k > m; --k) {
			mCalculator->assign(helper[k], n_wc[k]);
		}

		/*
		 * Filter out all winning coalitions which player is not a member of, since those coalitions cannot be turned
		 * by player i as they are already winning (in the range weight sum - wi to quota).
		 * This is done recursively starting at bz_helper[weight sum - wi] = n_wc[weightsum - wi] -
		 * bz_helper[weightsum].
		 * The coalitions with weight weightsum - wi do not contain player i at least on time as all the other
		 * possible players form the coalition with this weight once. So 1 (= bz_helper[weightsum]) gets subtracted
		 * initially from n_wc[weightsum - wi] and the new value gets added to bz_helper[weightsum - wi].
		 * This scheme continues until the quota is reached from above.
		 *
		 * n_wc[k]: The number of winning coalitions with weight k which could contain player i.
		 * bz_helper[k + wi]: The number of winning coalitions of weight k + wi representing the amount of coalitions
		 * containing player i.
		 *
		 * Then bz_helper[k + wi] gets added to bz_helper[k] representing the number of winning coalitions that
		 * player i is a member of.
		 */
		for (longUInt k = mGame.getWeightSum() - wi; k >= mGame.getQuota(); --k) {
			mCalculator->minus(helper[k], n_wc[k], helper[k + wi]);
		}

		/*
		 * Sum up the values where bz_helper[k] is a winning coalition and contains player i from the quota to quota
		 * + wi - 1. Subtracting the weight of player i from any of those coalitions would turn them into a losing one.
		 */
		m = std::min(mGame.getQuota() + wi, mGame.getWeightSum() + 1);
		for (longUInt k = mGame.getQuota(); k < m; ++k) {
			// Add all the values to get the amount how often player i is a swing player
			mCalculator->plusEqual(n_sp[i], helper[k]);
		}
	}

	mCalculator->free_largeNumberArray(helper.getArrayPointer());
}

void epic::index::RawBanzhaf::numberOfSwingPlayer(lint::LargeNumber n_sp[], lint::LargeNumber& total_sp) {
	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
		mCalculator->plusEqual(total_sp, n_sp[i]);
	}
}
