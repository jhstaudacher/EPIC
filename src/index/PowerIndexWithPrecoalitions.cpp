#include "index/PowerIndexWithPrecoalitions.h"

epic::index::PowerIndexWithPrecoalitions::PowerIndexWithPrecoalitions(Game& g) : ItfPowerIndex(g) {
	mNbPart = mGame.getPrecoalitions().size();
	mPartW = new longUInt[mNbPart]();
	mMaxPartSize = 0;
	for (longUInt i = 0; i < mNbPart; ++i) {
		longUInt partSize = mGame.getPrecoalitions()[i].size();
		for (longUInt p = 0; p < partSize; ++p) {
			mPartW[i] += mGame.getWeights()[mGame.getPrecoalitions()[i][p]];
		}

		if (partSize > mMaxPartSize) {
			mMaxPartSize = partSize;
		}
	}
}

epic::index::PowerIndexWithPrecoalitions::~PowerIndexWithPrecoalitions() {
	delete[] mPartW;
}

void epic::index::PowerIndexWithPrecoalitions::coalitionsContainingPlayerFromAbove(ArrayOffset<lint::LargeNumber>& cw, ArrayOffset<lint::LargeNumber>& cc, longUInt wi) {
    for (longUInt i = mGame.getQuota(); i <= mGame.getWeightSum(); ++i) {
		mCalculator->assign(cw[i], cc[i]);
	}

	for (longUInt i = mGame.getWeightSum() - wi; i >= mGame.getQuota(); --i) {
		mCalculator->minus(cw[i], cc[i], cw[i + wi]);
	}
}

void epic::index::PowerIndexWithPrecoalitions::generalizedBackwardCountingPerWeight(ArrayOffset<lint::LargeNumber>& c, longUInt* weights, longUInt n) {
	for (longUInt i = 0; i < n; ++i) {
        for (longUInt x = mGame.getQuota() + weights[i]; x <= mGame.getWeightSum(); ++x) {
            mCalculator->plusEqual(c[x - weights[i]], c[x]);
        }
	}
}

void epic::index::PowerIndexWithPrecoalitions::coalitionsCardinalityContainingPlayerFromAbove(Array2dOffset<lint::LargeNumber>& cw, Array2dOffset<lint::LargeNumber>& cc, longUInt n, longUInt p, longUInt* weights) {
	for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
		for (longUInt y = 0; y < n; ++y) {
			mCalculator->assign(cw(x, y), cc(x, y));
		}
	}

	for (longUInt x = mGame.getWeightSum() - weights[p]; x >= mGame.getQuota(); --x) {
		for (longUInt s = 1; s < n; ++s) {
			mCalculator->minus(cw(x, n - s - 1), cc(x, n - s - 1), cw(x + weights[p], n - s));
		}
	}
}

void epic::index::PowerIndexWithPrecoalitions::generalizedBackwardCountingPerWeightCardinality(Array2dOffset<lint::LargeNumber>& cc, longUInt* weights, longUInt n) {
	for (longUInt i = 0; i < n; ++i) {
		for (longUInt x = mGame.getQuota() + weights[i]; x <= mGame.getWeightSum(); ++x) {
			for (longUInt m = 1; m < n; ++m) {
				mCalculator->plusEqual(cc(x - weights[i], m - 1), cc(x, m));
			}
		}
	}
}
