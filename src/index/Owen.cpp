#include "index/Owen.h"

#include "Array.h"

#include <numeric>

epic::index::Owen::Owen(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: PowerIndexWithPrecoalitions(g) {
	bigInt max_value = approx->upperBound_swingPlayerPerCardinality();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);

	mCalculator->alloc_largeNumber(mTmp);
}

epic::index::Owen::~Owen() {
	mCalculator->free_largeNumber(mTmp);
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
		bigInt* factorial;
		{
			longUInt size = std::max(mMaxPartSize, mNbPart);
			factorial = new bigInt[size + 1];

			factorial[0] = 1;
			factorial[1] = 1;

			for (longUInt i = 2; i <= size; ++i) {
				factorial[i] = factorial[i - 1] * i;
			}
		}

		Array2dOffset<lint::LargeNumber> cc(mGame.getWeightSum() + 1, mNbPart, mGame.getQuota(), 0);
		mCalculator->allocInit_largeNumberArray(cc.getArrayPointer(), cc.getNumberOfElements());
		mCalculator->assign_one(cc(mGame.getWeightSum(), mNbPart - 1));
		generalizedBackwardCountingPerWeightCardinality(cc, mPartW, mNbPart);

		Array2dOffset<lint::LargeNumber> cw(mGame.getWeightSum() + 1, mNbPart, mGame.getQuota(), 0);
		mCalculator->allocInit_largeNumberArray(cw.getArrayPointer(), cw.getNumberOfElements());

		auto shapleysInternal = new bigInt[mGame.getNumberOfPlayers()]();

		Array2dOffset<lint::LargeNumber> cw2(mGame.getWeightSum() + 1, mMaxPartSize, mGame.getQuota(), 0);
		mCalculator->allocInit_largeNumberArray(cw2.getArrayPointer(), cw2.getNumberOfElements());

		Array2dOffset<lint::LargeNumber> cwi(mGame.getWeightSum() + 1, mMaxPartSize, mGame.getQuota(), 0);
		mCalculator->alloc_largeNumberArray(cwi.getArrayPointer(), cwi.getNumberOfElements());

		auto winternal = new longUInt[mMaxPartSize];

		bigInt factor;

		for (longUInt i = 0; i < mNbPart; ++i) {
			longUInt nbPlayersInPartI = mGame.getPrecoalitions()[i].size();

			coalitionsCardinalityContainingPlayerFromAbove(cw, cc, mNbPart, i, mPartW);

			// initialize winternal
			for (longUInt z = 0; z < nbPlayersInPartI; ++z) {
				winternal[z] = mGame.getWeights()[mGame.getPrecoalitions()[i][z]];
			}

			for (longUInt s = 0; s < mNbPart; ++s) {
				factor = factorial[s] * factorial[mNbPart - s - 1];

				if (nbPlayersInPartI > 1) {
					for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
						for (longUInt y = 0; y < nbPlayersInPartI - 1; ++y) {
							mCalculator->assign_zero(cw2(x, y));
						}
						mCalculator->assign(cw2(x, nbPlayersInPartI - 1), cw(x, s));
					}

					generalizedBackwardCountingPerWeightCardinality(cw2, winternal, nbPlayersInPartI);

					for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
						for (longUInt y = 0; y < nbPlayersInPartI; ++y) {
							mCalculator->assign(cwi(x, y), cw2(x, y));
						}
					}

					for (longUInt ii = 0; ii < nbPlayersInPartI; ++ii) {
						coalitionsCardinalityContainingPlayerFromAbove(cwi, cw2, nbPlayersInPartI, ii, winternal);
						updateInternalShapleyShubik(shapleysInternal, cwi, i, ii, winternal, factorial, factor);
					}
				} else {
					// shapleysInternal[mGame.getPrecoalitions()[i][0] - 1] += factor * sum(cw[q:tmp_min, s])
					mCalculator->assign_zero(mTmp);
					longUInt tmp_min = std::min(mGame.getQuota() + mPartW[i] - 1, mGame.getWeightSum());
					for (longUInt z = mGame.getQuota(); z <= tmp_min; ++z) {
						mCalculator->plusEqual(mTmp, cw(z, s));
					}
					mCalculator->to_bigInt(&mBigTmp, mTmp);
					shapleysInternal[mGame.getPrecoalitions()[i][0]] += mBigTmp * factor;
				}
			}
		}

		mCalculator->free_largeNumberArray(cc.getArrayPointer());
		mCalculator->free_largeNumberArray(cw.getArrayPointer());
		mCalculator->free_largeNumberArray(cw2.getArrayPointer());
		mCalculator->free_largeNumberArray(cwi.getArrayPointer());
		delete[] winternal;

		solution.resize(mGame.getNumberOfPlayers());
		{
			for (longUInt part = 0; part < mNbPart; ++part) {
				longUInt nbPlayersInParti = mGame.getPrecoalitions()[part].size();
				factor = factorial[nbPlayersInParti] * factorial[mNbPart];

				for (longUInt i = 0; i < nbPlayersInParti; ++i) {
					// solution[player] = shapleysInternal[player] / (factorial[nbPlayersInParti] * factorial[nbPartitions]);
					longUInt player = mGame.getPrecoalitions()[part][i];
					solution[player] = shapleysInternal[player];
					solution[player] /= factor;
				}
			}
		}

		delete[] factorial;
		delete[] shapleysInternal;
	}

	return solution;
}

std::string epic::index::Owen::getFullName() {
	return "Owen";
}

epic::longUInt epic::index::Owen::getMemoryRequirement() {
	bigInt memory = mNbPart * c_sizeof_longUInt; // mPartW
	longUInt max = std::max(mMaxPartSize, mNbPart);
	memory += max * GMPHelper::size_of_float(bigInt::factorial(max));												// factorial
	memory += (mGame.getWeightSum() + 1 - mGame.getQuota()) * mNbPart * mCalculator->getLargeNumberSize() * 2;		// cc, cw
	memory += mGame.getNumberOfPlayers() * GMPHelper::size_of_int(bigInt::factorial(mMaxPartSize));					// shapleyInternal (only very rough approximation
	memory += (mGame.getWeightSum() + 1 - mGame.getQuota()) * mMaxPartSize * mCalculator->getLargeNumberSize() * 2; // cw2, cwi
	memory += mMaxPartSize * c_sizeof_longUInt;																		// winternal

	memory /= cMemUnit_factor;
	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

void epic::index::Owen::updateInternalShapleyShubik(bigInt* internal_ssi, Array2dOffset<lint::LargeNumber>& cwi, longUInt precoalition, longUInt player, longUInt* weights, bigInt* factorial, bigInt& scale_factor) {
	longUInt n = mGame.getPrecoalitions()[precoalition].size();

	for (longUInt sinternal = 0; sinternal < n; ++sinternal) {
		mCalculator->assign_zero(mTmp);
		longUInt min = std::min(mGame.getQuota() + weights[player] - 1, mGame.getWeightSum());
		for (longUInt w = mGame.getQuota(); w <= min; ++w) {
			mCalculator->plusEqual(mTmp, cwi(w, sinternal));
		}

		mCalculator->to_bigInt(&mBigTmp, mTmp);
		internal_ssi[mGame.getPrecoalitions()[precoalition][player]] += (factorial[sinternal] * factorial[n - sinternal - 1]) * mBigTmp * scale_factor;
	}
}
