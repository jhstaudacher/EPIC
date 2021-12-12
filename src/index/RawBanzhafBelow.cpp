#include "index/RawBanzhafBelow.h"

#include "lint/GlobalCalculator.h"

#include <iostream>

epic::index::RawBanzhafBelow::RawBanzhafBelow() : ItfPowerIndex() {}

std::vector<epic::bigFloat> epic::index::RawBanzhafBelow::calculate(Game* g) {
	// n_sp[x]: number of times player x is a swing player
	auto n_sp = new lint::LargeNumber[g->getNumberOfNonZeroPlayers()];
	gCalculator->allocInit_largeNumberArray(n_sp, g->getNumberOfNonZeroPlayers());
	numberOfTimesPlayerIsSwingPlayer(g, n_sp);

	// solution[x]: the relative banzhaf index for player x
	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	{
		bigInt tmp;
		bigInt factor = bigInt(1) << g->getNumberOfPlayersWithWeight0(); // additional winning coalitions due to players of weight 0

		for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
			gCalculator->to_bigInt(&tmp, n_sp[i]);
			solution[i] = tmp * factor;
		}
		for (longUInt i = g->getNumberOfNonZeroPlayers(); i < g->getNumberOfPlayers(); ++i) {
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

std::string epic::index::RawBanzhafBelow::getFullName() {
	return "RawBanzhafBelow";
}

epic::longUInt epic::index::RawBanzhafBelow::getMemoryRequirement(Game* g) {
	bigInt memory = g->getNumberOfNonZeroPlayers() * gCalculator->getLargeNumberSize();						 // n_sp;
	memory += (g->getWeightSum() + 1 - g->getQuota()) * gCalculator->getLargeNumberSize() * 2; // n_wc + helper
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

void epic::index::RawBanzhafBelow::numberOfTimesPlayerIsSwingPlayer(Game* g, lint::LargeNumber n_sp[]) {
	// n_lc[x]: number of losing coalitions of weight x
	auto n_lc = new lint::LargeNumber[g->getQuota()];
	gCalculator->allocInit_largeNumberArray(n_lc, g->getQuota());
	numberOfLosingCoalitionsPerWeight(g, n_lc);

	numberOfTimesPlayerIsSwingPlayer(g, n_lc, n_sp);

	gCalculator->free_largeNumberArray(n_lc);
}

void epic::index::RawBanzhafBelow::numberOfTimesPlayerIsSwingPlayer(Game* g, lint::LargeNumber n_lc[], lint::LargeNumber n_sp[]) {
	// helper:: helper array for n_lc
	auto helper = new lint::LargeNumber[g->getQuota()];
	gCalculator->allocInit_largeNumberArray(helper, g->getQuota());

	for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
		// wi: weight of player i (the current player)
		longUInt wi = g->getWeights()[i];

		longUInt m = std::min(wi, g->getQuota());
		for (longUInt k = 0; k < m; ++k) {
			gCalculator->assign(helper[k], n_lc[k]);
		}

		for (longUInt k = wi; k < g->getQuota(); ++k) {
			gCalculator->minus(helper[k], n_lc[k], helper[k - wi]);
		}

		m = wi > g->getQuota() ? 0UL : g->getQuota() - wi;
		for (longUInt k = m; k < g->getQuota(); ++k) {
			gCalculator->plusEqual(n_sp[i], helper[k]);
		}
	}

	gCalculator->free_largeNumberArray(helper);
}

void epic::index::RawBanzhafBelow::numberOfSwingPlayer(Game* g, lint::LargeNumber n_sp[], lint::LargeNumber& total_sp) {
	for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
		gCalculator->plusEqual(total_sp, n_sp[i]);
	}
}

void epic::index::RawBanzhafBelow::numberOfLosingCoalitionsPerWeight(Game* g, lint::LargeNumber n_lc[]) {
	// initialize the empty coalition (weight = 0) with 1, since it is always losing
	gCalculator->assign_one(n_lc[0]);

	// For each player...
	for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
		// wi: weight of player i
		longUInt wi = g->getWeights()[i];

		/* Reverse-iterate over the array starting at the value of game.getQuota() - weight of player i - 1
		 *	-> this value is the starting position in the array from where on player i is no swing player. Then iterate over all possible combinations of the existing losing coalitions in the array including the new player.
		 *
		 * If a losing coalition exists with weight k, at least one coalition with k + weight_of_player_i exists and is losing. (in range of quota - weight_of_player_i - 1 to 0). This works by adding the number of losing coalitions of weight k to the number of possible losing coalitions of weight k + weight of player i.
		 */
		for (longInt k = g->getQuota() - wi - 1; k >= 0; --k) {
			gCalculator->plusEqual(n_lc[k + wi], n_lc[k]);
		}
	}
}

epic::bigInt epic::index::RawBanzhafBelow::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_totalNumberOfSwingPlayer();
}

epic::lint::Operation epic::index::RawBanzhafBelow::getOperationRequirement() {
	return lint::Operation::addition;
}
