#include "index/OwenBelow.h"

#include "../include/FastUpperBoundApproximation.h"
#include "../include/index/ShapleyShubik.h"
#include "Array.h"
#include "Game.h"

#include <Logging.h>
#include <numeric>

epic::index::OwenBelow::OwenBelow(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: PowerIndexWithPrecoalitions(g) {
	bigInt max_value = approx->upperBound_swingPlayerPerCardinality();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);

	mCalculator->alloc_largeNumber(mTmp);
}

epic::index::OwenBelow::~OwenBelow() {
	mCalculator->free_largeNumber(mTmp);
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::OwenBelow::calculate() {
	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	longUInt quota = mGame.getQuota();

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

	Array2d<lint::LargeNumber> cc;
	cc.alloc(quota, mNbPart);

	mCalculator->allocInit_largeNumberArray(cc.getArrayPointer(), cc.getNumberOfElements());
	generalizedForwardCountingPerWeightCardinality(cc, mPartW, mNbPart, (longUInt)1, (longUInt)0, false);

	Array2d<lint::LargeNumber> cwo;
	cwo.alloc(quota, mNbPart);
	mCalculator->allocInit_largeNumberArray(cwo.getArrayPointer(), cwo.getNumberOfElements());
	auto shapleysInternal = new bigInt[mGame.getNumberOfPlayers()]();

	Array2d<lint::LargeNumber> cwo2;
	cwo2.alloc(quota, mMaxPartSize + 1);
	mCalculator->allocInit_largeNumberArray(cwo2.getArrayPointer(), cwo2.getNumberOfElements());

	Array2d<lint::LargeNumber> cwoi;
	cwoi.alloc(quota, mMaxPartSize + 1);
	mCalculator->allocInit_largeNumberArray(cwoi.getArrayPointer(), cwoi.getNumberOfElements());

	auto winternal = new longUInt[mMaxPartSize];
	bigInt factor;

	for (longUInt i = 0; i < mNbPart; ++i) {
		longUInt nbPlayersInPartI = mGame.getPrecoalitions()[i].size();
		// initialize winternal
		for (longUInt z = 0; z < nbPlayersInPartI; ++z) {
			winternal[z] = mGame.getWeights()[mGame.getPrecoalitions()[i][z]];
		}

		coalitionsCardinalityWithoutPlayerFromBelow(cc, cwo, mNbPart - 1, i, mPartW, 0, -1);
		for (int s = 0; s < ((int)mNbPart); ++s) {
			s = s - 1;
			factor = factorial[s + 1] * factorial[mNbPart - s - 2];
			if (nbPlayersInPartI > 1) {
				for (longUInt x = 0; x < quota; ++x) {
					for (longUInt y = 0; y < (mMaxPartSize + 1); ++y) {
						mCalculator->assign_zero(cwo2(x, y));
					}
					if (s >= 0) {
						mCalculator->assign(cwo2(x, 0), cwo(x, s));
					}
				}
				if (s == -1) {
					generalizedForwardCountingPerWeightCardinality(cwo2, winternal, nbPlayersInPartI, (longUInt)1, (longUInt)1, true);
				} else {
					generalizedForwardCountingPerWeightCardinality(cwo2, winternal, nbPlayersInPartI, (longUInt)1, (longUInt)1, false);
				}
				for (longUInt x = 0; x < quota; ++x) {
					for (longUInt y = 0; y < (mMaxPartSize + 1); ++y) {
						mCalculator->assign(cwoi(x, y), cwo2(x, y));
					}
				}
				for (longUInt ii = 0; ii < nbPlayersInPartI; ++ii) {
					coalitionsCardinalityWithoutPlayerFromBelow(cwo2, cwoi, nbPlayersInPartI, ii, winternal, (longUInt)1, s);
					updateInternalShapleyShubik(shapleysInternal, cwoi, i, ii, winternal, factorial, factor, s);
				}
			} else {
				mCalculator->assign_zero(mTmp);
				longUInt max = 0;
				if (quota > mPartW[i]) {
					max = quota - mPartW[i] - 1;
				}
				if (s >= 0) {
					for (longUInt z = max; z < quota - 1; ++z) {
						mCalculator->plusEqual(mTmp, cwo(z, s));
					}
				}

				if (s == -1) {
					if (mPartW[i] >= quota) {
						mCalculator->increment(mTmp);
					}
				}
				mCalculator->to_bigInt(&mBigTmp, mTmp);
				shapleysInternal[mGame.getPrecoalitions()[i][0]] += mBigTmp * factorial[s + 1] * factorial[mNbPart - s - 2];
			}
			s++;
		}
	}
	mCalculator->free_largeNumberArray(cc.getArrayPointer());
	mCalculator->free_largeNumberArray(cwo.getArrayPointer());
	mCalculator->free_largeNumberArray(cwo2.getArrayPointer());
	mCalculator->free_largeNumberArray(cwoi.getArrayPointer());
	delete[] winternal;
	solution.resize(mGame.getNumberOfPlayers());
	{
		for (longUInt part = 0; part < mNbPart; ++part) {
			longUInt nbPlayersInParti = mGame.getPrecoalitions()[part].size();
			factor = factorial[nbPlayersInParti] * factorial[mNbPart];

			for (longUInt i = 0; i < nbPlayersInParti; ++i) {
				longUInt player = mGame.getPrecoalitions()[part][i];
				solution[player] = shapleysInternal[player];
				solution[player] /= factor;
			}
		}
	}
	delete[] factorial;
	delete[] shapleysInternal;

	return solution;
}

std::string epic::index::OwenBelow::getFullName() {
	return "OwenBelow";
}

epic::longUInt epic::index::OwenBelow::getMemoryRequirement() {
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

void epic::index::OwenBelow::updateInternalShapleyShubik(bigInt* internal_ssi, Array2d<lint::LargeNumber>& cwoi, longUInt precoalition, longUInt player, longUInt* weights, bigInt* factorial, bigInt& scale_factor, int s) {
	longUInt n = mGame.getPrecoalitions()[precoalition].size();

	for (longUInt sinternal = 0; sinternal < n; ++sinternal) {
		mCalculator->assign_zero(mTmp);
		// log::out << "max: " << mGame.getQuota()-weights[player] << log::endl;
		longUInt max = std::max((longUInt)1, mGame.getQuota() - weights[player]);
		// TESTING replaced w=mGame.getQuota()-weights[player] with max
		for (longUInt w = max; w < mGame.getQuota(); ++w) {
			// log::out << "cwoi: " << cwoi(w-1, sinternal).uint << log::endl;
			mCalculator->plusEqual(mTmp, cwoi(w - 1, sinternal));
		}

		// for dictator
		if (s == -1) {
			if (sinternal == 0) {
				if (weights[player] >= mGame.getQuota()) {
					mCalculator->increment(mTmp);
				}
			}
		}
		mCalculator->to_bigInt(&mBigTmp, mTmp);
		internal_ssi[mGame.getPrecoalitions()[precoalition][player]] += (factorial[sinternal] * factorial[n - sinternal - 1]) * mBigTmp * scale_factor;
	}
}
