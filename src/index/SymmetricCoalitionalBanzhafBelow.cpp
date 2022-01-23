#include "index/SymmetricCoalitionalBanzhafBelow.h"

#include "Logging.h"

#include <cmath>

epic::index::SymmetricCoalitionalBanzhafBelow::SymmetricCoalitionalBanzhafBelow(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: PowerIndexWithPrecoalitions(g) {
	bigInt max_value = approx->upperBound_totalNumberOfSwingPlayer();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);

	mCalculator->alloc_largeNumber(mTmp);
}

epic::index::SymmetricCoalitionalBanzhafBelow::~SymmetricCoalitionalBanzhafBelow() {
	/*
	 * Delete the mCalculator object
	 */
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);

	/**
	 * If needed do additional cleanup below
	 */
}

std::vector<epic::bigFloat> epic::index::SymmetricCoalitionalBanzhafBelow::calculate() {
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

	longUInt quota = mGame.getQuota();
	int nbPlayers = mGame.getNumberOfPlayers();
	//longUInt totalWeight = mGame.getWeightSum();
	std::vector<bigFloat> solution(nbPlayers);
	auto banzhafsExternalGame = new lint::LargeNumber[mNbPart];
	mCalculator->allocInit_largeNumberArray(banzhafsExternalGame, mNbPart);

	auto cc = new lint::LargeNumber[quota];
	mCalculator->allocInit_largeNumberArray(cc, quota);

	auto cwo = new lint::LargeNumber[quota];
	mCalculator->allocInit_largeNumberArray(cwo, quota);

	Array2d<lint::LargeNumber> cwoi;
	cwoi.alloc(quota, mMaxPartSize + 1);
	mCalculator->allocInit_largeNumberArray(cwoi.getArrayPointer(), cwoi.getNumberOfElements());

	auto winternal = new longUInt[mMaxPartSize];

	generalizedForwardCountingPerWeight(cc, mPartW, mNbPart, 1);

	bigFloat ExternalMultiplier = 1 / (pow(2, (mNbPart)-1));
	for (longUInt i = 0; i < mNbPart; i++) {
		int nbPlayersInParti = mGame.getPrecoalitions()[i].size();
		//initialize winternal
		for (int z = 0; z < nbPlayersInParti; ++z) {
			winternal[z] = mGame.getWeights()[mGame.getPrecoalitions()[i][z]];
		}
		
		coalitionsWithoutPlayerFromBelow(cwo, cc, mPartW[i]);
		longUInt max = std::max((longUInt)1, quota-mPartW[i]);
		for (longUInt ii = max; ii < quota; ii++) {
			mCalculator->plusEqual(banzhafsExternalGame[i], cwo[ii-1]);
		}
		//for dictator players
		if (mPartW[i] >= quota){
			mCalculator->increment(banzhafsExternalGame[i]);
		}


		Array2d<lint::LargeNumber> cwo2;
		cwo2.alloc(quota, mMaxPartSize + 1);
		mCalculator->allocInit_largeNumberArray(cwo2.getArrayPointer(), cwo2.getNumberOfElements());
		for (longUInt ii = 0; ii < quota; ii++) {
			mCalculator->assign(cwo2(ii, 0), cwo[ii]);
		}


		if (nbPlayersInParti > 1) {
			auto shapleysInternal = new bigInt[mGame.getPrecoalitions()[i].size()];
			generalizedForwardCountingPerWeightCardinality(cwo2, winternal, nbPlayersInParti, (longUInt)1, 1, true);

			for (int ii = 0; ii < nbPlayersInParti; ii++) {				
				coalitionsCardinalityWithoutPlayerFromBelow(cwo2, cwoi, nbPlayersInParti, ii, winternal, 1, -1);

				updateInternalShapleyShubik(shapleysInternal, cwoi, i, ii, winternal, factorial);
				solution[mGame.getPrecoalitions()[i][ii]] = ExternalMultiplier * shapleysInternal[ii] / factorial[nbPlayersInParti];
			}
			delete[] shapleysInternal;
		} else {
			bigInt banzhafs_External;
			mCalculator->to_bigInt(&banzhafs_External, banzhafsExternalGame[i]);
			solution[mGame.getPrecoalitions()[i][0]] = ExternalMultiplier * banzhafs_External;
		}
		mCalculator->free_largeNumberArray(cwo2.getArrayPointer());
	}
	mCalculator->free_largeNumberArray(cwoi.getArrayPointer());
	mCalculator->free_largeNumberArray(cc);
	mCalculator->free_largeNumberArray(cwo);
	mCalculator->free_largeNumberArray(banzhafsExternalGame);
	delete[] banzhafsExternalGame;
	delete[] factorial;

	return solution;
}

std::string epic::index::SymmetricCoalitionalBanzhafBelow::getFullName() {
	/*
	 * Return the full name of the implemented power index.
	 */
	return "SymmetricCoalitionalBanzhafBelow";
}

epic::longUInt epic::index::SymmetricCoalitionalBanzhafBelow::getMemoryRequirement() {
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

void epic::index::SymmetricCoalitionalBanzhafBelow::updateInternalShapleyShubik(bigInt* internal_ssi, Array2d<lint::LargeNumber>& cwoi, longUInt precoalition, longUInt player, longUInt* weights, bigInt* factorial) {
	longUInt n = mGame.getPrecoalitions()[precoalition].size();
	for (longUInt sinternal = 0; sinternal < n; ++sinternal) {
		mCalculator->assign_zero(mTmp);
		longUInt max = std::max((longUInt)1, mGame.getQuota()-weights[player]);
		for (longUInt w = max; w < mGame.getQuota(); ++w) {
			mCalculator->plusEqual(mTmp, cwoi(w-1, sinternal));
		}
		//added for dictator players
		if (sinternal == 0){
			if (weights[player] >= mGame.getQuota()){
				mCalculator->increment(mTmp);
			}
		}
		mCalculator->to_bigInt(&mBigTmp, mTmp);
		internal_ssi[player] += (factorial[sinternal] * factorial[n - sinternal - 1]) * mBigTmp;
	}
}