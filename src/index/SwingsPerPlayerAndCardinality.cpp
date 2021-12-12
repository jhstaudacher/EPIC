#include "index/SwingsPerPlayerAndCardinality.h"

#include "lint/GlobalCalculator.h"

epic::index::SwingsPerPlayerAndCardinality::SwingsPerPlayerAndCardinality() : ItfPowerIndex() {}

epic::longUInt epic::index::SwingsPerPlayerAndCardinality::getMemoryRequirement(Game* g) {
	bigInt memory = bigInt(g->getWeightSum() + 1 - g->getQuota()) * (g->getNumberOfNonZeroPlayers() + 1) * gCalculator->getLargeNumberSize() * 2; // n_wc, helper_wc
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

void epic::index::SwingsPerPlayerAndCardinality::numberOfWinningCoalitionsPerWeightAndCardinality(Game* g, Array2dOffset<lint::LargeNumber>& n_wc) {
	// initialize the grand coalition (weight = weightsum, cardinality = numberofplayers) with 1, since it is always winning
	gCalculator->assign_one(n_wc(g->getWeightSum(), g->getNumberOfNonZeroPlayers()));

	/*
	 * Find out winning coalitions per weight sum and cardinality. Iterate from the weight = quota + wi upwards since a coalition of that weight is guaranteed to be a winning coalition even without player i.
	 *
	 * The grand coalition minus the player's weight is always a new winning coalition (if the player is no veto player). That's why the grand coalition minus player i is a new winning coalition.
	 * This scheme can recursively continued: If a winning coalition exists with weight k, at least one coalition with weight k - wi exists, is winning (only int the range from quota + wi to weightsum) and has one player less.
	 *
	 * The difference to the Banzhaf calculation is only the second for-loop for the cardinality.
	 *
	 * The value n_wc[k][m] is then added to the coalition count of n_wc[k - wi][m - 1], since the coalition loses player i's weight (k - wi) and one player (m - 1).
	 */
	for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
		longUInt wi = g->getWeights()[i];

		for (longUInt k = g->getQuota() + wi; k <= g->getWeightSum(); ++k) {
			for (longUInt m = 1; m <= g->getNumberOfNonZeroPlayers(); ++m) {
				gCalculator->plusEqual(n_wc(k - wi, m - 1), n_wc(k, m));
			}
		}
	}
}

void epic::index::SwingsPerPlayerAndCardinality::swingsPerPlayerAndCardinality(Game* g, Array2dOffset<lint::LargeNumber>& n_wc, Array2d<lint::LargeNumber>& raw_ssi, bool only_swingplayer) {
	// helper_wc(x, y): helper matrix for n_wc
	Array2dOffset<lint::LargeNumber> helper(g->getWeightSum() + 1, g->getNumberOfNonZeroPlayers() + 1, g->getQuota(), 0);
	gCalculator->allocInit_largeNumberArray(helper.getArrayPointer(), helper.getNumberOfElements());

	for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
		longUInt wi = g->getWeights()[i]; // all weights are greater than zero!

		// quota - 1 is only the maximum if player i is a veto player
		longUInt m = std::max(g->getWeightSum() - wi, g->getQuota() - 1);
		for (longUInt k = g->getWeightSum(); k > m; --k) {
			for (longUInt p = 1; p <= g->getNumberOfNonZeroPlayers(); ++p) {
				gCalculator->assign(helper(k, p), n_wc(k, p));
			}
		}

		/*
		 * Filtering out all coalitions which player i is not a member of, since those coalitions cannot be turned by player i as they are already winning.
		 * This is done recursively starting at helper_wc[weightsum - wi][p] = n_wc[k][p] - helper_wc[k + wi][p + 1] for each possible cardinality p.
		 * The coalitions with weight = weightsum - wi do not contain player i at least once. This winning coalition  has one player less than the grand coalition. So 1 (= helper_wc[weightsum][numberOfPlayers]) is substracted initially from n_wc[weightsum - wi][numberOfPlayers - 1] and the new value added to helper_wc[weightsum - wi][numberOfPlayers - 1]. This scheme continues until the quota is reached (from above).
		 *
		 * n_wc[k][p] is the number of coalitions with weight k and cardinality p which could contain player i.
		 * helper_wc[k + wi][p + 1] is the number of coalitions of weight = k + wi and cardinality = p + 1 representing the count of coalitions of cardinality = p + 1 containing player i.
		 *
		 * These numbers are subtracted to find the number of coalitions of a certain weight that do contain player i. This number is then put into the array at helper_wc[k][p] representing the number of coalitions that player i is a member of.
		 *
		 * The only difference to the Banzhaf calculation is the second for loop for the cardinalities.
		 */
		for (longUInt k = g->getWeightSum() - wi; k >= g->getQuota(); --k) {
			for (longUInt p = 0; p < g->getNumberOfNonZeroPlayers(); ++p) {
				gCalculator->minus(helper(k, p), n_wc(k, p), helper(k + wi, p + 1));
			}
		}

		/*
		 * m is the end of the range, where player i stops be a swing player and we therefore stop adding the number of coalitions with a certain weight and size to our count for player i.
		 * weightsum + 1 is only ever reached if player i is a veto player, being in every coalition from the quota to weightsum + 1 that is winning.
		 */
		m = only_swingplayer ? std::min(g->getWeightSum() + 1, g->getQuota() + wi) : g->getWeightSum() + 1;
		for (longUInt k = g->getQuota(); k < m; ++k) {
			for (longUInt p = 1; p <= g->getNumberOfNonZeroPlayers(); ++p) {
				gCalculator->plusEqual(raw_ssi(i, p), helper(k, p));
			}
		}
	}

	gCalculator->free_largeNumberArray(helper.getArrayPointer());
}

void epic::index::SwingsPerPlayerAndCardinality::swingsPerPlayerAndCardinality(Game* g, Array2d<lint::LargeNumber>& raw_ssi, bool only_swingplayer) {
	// n_wc(x, y) => n_wc[x][y]: number of winning coalitions of weight x and cardinality y
	Array2dOffset<lint::LargeNumber> n_wc(g->getWeightSum() + 1, g->getNumberOfNonZeroPlayers() + 1, g->getQuota(), 0);
	gCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());

	numberOfWinningCoalitionsPerWeightAndCardinality(g, n_wc);
	swingsPerPlayerAndCardinality(g, n_wc, raw_ssi, only_swingplayer);

	gCalculator->free_largeNumberArray(n_wc.getArrayPointer());
}
