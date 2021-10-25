#include "index/Owen.h"

#include "Array.h"

#include <numeric>

// DEBUG
#include <iostream>

epic::index::Owen::Owen(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: ItfPowerIndex(g) {
	bigInt max_value = approx->upperBound_swingPlayerPerCardinality();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::multiplication, int_representation);

	mNbPart = mGame.getPrecoalitions().size();
	mPartW = new longUInt[mNbPart]();
	for (longUInt i = 0; i < mNbPart; ++i) {
		longUInt partSize = mGame.getPrecoalitions()[i].size();
		for (longUInt p = 0; p < partSize; ++p) {
			mPartW[i] += mGame.getWeights()[mGame.getPrecoalitions()[i][p]];
		}

		if (partSize > mMaxPartSize) {
			mMaxPartSize = partSize;
		}
	}

	mCalculator->alloc_largeNumber(mTmp);
	mCalculator->alloc_largeNumber(mTmp2);
}

epic::index::Owen::~Owen() {
	mCalculator->free_largeNumber(mTmp);
	mCalculator->free_largeNumber(mTmp2);
	delete[] mPartW;
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::Owen::calculate() {
	std::vector<bigFloat> solution;
	if (mGame.getNumberOfVetoPlayers() > 0) {
		solution.resize(mGame.getNumberOfPlayers());
		for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
			solution[i] = mGame.getVetoPlayerVector()[i] ? 1 : 0;
		}
	} else {
		lint::LargeNumber* factorial;
		{
			longUInt size = std::max(mMaxPartSize, mNbPart);
			factorial = new lint::LargeNumber[size + 1];
			mCalculator->alloc_largeNumberArray(factorial, size + 1);

			mCalculator->assign_one(factorial[0]);
			mCalculator->assign_one(factorial[1]);
			for (longUInt i = 2; i <= size; ++i) {
				mCalculator->mul(factorial[i], factorial[i - 1], i);
			}
		}

		Array2dOffset<lint::LargeNumber> cc(mGame.getWeightSum() + 1, mNbPart, mGame.getQuota(), 0);
		mCalculator->allocInit_largeNumberArray(cc.getArrayPointer(), cc.getNumberOfElements());
		backwardCountingPerWeightAndCardinality(cc);

		Array2dOffset<lint::LargeNumber> cw(mGame.getWeightSum() + 1, mNbPart, mGame.getQuota(), 0);
		mCalculator->allocInit_largeNumberArray(cw.getArrayPointer(), cw.getNumberOfElements());

		auto shapleysInternal = new lint::LargeNumber[mGame.getNumberOfPlayers()];
		mCalculator->allocInit_largeNumberArray(shapleysInternal, mGame.getNumberOfPlayers());

		Array2dOffset<lint::LargeNumber> cw2(mGame.getWeightSum() + 1, mMaxPartSize, mGame.getQuota(), 0);
		mCalculator->allocInit_largeNumberArray(cw2.getArrayPointer(), cw2.getNumberOfElements());

		Array2dOffset<lint::LargeNumber> cwi(mGame.getWeightSum() + 1, mMaxPartSize, mGame.getQuota(), 0);
		mCalculator->alloc_largeNumberArray(cwi.getArrayPointer(), cwi.getNumberOfElements());

		auto winternal = new longUInt[mMaxPartSize];

		lint::LargeNumber factor;
		mCalculator->alloc_largeNumber(factor);

		for (longUInt i = 0; i < mNbPart; ++i) {
			longUInt nbPlayersInPartI = mGame.getPrecoalitions()[i].size();

			countCoalitionsPerPlayer(cw, cc, mNbPart, i, mPartW);

			// initialize winternal
			for (longUInt z = 0; z < nbPlayersInPartI; ++z) {
				winternal[z] = mGame.getWeights()[mGame.getPrecoalitions()[i][z]];
			}

			for (longUInt s = 0; s < mNbPart - 1; ++s) {
				mCalculator->mul(factor, factorial[s + 1], factorial[mNbPart - s - 2]);

				if (nbPlayersInPartI > 1) {
					for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
						for (longUInt y = 0; y < nbPlayersInPartI - 1; ++y) {
							mCalculator->assign_zero(cw2(x, y));
						}
						mCalculator->assign(cw2(x, nbPlayersInPartI - 1), cw(x, s + 1));
					}

					updateNumberOfWinningCoalitionsPerWeightAndCardinality(cw2, i, winternal);

					for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
						for (longUInt y = 0; y < nbPlayersInPartI; ++y) {
							mCalculator->assign(cwi(x, y), cw2(x, y));
						}
					}

					for (longUInt ii = 0; ii < nbPlayersInPartI; ++ii) {
						countCoalitionsPerPlayer(cwi, cw2, nbPlayersInPartI, ii, winternal);
						updateInternalShapleyShubik(shapleysInternal, cwi, i, ii, winternal, factorial, factor);
					}
				} else {
					// shapleysInternal[mGame.getPrecoalitions()[i][0] - 1] += factor * sum(cw[q:tmp_min, s+1])
					mCalculator->assign_zero(mTmp2);
					longUInt tmp_min = std::min(mGame.getQuota() + mPartW[i] - 1, mGame.getWeightSum());
					for (longUInt z = mGame.getQuota(); z <= tmp_min; ++z) {
						mCalculator->plusEqual(mTmp2, cw(z, s + 1));
					}
					mCalculator->mul(mTmp, factor, mTmp2);
					mCalculator->plusEqual(shapleysInternal[mGame.getPrecoalitions()[i][0]], mTmp);
				}
			}
		}

		mCalculator->free_largeNumber(factor);

		mCalculator->free_largeNumberArray(cc.getArrayPointer());
		mCalculator->free_largeNumberArray(cw.getArrayPointer());
		mCalculator->free_largeNumberArray(cw2.getArrayPointer());
		mCalculator->free_largeNumberArray(cwi.getArrayPointer());
		delete[] winternal;

		solution.resize(mGame.getNumberOfPlayers());
		{
			bigInt shapleys_internal;
			bigInt factor;
			bigInt factorial_nbPartitions;
			mCalculator->to_bigInt(&factorial_nbPartitions, factorial[mNbPart]);

			for (longUInt part = 0; part < mNbPart; ++part) {
				longUInt nbPlayersInParti = mGame.getPrecoalitions()[part].size();
				mCalculator->to_bigInt(&factor, factorial[nbPlayersInParti]);
				factor *= factorial_nbPartitions;

				for (longUInt i = 0; i < nbPlayersInParti; ++i) {
					// solution[player] = shapleysInternal[player] / (factorial[nbPlayersInParti] * factorial[nbPartitions]);
					longUInt player = mGame.getPrecoalitions()[part][i];
					mCalculator->to_bigInt(&shapleys_internal, shapleysInternal[player]);
					solution[player] = bigFloat(shapleys_internal) / factor;
				}
			}
		}

		mCalculator->free_largeNumberArray(factorial);
		delete[] factorial;

		mCalculator->free_largeNumberArray(shapleysInternal);
		delete[] shapleysInternal;
	}


	// DEBUG
	std::cout << "owenIndices:" << std::endl;
	for (longUInt x = 0; x < mGame.getNumberOfPlayers(); ++x) {
		std::cout << "[" << x << "]\t" << solution[x] << std::endl;
	}

	return solution;
}

