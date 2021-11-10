#include "index/SymmetricCoalitionalBanzhaf.h"

#include "Logging.h"

#include <cmath>

epic::index::SymmetricCoalitionalBanzhaf::SymmetricCoalitionalBanzhaf(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: PowerIndexWithPrecoalitions(g) {
	bigInt max_value = approx->upperBound_totalNumberOfSwingPlayer();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);

	mCalculator->alloc_largeNumber(mTmp);
}

epic::index::SymmetricCoalitionalBanzhaf::~SymmetricCoalitionalBanzhaf() {
	/*
	 * Delete the mCalculator object
	 */
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);

	/**
	 * If needed do additional cleanup below
	 */
}

std::vector<epic::bigFloat> epic::index::SymmetricCoalitionalBanzhaf::calculate() {
	/*
	 * Use mGame and mCalculator to calculate the power index and return the results.
	 */
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

	int nbPlayers = mGame.getNumberOfPlayers();
	longUInt totalWeight = mGame.getWeightSum();
	std::vector<bigFloat> solution(nbPlayers);
	auto banzhafsExternalGame = new lint::LargeNumber[mNbPart];
	mCalculator->allocInit_largeNumberArray(banzhafsExternalGame, mNbPart);

	ArrayOffset<lint::LargeNumber> cc(totalWeight + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(cc.getArrayPointer(), cc.getNumberOfElements());
	mCalculator->assign_one(cc[totalWeight]);

	ArrayOffset<lint::LargeNumber> cw(totalWeight + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(cw.getArrayPointer(), cw.getNumberOfElements());

	Array2dOffset<lint::LargeNumber> cwi;
	cwi.alloc(totalWeight + 1, mMaxPartSize, mGame.getQuota(), 0);
	mCalculator->allocInit_largeNumberArray(cwi.getArrayPointer(), cwi.getNumberOfElements());

	auto winternal = new longUInt[mMaxPartSize];

	generalizedBackwardCountingPerWeight(cc, mPartW, mNbPart);

	bigFloat ExternalMultiplier = 1 / (pow(2, (mNbPart)-1));
	for (longUInt i = 0; i < mNbPart; i++) {
		int nbPlayersInParti = mGame.getPrecoalitions()[i].size();
		//initialize winternal
		for (int z = 0; z < nbPlayersInParti; ++z) {
			winternal[z] = mGame.getWeights()[mGame.getPrecoalitions()[i][z]];
		}
		coalitionsContainingPlayerFromAbove(cw, cc, mPartW[i]);

		//get sum of vector
		longUInt min = std::min(mGame.getQuota() + mPartW[i] - 1, mGame.getWeightSum());

		for (longUInt iii = mGame.getQuota(); iii <= min; iii++) {
			mCalculator->plusEqual(banzhafsExternalGame[i], cw[iii]);
		}

		Array2dOffset<lint::LargeNumber> cw2;
		cw2.alloc(totalWeight + 1, mMaxPartSize + 1, mGame.getQuota(), 0);
		mCalculator->allocInit_largeNumberArray(cw2.getArrayPointer(), cw2.getNumberOfElements());
		//replicate vector cw onto cw2[]
		for (longUInt i = mGame.getQuota(); i < (totalWeight + 1); i++) {
			mCalculator->assign(cw2(i, nbPlayersInParti - 1), cw[i]);
		}

		if (nbPlayersInParti > 1) {
			auto shapleysInternal = new bigInt[mGame.getPrecoalitions()[i].size()];
			mCalculator->assign_one(cw2(totalWeight, nbPlayersInParti));

			generalizedBackwardCountingPerWeightCardinality(cw2, winternal, nbPlayersInParti);

			for (int ii = 0; ii < nbPlayersInParti; ii++) {
				if ((totalWeight - winternal[ii]) >= mGame.getQuota()) {
					coalitionsCardinalityContainingPlayerFromAbove(cwi, cw2, nbPlayersInParti, ii, winternal);
				} else {
					for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
						for (int y = 0; y < nbPlayersInParti; ++y) {
							mCalculator->assign(cwi(x, y), cw2(x, y));
						}
					}
				}
				updateInternalShapleyShubik(shapleysInternal, cwi, i, ii, winternal, factorial);
				solution[mGame.getPrecoalitions()[i][ii]] = ExternalMultiplier * shapleysInternal[ii] / factorial[nbPlayersInParti];
			}
			delete[] shapleysInternal;
		} else {
			bigInt banzhafs_External;
			mCalculator->to_bigInt(&banzhafs_External, banzhafsExternalGame[i]);
			solution[mGame.getPrecoalitions()[i][0]] = ExternalMultiplier * banzhafs_External;
		}
		mCalculator->free_largeNumberArray(cw2.getArrayPointer());
	}
	mCalculator->free_largeNumberArray(cwi.getArrayPointer());
	mCalculator->free_largeNumberArray(cc.getArrayPointer());
	mCalculator->free_largeNumberArray(cw.getArrayPointer());
	mCalculator->free_largeNumberArray(banzhafsExternalGame);
	delete[] banzhafsExternalGame;
	delete[] factorial;

	return solution;
}

std::string epic::index::SymmetricCoalitionalBanzhaf::getFullName() {
	/*
	 * Return the full name of the implemented power index.
	 */
	return "SymmetricCoalitionalBanzhaf";
}

epic::longUInt epic::index::SymmetricCoalitionalBanzhaf::getMemoryRequirement() {
	bigInt memory = (mGame.getWeightSum() + 1 - mGame.getQuota()) * mCalculator->getLargeNumberSize() * 2; // cc, cw
	memory += mNbPart * c_sizeof_longUInt;																   // mPartW
	longUInt max = std::max(mMaxPartSize, mNbPart);
	memory += max * GMPHelper::size_of_float(bigInt::factorial(max));												// factorial
	memory += mGame.getNumberOfPlayers() * GMPHelper::size_of_int(bigInt::factorial(mMaxPartSize));					// shapleyInternal (only very rough approximation
	memory += mGame.getNumberOfPlayers() * GMPHelper::size_of_int(bigInt::factorial(mNbPart));						// banzhafExternal
	memory += (mGame.getWeightSum() + 1 - mGame.getQuota()) * mMaxPartSize * mCalculator->getLargeNumberSize() * 2; // cw2, cwi
	memory += mMaxPartSize * c_sizeof_longUInt;																		// winternal

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}
	return ret;
}

void epic::index::SymmetricCoalitionalBanzhaf::updateInternalShapleyShubik(bigInt* internal_ssi, Array2dOffset<lint::LargeNumber>& cwi, longUInt precoalition, longUInt player, longUInt* weights, bigInt* factorial) {
	longUInt n = mGame.getPrecoalitions()[precoalition].size();

	for (longUInt sinternal = 0; sinternal < n; ++sinternal) {
		mCalculator->assign_zero(mTmp);
		longUInt min = std::min(mGame.getQuota() + weights[player] - 1, mGame.getWeightSum());
		for (longUInt w = mGame.getQuota(); w <= min; ++w) {
			mCalculator->plusEqual(mTmp, cwi(w, sinternal));
		}

		mCalculator->to_bigInt(&mBigTmp, mTmp);
		internal_ssi[player] += (factorial[sinternal] * factorial[n - sinternal - 1]) * mBigTmp;
	}
}