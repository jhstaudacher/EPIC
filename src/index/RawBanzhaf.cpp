#include "index/RawBanzhaf.h"

#include "lint/GlobalCalculator.h"

#include <iostream>

epic::index::RawBanzhaf::RawBanzhaf() : PowerIndexWithWinningCoalitions() {}

std::vector<epic::bigFloat> epic::index::RawBanzhaf::calculate(Game& g) {
	// n_sp[x]: number of times player x is a swing player
	auto n_sp = new lint::LargeNumber[g.getNumberOfNonZeroPlayers()];
	gCalculator->allocInit_largeNumberArray(n_sp, g.getNumberOfNonZeroPlayers());
	numberOfTimesPlayerIsSwingPlayer(g, n_sp);

	// solution[x]: the relative banzhaf index for player x
	std::vector<bigFloat> solution(g.getNumberOfPlayers());
	{
		bigInt tmp;
		bigInt factor = bigInt(1) << g.getNumberOfPlayersWithWeight0(); // additional winning coalitions due to players of weight 0

		for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) {
			gCalculator->to_bigInt(&tmp, n_sp[i]);
			solution[i] = tmp * factor;
		}
		for (longUInt i = g.getNumberOfNonZeroPlayers(); i < g.getNumberOfPlayers(); ++i) {
			solution[i] = 0;
		}
	}

	/*
	 * DELETE
	 */
	gCalculator->free_largeNumberArray(n_sp);
	delete[] n_sp;

	return solution;
}

std::string epic::index::RawBanzhaf::getFullName() {
	return "RawBanzhaf";
}

epic::longUInt epic::index::RawBanzhaf::getMemoryRequirement(Game& g) {
	bigInt memory = g.getNumberOfNonZeroPlayers() * gCalculator->getLargeNumberSize();						 // n_sp;
	memory += (g.getWeightSum() + 1 - g.getQuota()) * gCalculator->getLargeNumberSize() * 2; // n_wc + helper
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

epic::bigInt epic::index::RawBanzhaf::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_totalNumberOfSwingPlayer();
}

epic::lint::Operation epic::index::RawBanzhaf::getOperationRequirement() {
	return lint::Operation::addition;
}

void epic::index::RawBanzhaf::numberOfTimesPlayerIsSwingPlayer(Game& g, lint::LargeNumber n_sp[]) {
	// n_wc[x]: number of winning coalitions of weight x.
	ArrayOffset<lint::LargeNumber> n_wc(g.getWeightSum() + 1, g.getQuota());
	gCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeight(g, n_wc);

	numberOfTimesPlayerIsSwingPlayer(g, n_wc, n_sp);

	gCalculator->free_largeNumberArray(n_wc.getArrayPointer());
}

void epic::index::RawBanzhaf::numberOfTimesPlayerIsSwingPlayer(Game& g, ArrayOffset<lint::LargeNumber>& n_wc, lint::LargeNumber n_sp[]) {
	// helper: helper array for n_wc
	ArrayOffset<lint::LargeNumber> helper(g.getWeightSum() + 1, g.getQuota());
	gCalculator->alloc_largeNumberArray(helper.getArrayPointer(), helper.getNumberOfElements());

	for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) {
		// wi: weight of player i (the current player)
		int wi = g.getWeights()[i];

		// quota - 1 is the maximum only if i is a veto player
		longUInt m = std::max(g.getWeightSum() - wi, g.getQuota() - 1);
		/*
		 * Initialize helper array bz_helper for the values from weight sum to weightsum - wi + 1.
		 * bz_helper[x]: number of winning coalitions containing player x.
		 */
		for (longUInt k = g.getWeightSum(); k > m; --k) {
			gCalculator->assign(helper[k], n_wc[k]);
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
		for (longUInt k = g.getWeightSum() - wi; k >= g.getQuota(); --k) {
			gCalculator->minus(helper[k], n_wc[k], helper[k + wi]);
		}

		/*
		 * Sum up the values where bz_helper[k] is a winning coalition and contains player i from the quota to quota
		 * + wi - 1. Subtracting the weight of player i from any of those coalitions would turn them into a losing one.
		 */
		m = std::min(g.getQuota() + wi, g.getWeightSum() + 1);
		for (longUInt k = g.getQuota(); k < m; ++k) {
			// Add all the values to get the amount how often player i is a swing player
			gCalculator->plusEqual(n_sp[i], helper[k]);
		}
	}

	gCalculator->free_largeNumberArray(helper.getArrayPointer());
}

void epic::index::RawBanzhaf::numberOfSwingPlayer(Game&g, lint::LargeNumber n_sp[], lint::LargeNumber& total_sp) {
	for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) {
		gCalculator->plusEqual(total_sp, n_sp[i]);
	}
}
