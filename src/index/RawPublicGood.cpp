#include "index/RawPublicGood.h"

#include "Array.h"
#include "lint/GlobalCalculator.h"

#include <iostream>

epic::index::RawPublicGood::RawPublicGood()
	: ItfPowerIndex() {}

std::vector<epic::bigFloat> epic::index::RawPublicGood::calculate(Game* g) {
	auto mwc = new lint::LargeNumber[g->getNumberOfPlayers()];
	gCalculator->allocInit_largeNumberArray(mwc, g->getNumberOfPlayers());
	calculate(g, mwc);

	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	bigInt big_mwc;

	for (longUInt i = 0; i < g->getNumberOfPlayers(); ++i) {
		gCalculator->to_bigInt(&big_mwc, mwc[i]);
		solution[i] = big_mwc;
	}

	gCalculator->free_largeNumberArray(mwc);
	delete[] mwc;

	return solution;
}

void epic::index::RawPublicGood::calculate(Game* g, lint::LargeNumber* solution) {
	calculateMinimalWinningCoalitionsPerPlayer(g, solution);

	for (longUInt i = g->getNumberOfNonZeroPlayers(); i < g->getNumberOfPlayers(); ++i) {
		gCalculator->assign_zero(solution[i]);
	}
}

std::string epic::index::RawPublicGood::getFullName() {
	return "RawPublicGood";
}

epic::longUInt epic::index::RawPublicGood::getMemoryRequirement(Game* g) {
	bigInt memory = g->getNumberOfNonZeroPlayers() * gCalculator->getLargeNumberSize(); // mwc

	// The memory needed inside the calculateMinimalWinningCoalitionsPerPlayer() function
	memory += g->getQuota() * gCalculator->getLargeNumberSize();		   // f
	memory += (g->getQuota() + 1) * gCalculator->getLargeNumberSize() * 2; // b, b_helper
	// don't add solution vector since it gets allocated after the previous arrays are already freed.
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

epic::bigInt epic::index::RawPublicGood::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_totalNumberOfSwingPlayer();
}

epic::lint::Operation epic::index::RawPublicGood::getOperationRequirement() {
	return lint::Operation::multiplication;
}

void epic::index::RawPublicGood::calculateFVector(Game* g, lint::LargeNumber f[]) {
	// Initialize the empty coalition with 1 since we know, that the empty coalition is always losing.
	gCalculator->assign_one(f[0]);

	for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
		longUInt wi = g->getWeights()[i];

		// Players are veto players
		if (wi < g->getQuota()) {
			for (longUInt k = g->getQuota() - wi - 1; k < g->getQuota(); --k) {
				gCalculator->plusEqual(f[k + wi], f[k]);
			}
		}
	}
}

void epic::index::RawPublicGood::calculateMinimalWinningCoalitionsPerPlayer(Game* g, lint::LargeNumber mwc[]) {
	auto f = new lint::LargeNumber[g->getQuota()];
	gCalculator->allocInit_largeNumberArray(f, g->getQuota());
	calculateFVector(g, f);

	auto b = new lint::LargeNumber[g->getQuota() + 1];
	gCalculator->allocInit_largeNumberArray(b, g->getQuota() + 1);

	lint::LargeNumber tmp;
	gCalculator->alloc_largeNumber(tmp);

	// Last Player
	{
		longUInt last_player = g->getNumberOfNonZeroPlayers() - 1;
		longUInt w_last = g->getWeights()[last_player];

		if (w_last < g->getQuota()) {
			for (longUInt i = 0; i <= w_last; ++i) {
				gCalculator->assign_one(b[i]);
			}

			for (longUInt i = w_last; i < g->getQuota(); ++i) {
				gCalculator->minusEqual(f[i], f[i - w_last]);
			}
		}

		for (longUInt i = 0; i < g->getQuota(); ++i) {
			gCalculator->mul(tmp, f[i], b[g->getQuota() - i]);
			gCalculator->plusEqual(mwc[last_player], tmp);
		}
	}

	auto b_helper = new lint::LargeNumber[g->getQuota() + 1];
	gCalculator->allocInit_largeNumberArray(b_helper, g->getQuota() + 1);

	// For each player (except the last one)
	for (longUInt i = g->getNumberOfNonZeroPlayers() - 2; i < g->getNumberOfNonZeroPlayers(); --i) {
		longUInt wi = g->getWeights()[i];
		longUInt wi_plus1 = g->getWeights()[i + 1];

		for (longUInt k = 0; k <= g->getQuota(); ++k) {
			gCalculator->assign(b_helper[k], b[k]);
		}

		for (longUInt k = 0; k <= wi && k <= g->getQuota(); ++k) {
			gCalculator->assign_one(b[k]);
		}

		if (wi < g->getQuota()) {
			for (longUInt k = wi; k < g->getQuota(); ++k) {
				gCalculator->minusEqual(f[k], f[k - wi]);
			}

			for (longUInt k = wi + 1; k <= g->getQuota(); ++k) {
				gCalculator->plus(b[k], b_helper[k - wi + wi_plus1], b_helper[k - wi]);
			}
		}

		for (longUInt k = 0; k < g->getQuota(); ++k) {
			gCalculator->mul(tmp, f[k], b[g->getQuota() - k]);
			gCalculator->plusEqual(mwc[i], tmp);
		}
	}

	// Handling veto players: As they are ignored in the above calculation, we can just look through the player base here and add 1 to all of them, since each one veto player is ONLY ever in 1 minimal winning coalition
	for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); i++) {
		// If player is veto player:
		if (g->getVetoPlayerVector()[i]) {
			gCalculator->assign(mwc[i], 1ul);
		}
	}

	/**
	 * DELETE
	 */
	gCalculator->free_largeNumberArray(b_helper);
	delete[] b_helper;

	gCalculator->free_largeNumberArray(b);
	delete[] b;

	gCalculator->free_largeNumberArray(f);
	delete[] f;

	gCalculator->free_largeNumber(tmp);
}