std::string epic::index::Owen::getFullName() {
	return "Owen";
}

epic::longUInt epic::index::Owen::getMemoryRequirement() {
	bigInt memory = mNbPart * c_sizeof_longUInt; // mPartW
	memory += std::max(mMaxPartSize, mNbPart) * mCalculator->getLargeNumberSize(); // factorial
	memory += (mGame.getWeightSum() + 1 - mGame.getQuota()) * mNbPart * mCalculator->getLargeNumberSize() * 2; // cc, cw
	memory += mGame.getNumberOfPlayers() * mCalculator->getLargeNumberSize(); // shapleysInternal
	memory += (mGame.getWeightSum() + 1 - mGame.getQuota()) * mMaxPartSize * mCalculator->getLargeNumberSize() * 2; // cw2, cwi
	memory += mMaxPartSize * c_sizeof_longUInt; // winternal

	memory /= cMemUnit_factor;
	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

// ! c must be allocated and zero-initialized in the range [quota, capitalC] x [0, nbPartitions - 1]
void epic::index::Owen::backwardCountingPerWeightAndCardinality(Array2dOffset<lint::LargeNumber>& c) {
	longUInt n = mGame.getPrecoalitions().size();

	mCalculator->assign_one(c(mGame.getWeightSum(), n -1));
	for (longUInt i = 0; i < n; ++i) {
		for (longUInt x = mGame.getQuota() + mPartW[i]; x <= mGame.getWeightSum(); ++x) {
			for (longUInt m = 1; m < n; ++m) {
				mCalculator->plusEqual(c(x - mPartW[i], m - 1), c(x, m));
			}
		}
	}
}

void epic::index::Owen::countCoalitionsPerPlayer(Array2dOffset<lint::LargeNumber>& cw, Array2dOffset<lint::LargeNumber>& cc, longUInt n_player, longUInt player, longUInt* weights) {
	for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
		for (longUInt y = 0; y < n_player; ++y) {
			mCalculator->assign(cw(x, y), cc(x, y));
		}
	}

	for (longUInt x = mGame.getWeightSum() - weights[player]; x >= mGame.getQuota(); --x) {
		for (longUInt s = 1; s < n_player; ++s) {
			mCalculator->minus(cw(x, n_player - s - 1), cc(x, n_player - s - 1), cw(x + weights[player], n_player - s));
		}
	}
}

