#include "index/RawPublicHelpTheta.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

epic::longUInt epic::index::RawPublicHelpTheta::getMemoryRequirement(Game* g) {
	bigInt memory = (g->getWeightSum() + 1 - g->getQuota()) * gCalculator->getLargeNumberSize() * 2;  // n_wc, helper
	memory += g->getNumberOfPlayers() * gCalculator->getLargeNumberSize();							  // wci
	memory += GMPHelper::size_of_int(bigInt(1) << g->getNumberOfPlayers()) * g->getNumberOfPlayers(); // big_wci
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

epic::bigInt epic::index::RawPublicHelpTheta::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_numberOfWinningCoalitionsPerWeight();
}

epic::lint::Operation epic::index::RawPublicHelpTheta::getOperationRequirement() {
	return lint::Operation::addition;
}

epic::index::RawPublicHelpTheta::RawPublicHelpTheta(Game* g)
	: PowerIndexWithWinningCoalitions() {
	if (g->getNumberOfNullPlayers() > 0 && g->getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::invalidFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::RawPublicHelpTheta::calculate(Game* g) {
	auto big_wci = new bigInt[g->getNumberOfPlayers()];

	winningCoalitionsForPlayer(g, big_wci);
	bigInt factor = bigInt(1) << g->getNumberOfPlayersWithWeight0(); // additional winning coalitions due to players of weight 0

	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	for (longUInt i = 0; i < g->getNumberOfPlayers(); i++) {
		solution[i] = big_wci[i] * factor;
	}

	delete[] big_wci;
	return solution;
}

std::string epic::index::RawPublicHelpTheta::getFullName() {
	return "RawPublicHelpTheta";
}

void epic::index::RawPublicHelpTheta::winningCoalitionsForPlayer(Game* g, bigInt big_wci[], bigFloat* big_total_wc) {
	// n_wc[x]: number of winning coalitions of weight x.
	ArrayOffset<lint::LargeNumber> n_wc(g->getWeightSum() + 1, g->getQuota());
	gCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());

	// helper: helper array for n_wc
	ArrayOffset<lint::LargeNumber> helper(g->getWeightSum() + 1, g->getQuota());
	gCalculator->allocInit_largeNumberArray(helper.getArrayPointer(), helper.getNumberOfElements());

	numberOfWinningCoalitionsPerWeight(g, n_wc);

	// wci[x]: number of winning coalitions for player x
	auto wci = new lint::LargeNumber[g->getNumberOfPlayers()];

	gCalculator->allocInit_largeNumberArray(wci, g->getNumberOfPlayers());

	/*
	 * Handle all players of weight > 0
	 */

	// This algorithm is nearly the same as it is used in the PowerIndexWithWinningCoalitionsAndSwingPlayers::numberOfTimesPlayerIsSwingPlayer() function
	for (longUInt i = 0; i < g->getNumberOfPlayers(); ++i) {
		longUInt wi = g->getWeights()[i];

		// skip dummy players if they are present in the array (!g->getFlagNullPlayerHandling())
		if ((i >= g->getNumberOfPlayers() - g->getNumberOfNullPlayers()) && g->getFlagNullPlayerHandling()) {
			continue;
		}

		longUInt m = std::max(g->getWeightSum() - wi, g->getQuota() - 1);
		for (longUInt k = g->getWeightSum(); k > m; --k) {
			gCalculator->assign(helper[k], n_wc[k]);
		}

		for (longUInt k = g->getWeightSum() - wi; k >= g->getQuota(); --k) {
			gCalculator->minus(helper[k], n_wc[k], helper[k + wi]);
		}

		// this is the only difference to the previously mentioned algorithm.
		// instead of looping over all coalitions the player would turn into losing ones, we step over all winning coalitions the player is a member of (up to weightsum).
		for (longUInt k = g->getQuota(); k <= g->getWeightSum(); ++k) {
			gCalculator->plusEqual(wci[i], helper[k]);
		}

		gCalculator->to_bigInt(&big_wci[i], wci[i]);
	}

	gCalculator->free_largeNumberArray(wci);
	delete[] wci;

	// If total_wc is wished for, provide it, otherwise delete it
	bool delete_total_wc;
	if (big_total_wc == nullptr) {
		delete_total_wc = true;
		big_total_wc = new bigFloat;
	} else {
		delete_total_wc = false;
	}

	// initialize total_wc
	// total_wc: the total number of winning coalitions (sum over all n_wc)
	lint::LargeNumber total_wc;
	gCalculator->allocInit_largeNumber(total_wc);

	numberOfWinningCoalitions(g, n_wc, total_wc);

	bigInt tmp;
	gCalculator->to_bigInt(&tmp, total_wc);
	*big_total_wc = tmp;

	gCalculator->free_largeNumber(total_wc);
	gCalculator->free_largeNumberArray(n_wc.getArrayPointer());
	gCalculator->free_largeNumberArray(helper.getArrayPointer());

	// Handle null/dummy players
	// If there are null players, naturally they start at index g->getNumberOfPlayers() - g->getNumberOfNullPlayers()
	for (longUInt i = g->getNumberOfPlayers() - g->getNumberOfNullPlayers(); i < g->getNumberOfPlayers(); ++i) {
		big_wci[i] = *big_total_wc / 2;
	}

	if (delete_total_wc) {
		delete big_total_wc;
	}
}
