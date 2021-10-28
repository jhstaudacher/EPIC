#include "index/SymmetricCoalitionalBanzhaf.h"
#include "Logging.h"
#include <cmath>

epic::index::SymmetricCoalitionalBanzhaf::SymmetricCoalitionalBanzhaf(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation) : ItfPowerIndex(g) {
	/*
	 * Initialize the protected ItfPowerIndex::mCalculator object. This gets used to do all large integer calculations later.
	 * For an approximation of the maximum value needed for the calculations the approx object can get used.
	 */
	bigInt max_value = approx->upperBound_totalNumberOfSwingPlayer();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);

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

	/*
	 * If needed add additional initializations below
	 */
}

epic::index::SymmetricCoalitionalBanzhaf::~SymmetricCoalitionalBanzhaf() {
	/*
	 * Delete the mCalculator object
	 */
	delete[] mPartW;
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);

	/**
	 * If needed do additional cleanup below
	 */
}

std::vector<epic::bigFloat> epic::index::SymmetricCoalitionalBanzhaf::calculate() {
	/*
	 * Use mGame and mCalculator to calculate the power index and return the results.
	 */
	lint::LargeNumber* factorial;
	{
		longUInt size = std::max(mMaxPartSize, mNbPart);
		factorial = new lint::LargeNumber[size + 1];
		mCalculator->alloc_largeNumberArray(factorial, size+1);

		mCalculator->assign_one(factorial[0]);
		mCalculator->assign_one(factorial[1]);
		for (longUInt i = 2; i <= size; ++i) {
			mCalculator->mul(factorial[i], factorial[i - 1], i);
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

	auto winternal2 = new longUInt[mMaxPartSize];

	for (longUInt i = 0; i < mNbPart; i++) {
		int wi = mPartW[i];
		for (longUInt k = mGame.getQuota() + wi; k <= totalWeight; ++k) {
			mCalculator->plusEqual(cc[k - wi], cc[k]);
		}
	}	

	//replicate vector cc onto cw
	for (longUInt i = mGame.getQuota(); i < (totalWeight + 1); i++){
		mCalculator->assign(cw[i], cc[i]);
	}

	bigFloat ExternalMultiplier = 1 / (pow(2, (mNbPart)-1));
	for (longUInt i = 0; i < mNbPart; i++){
		int nbPlayersInParti = mGame.getPrecoalitions()[i].size();
		//initialize winternal
		for (int z = 0; z < nbPlayersInParti; ++z) {
			winternal2[z] = mGame.getWeights()[mGame.getPrecoalitions()[i][z]];
		}

		for (longUInt ii = mGame.getQuota(); ii < (totalWeight + 1); ii++){
			//cw[ii] = cc[ii];
			mCalculator->assign(cw[ii], cc[ii]);
		}

		if ((totalWeight-mPartW[i]) >= mGame.getQuota()){
			for(longUInt ii = (totalWeight - mPartW[i]); ii >= mGame.getQuota(); ii = ii - 1){
				mCalculator->minus(cw[ii], cc[ii], cw[ii + mPartW[i]]);
			}
		}

		//get sum of vector
		longUInt min = std::min(mGame.getQuota() + mPartW[i] - 1, mGame.getWeightSum());
		
		for (longUInt iii = mGame.getQuota(); iii <= min ; iii++){
			mCalculator->plusEqual(banzhafsExternalGame[i], cw[iii]);
		}

		Array2dOffset<lint::LargeNumber> cw2;
		cw2.alloc(totalWeight + 1, mMaxPartSize + 1, mGame.getQuota(), 0);
		mCalculator->allocInit_largeNumberArray(cw2.getArrayPointer(), cw2.getNumberOfElements());
		//replicate vector cw onto cw2[]
		for (longUInt i = mGame.getQuota(); i < (totalWeight + 1); i++){
			mCalculator->assign(cw2(i, nbPlayersInParti-1), cw[i]);
		}


		if (nbPlayersInParti > 1){
			auto shapleysInternal = new lint::LargeNumber[mGame.getPrecoalitions()[i].size()];
			mCalculator->allocInit_largeNumberArray(shapleysInternal, mGame.getPrecoalitions()[i].size());
			mCalculator->assign_one(cw2(totalWeight, nbPlayersInParti));

			generalizedBackwardCountingPerWeightCardinality(cw2, winternal2, nbPlayersInParti);

			for (int ii = 0; ii < nbPlayersInParti; ii++){
				longUInt winternal = mGame.getWeights()[mGame.getPrecoalitions()[i][ii]];

				if ((totalWeight-winternal) >= mGame.getQuota()){
					coalitionsCardinalityContainingPlayerFromAbove(cwi, cw2, nbPlayersInParti, ii, winternal2);
				}
				else {
					for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
						for (int y = 0; y < nbPlayersInParti; ++y) {
							mCalculator->assign(cwi(x, y), cw2(x, y));
						}
					}
				}
				
				for (int s = 0; s < nbPlayersInParti; s++){
					lint::LargeNumber factor;
					mCalculator->alloc_largeNumber(factor);

					mCalculator->mul(factor, factorial[nbPlayersInParti-s-1], factorial[s]);
					lint::LargeNumber sum;
					mCalculator->alloc_largeNumber(sum);
					mCalculator->assign_zero(sum);
					min = std::min(mGame.getQuota() + winternal - 1, mGame.getWeightSum());
					for (longUInt iii = mGame.getQuota(); iii <= min; iii++){
						mCalculator->plusEqual(sum, cwi(iii, s));
					}
					mCalculator->mulEqual(sum, factor);

					mCalculator->plusEqual(shapleysInternal[ii], sum);
					mCalculator->free_largeNumber(factor);
					mCalculator->free_largeNumber(sum);
				}
				
				bigInt shapleys_Internal;
				bigInt factor;

				mCalculator->to_bigInt(&factor, factorial[nbPlayersInParti]);
				mCalculator->to_bigInt(&shapleys_Internal, shapleysInternal[ii]);

				solution[mGame.getPrecoalitions()[i][ii]] = ExternalMultiplier * shapleys_Internal / factor;
			}

			mCalculator->free_largeNumberArray(shapleysInternal);
			delete[] shapleysInternal;			
		}
		else{
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
	mCalculator->free_largeNumberArray(factorial);
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
	/*
	 * Return an approximated value of the amount of memory needed for the power index calculation.
	 */
	bigInt memory = 42; // write the needed memory into this variable

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}
	return ret;
}

//cwi, cw2, nbPlayersInParti, ii, winternal2
void epic::index::SymmetricCoalitionalBanzhaf::coalitionsCardinalityContainingPlayerFromAbove(Array2dOffset<lint::LargeNumber>& cw, Array2dOffset<lint::LargeNumber>& cc, longUInt n_player, longUInt player, longUInt* weights) {
	longUInt totalWeight = mGame.getWeightSum();
	longUInt winternal = weights[player];
	//replicate cc onto cw
	for (longUInt x = mGame.getQuota(); x <= totalWeight; ++x) {
		for (longUInt y = 0; y < n_player; ++y) {
			mCalculator->assign(cw(x, y), cc(x, y));
		}
	}

	for (longUInt x = mGame.getWeightSum() - winternal; x >= mGame.getQuota(); --x) {
		for (longUInt s = 1; s < n_player; ++s) {
			mCalculator->minus(cw(x, n_player - s - 1), cc(x, n_player - s - 1), cw(x + winternal, n_player - s));
		}
	}
}

void epic::index::SymmetricCoalitionalBanzhaf::generalizedBackwardCountingPerWeightCardinality(Array2dOffset<lint::LargeNumber>& cw2, longUInt*  weights, longUInt n) {
	longUInt totalWeight = mGame.getWeightSum();
	longUInt quota = mGame.getQuota();
	for (longUInt i = 0; i < n; ++i) {
		longUInt winternal = weights[i];
		for (longUInt x = quota + winternal; x <= totalWeight; ++x) {
			for (longUInt m = 1; m < n; ++m) {
				mCalculator->plusEqual(cw2(x - winternal, m - 1), cw2(x, m));
			}
		}
	}

}
