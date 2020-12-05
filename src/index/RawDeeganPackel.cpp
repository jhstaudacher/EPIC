#include "index/RawDeeganPackel.h"

#include <iostream>

epic::index::RawDeeganPackel::RawDeeganPackel(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: ItfPowerIndex(g) {
	bigInt max_value = approx->upperBound_numberOfWinningCoalitionsPerWeight();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);
}

epic::index::RawDeeganPackel::~RawDeeganPackel() {
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::RawDeeganPackel::calculate() {
	//note: the players are sorted by weight in descending ordering!

	// formula f(n,q) => f[y][x] represents number of coalitions with cardinality y and weight x;
	Array2d<lint::LargeNumber> f(mNonZeroPlayerCount, mGame.getQuota() + 1, true);
	mCalculator->allocInit_largeNumberArray(f.getArrayPointer(), f.getNumberOfElements());

	// initialise the empty coalition with 1, since we know, that the empty coalition is always losing
	mCalculator->assign_one(f(0, 0));

	/*
	 *	compute formula f for each player but hold f[n-1][k][x] in array (we start at zero-> n-2) only
	 */
	for (longUInt i = 0; i < mNonZeroPlayerCount - 1; ++i) {
		//wi: weight of player i
		longUInt wi = mGame.getWeights()[i];
		for (longUInt k = i; k <= i; --k) {
			for (longUInt x = mGame.getQuota(); x > wi; --x) {
				mCalculator->plusEqual(f(k + 1, x - 1), f(k, x - wi - 1));
			}
		}
	}

	/*
	 * formula b(n,q) => b[y][x] represents how many coalitions S there are,
	 * such that w(S) >= weight x and player i element of S
	 * and d(w(S)) < x; d(w(S)) means the weight of coalition without player i
	*/
	Array2d<double> b(mNonZeroPlayerCount, mGame.getQuota() + 1, true);

	bigInt big_tmp;

	// set solution for each player i
	// solution[i] = DP[i] = unscaled-DP[i] / total number of minimal winning coalitions
	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());

	// compute smallest player
	{
		longUInt w_last = mGame.getWeights()[mNonZeroPlayerCount - 1]; // w_last cant be greater or equal than the quota since the quota must be at least 50% and the games must have more than one player!
		double dp_unscaled = 0;

		// initialize b[n-1,,] for computation of DP for player n-1
		for (longUInt k = 0; k < mNonZeroPlayerCount; ++k) {
			double tmp = 1 / static_cast<double>(k + 1);
			for (longUInt x = 1; x < w_last + 1; x++) {
				b(k, x) = tmp;
			}

			for (longUInt x = 0; x <= mGame.getQuota(); x++) {
				mCalculator->to_bigInt(&big_tmp, f(k, x));
				dp_unscaled += b(k, mGame.getQuota() - x) * big_tmp.get_d();
			}
		}

		solution[mNonZeroPlayerCount - 1] = bigFloat(dp_unscaled);
	}

	// compute for each player from n-1 to zero
	for (longUInt i = mNonZeroPlayerCount - 2; i < mNonZeroPlayerCount; --i) {
		longUInt wi = mGame.getWeights()[i];

		// if weight of player <= quota
		if (wi <= mGame.getQuota()) {
			// first iteration: only initialise b[n-1,k,x] ->further down
			// other iteration: compute b[i,,] from b[i+1,,]
			for (longUInt k = 0; k <= i; ++k) {
				for (longUInt x = mGame.getQuota() + 1; x > wi; --x) {
					b(k, x - 1) = b(k, x - wi + mGame.getWeights()[i + 1] - 1) + b(k + 1, x - wi - 1); //(#increment b)
				}
			}
		} else {
			// handle veto player
			for (longUInt k = 0; k <= i; ++k) {
				b(k, mGame.getQuota()) = 1;
			}
		}

		// compute f[i,,] from f[i+1,,]
		for (longUInt k = 0; k <= i; ++k) {
			for (longUInt x = wi; x < mGame.getQuota(); ++x) {
				mCalculator->minus(f(k + 1, x), f(k + 1, x), f(k, x - wi)); //(#decrement f)
			}
		}

		// specify minimum for formula b
		longUInt minimum = (mGame.getQuota() <= wi) ? mGame.getQuota() : wi + 1;

		// reinitialize some values of b[i,,] for player i
		// if weight of player <= quota
		for (longUInt k = 0; k <= i; ++k) {
			double tmp = 1 / static_cast<double>(k + 1);
			for (longUInt x = mGame.getWeights()[i + 1] + 1; x < minimum; ++x) {
				b(k, x) = tmp;
			}
		}

		// compute DP for player i
		// for player i: f(i-1,k,x)*b(i,k,q-x)
		double dp_unscaled = 0;
		for (longUInt k = 0; k <= i; ++k) {
			for (longUInt x = 0; x <= mGame.getQuota(); ++x) {
				mCalculator->to_bigInt(&big_tmp, f(k, x));
				dp_unscaled += b(k, mGame.getQuota() - x) * big_tmp.get_d();
			}
		}

		solution[i] = bigFloat(dp_unscaled);
	}

	// handle player of weight zero
	for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
		solution[i] = 0;
	}

	mCalculator->free_largeNumberArray(f.getArrayPointer());

	return solution;
}

std::string epic::index::RawDeeganPackel::getFullName() {
	return "RawDeeganPackel";
}

epic::longUInt epic::index::RawDeeganPackel::getMemoryRequirement() {
	bigInt memory = mNonZeroPlayerCount * (mGame.getQuota() + 1) * mCalculator->getLargeNumberSize(); //f
	memory += mNonZeroPlayerCount * (mGame.getQuota() + 1) * c_sizeof_double;						   // b

	// the memory used by f and b is always larger than the memory needed for the numberOfMinimalWinningCoalitions() function, which is (mGame.getQuota() + 1 + mGame.getNumberOfPlayers) * mCalculator->getLargeNumberSize() for wc and sum_minwc.

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}
