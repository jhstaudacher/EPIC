#include "index/RawDeeganPackel.h"

#include "lint/GlobalCalculator.h"

#include <iostream>

epic::index::RawDeeganPackel::RawDeeganPackel() : ItfPowerIndex() {}

std::vector<epic::bigFloat> epic::index::RawDeeganPackel::calculate(Game* g) {
	//note: the players are sorted by weight in descending ordering!

	// formula f(n,q) => f[y][x] represents number of coalitions with cardinality y and weight x;
	Array2d<lint::LargeNumber> f(g->getNumberOfNonZeroPlayers(), g->getQuota() + 1, true);
	gCalculator->allocInit_largeNumberArray(f.getArrayPointer(), f.getNumberOfElements());

	// initialise the empty coalition with 1, since we know, that the empty coalition is always losing
	gCalculator->assign_one(f(0, 0));

	/*
	 *	compute formula f for each player but hold f[n-1][k][x] in array (we start at zero-> n-2) only
	 */
	for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers() - 1; ++i) {
		//wi: weight of player i
		longUInt wi = g->getWeights()[i];
		for (longUInt k = i; k <= i; --k) {
			for (longUInt x = g->getQuota(); x > wi; --x) {
				gCalculator->plusEqual(f(k + 1, x - 1), f(k, x - wi - 1));
			}
		}
	}

	/*
	 * formula b(n,q) => b[y][x] represents how many coalitions S there are,
	 * such that w(S) >= weight x and player i element of S
	 * and d(w(S)) < x; d(w(S)) means the weight of coalition without player i
	*/
	Array2d<double> b(g->getNumberOfNonZeroPlayers(), g->getQuota() + 1, true);

	bigInt big_tmp;

	// set solution for each player i
	// solution[i] = DP[i] = unscaled-DP[i] / total number of minimal winning coalitions
	std::vector<bigFloat> solution(g->getNumberOfPlayers());

	// compute smallest player
	{
		longUInt w_last = g->getWeights()[g->getNumberOfNonZeroPlayers() - 1]; // w_last cant be greater or equal than the quota since the quota must be at least 50% and the games must have more than one player!
		double dp_unscaled = 0;

		// initialize b[n-1,,] for computation of DP for player n-1
		for (longUInt k = 0; k < g->getNumberOfNonZeroPlayers(); ++k) {
			double tmp = 1 / static_cast<double>(k + 1);
			for (longUInt x = 1; x < w_last + 1; x++) {
				b(k, x) = tmp;
			}

			for (longUInt x = 0; x <= g->getQuota(); x++) {
				gCalculator->to_bigInt(&big_tmp, f(k, x));
				dp_unscaled += b(k, g->getQuota() - x) * big_tmp.get_d();
			}
		}

		solution[g->getNumberOfNonZeroPlayers() - 1] = bigFloat(dp_unscaled);
	}

	// compute for each player from n-1 to zero
	for (longUInt i = g->getNumberOfNonZeroPlayers() - 2; i < g->getNumberOfNonZeroPlayers(); --i) {
		longUInt wi = g->getWeights()[i];

		// if weight of player <= quota
		if (wi <= g->getQuota()) {
			// first iteration: only initialise b[n-1,k,x] ->further down
			// other iteration: compute b[i,,] from b[i+1,,]
			for (longUInt k = 0; k <= i; ++k) {
				for (longUInt x = g->getQuota() + 1; x > wi; --x) {
					b(k, x - 1) = b(k, x - wi + g->getWeights()[i + 1] - 1) + b(k + 1, x - wi - 1); //(#increment b)
				}
			}
		} else {
			// handle veto player
			for (longUInt k = 0; k <= i; ++k) {
				b(k, g->getQuota()) = 1;
			}
		}

		// compute f[i,,] from f[i+1,,]
		for (longUInt k = 0; k <= i; ++k) {
			for (longUInt x = wi; x < g->getQuota(); ++x) {
				gCalculator->minus(f(k + 1, x), f(k + 1, x), f(k, x - wi)); //(#decrement f)
			}
		}

		// specify minimum for formula b
		longUInt minimum = (g->getQuota() <= wi) ? g->getQuota() : wi + 1;

		// reinitialize some values of b[i,,] for player i
		// if weight of player <= quota
		for (longUInt k = 0; k <= i; ++k) {
			double tmp = 1 / static_cast<double>(k + 1);
			for (longUInt x = g->getWeights()[i + 1] + 1; x < minimum; ++x) {
				b(k, x) = tmp;
			}
		}

		// compute DP for player i
		// for player i: f(i-1,k,x)*b(i,k,q-x)
		double dp_unscaled = 0;
		for (longUInt k = 0; k <= i; ++k) {
			for (longUInt x = 0; x <= g->getQuota(); ++x) {
				gCalculator->to_bigInt(&big_tmp, f(k, x));
				dp_unscaled += b(k, g->getQuota() - x) * big_tmp.get_d();
			}
		}

		solution[i] = bigFloat(dp_unscaled);
	}

	// handle player of weight zero
	for (longUInt i = g->getNumberOfNonZeroPlayers(); i < g->getNumberOfPlayers(); ++i) {
		solution[i] = 0;
	}

	gCalculator->free_largeNumberArray(f.getArrayPointer());

	return solution;
}

std::string epic::index::RawDeeganPackel::getFullName() {
	return "RawDeeganPackel";
}

epic::longUInt epic::index::RawDeeganPackel::getMemoryRequirement(Game* g) {
	bigInt memory = g->getNumberOfNonZeroPlayers() * (g->getQuota() + 1) * gCalculator->getLargeNumberSize(); //f
	memory += g->getNumberOfNonZeroPlayers() * (g->getQuota() + 1) * c_sizeof_double;						  // b
	// the memory used by f and b is always larger than the memory needed for the numberOfMinimalWinningCoalitions() function, which is (g->getQuota() + 1 + g->getNumberOfPlayers) * gCalculator->getLargeNumberSize() for wc and sum_minwc.
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

epic::bigInt epic::index::RawDeeganPackel::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_numberOfWinningCoalitionsPerWeight();
}

epic::lint::Operation epic::index::RawDeeganPackel::getOperationRequirement() {
	return lint::Operation::addition;
}
