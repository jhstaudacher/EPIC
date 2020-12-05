#include "index/PowerIndexWithWinningCoalitions.h"

epic::index::PowerIndexWithWinningCoalitions::PowerIndexWithWinningCoalitions(Game& g)
	: ItfPowerIndex(g) {
}

void epic::index::PowerIndexWithWinningCoalitions::numberOfWinningCoalitionsPerWeight(ArrayOffset<lint::LargeNumber>& n_wc) {
	// initialize the grand coalition (weight == weightsum) with 1, since it is always winning
	mCalculator->assign_one(n_wc[mGame.getWeightSum()]);

	// For each player...
	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
		// wi: weight of player i
		longUInt wi = mGame.getWeights()[i];

		/*
		 * Iterate over the array starting at the value of quota + wi. This is the coalition weight from whereon
		 * subtracting player i's weight does not turn it into a losing coalition.
		 * Then iterate over all possible combinations of the existing winning coalitions in the array with the new
		 * player. The grand coalition minus the player's weight is always a new winning coalition (it the player is
		 * not a veto player).
		 * From there we can continue this recursive scheme: If a winning coalition exists with weight k, at least
		 * one coalition with weight k - wi exists and is winning (int range quota + wi to weightsum).
		 * This works by adding the number of winning coalitions of weight k to the number of possible winning
		 * coalitions of weight k - wi (we know that subtracting player i's weight does not turn the coalition
		 * losing, since we started at quota + wi, ensuring that).
		 */
		for (longUInt k = mGame.getQuota() + wi; k <= mGame.getWeightSum(); ++k) {
			mCalculator->plusEqual(n_wc[k - wi], n_wc[k]);
		}
	}
}

void epic::index::PowerIndexWithWinningCoalitions::numberOfWinningCoalitions(ArrayOffset<lint::LargeNumber>& n_wc, lint::LargeNumber& total_wc) {
	for (longUInt i = mGame.getQuota(); i <= mGame.getWeightSum(); ++i) {
		mCalculator->plusEqual(total_wc, n_wc[i]);
	}
}