void epic::index::Owen::updateNumberOfWinningCoalitionsPerWeightAndCardinality(Array2dOffset<lint::LargeNumber>& cw2, longUInt precoalition, longUInt* weights) {
	longUInt n = mGame.getPrecoalitions()[precoalition].size();

	for (longUInt i = 0; i < n; ++i) {
		for (longUInt x = mGame.getQuota() + weights[i]; x <= mGame.getWeightSum(); ++x) {
			for (longUInt m = n - 1; m > 0; --m) {
				mCalculator->plus(cw2(x - weights[i], m - 1), cw2(x, m), cw2(x - weights[i], m - 1));
			}
		}
	}
}

void epic::index::Owen::updateInternalShapleyShubik(lint::LargeNumber* internal_ssi, Array2dOffset<lint::LargeNumber>& cwi, longUInt precoalition, longUInt player, longUInt* weights, lint::LargeNumber* factorial, lint::LargeNumber& scale_factor) {
	longUInt n = mGame.getPrecoalitions()[precoalition].size();

	for (longUInt sinternal = 0; sinternal < n; ++sinternal) {
		mCalculator->mul(mTmp, factorial[sinternal], factorial[n - sinternal - 1]);

		mCalculator->assign_zero(mTmp2);
		for (longUInt w = mGame.getQuota(); w <= mGame.getQuota() + weights[player] - 1; ++w) {
			mCalculator->plusEqual(mTmp2, cwi(w, sinternal));
		}

		// shapleysInternal[mGame.getPrecoalitions()[i][ii] - 1] += tmp * tmp_sum * factor
		mCalculator->mul(mTmp, mTmp, mTmp2);
		mCalculator->mulEqual(mTmp, scale_factor);
		mCalculator->plusEqual(internal_ssi[mGame.getPrecoalitions()[precoalition][player]], mTmp);
	}
}
