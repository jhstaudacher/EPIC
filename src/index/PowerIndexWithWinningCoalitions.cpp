#include "index/PowerIndexWithWinningCoalitions.h"

#include "lint/GlobalCalculator.h"

void epic::index::PowerIndexWithWinningCoalitions::numberOfWinningCoalitionsPerWeight(Game& g, ArrayOffset<lint::LargeNumber>& n_wc) {
	// initialize the grand coalition (weight == weightsum) with 1, since it is always winning
	gCalculator->assign_one(n_wc[g.getWeightSum()]);

	// For each player...
	for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) {
		// wi: weight of player i
		longUInt wi = g.getWeights()[i];

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
		for (longUInt k = g.getQuota() + wi; k <= g.getWeightSum(); ++k) {
			gCalculator->plusEqual(n_wc[k - wi], n_wc[k]);
		}
	}
}

void epic::index::PowerIndexWithWinningCoalitions::numberOfWinningCoalitions(Game& g, ArrayOffset<lint::LargeNumber>& n_wc, lint::LargeNumber& total_wc) {
	for (longUInt i = g.getQuota(); i <= g.getWeightSum(); ++i) {
		gCalculator->plusEqual(total_wc, n_wc[i]);
	}
}
