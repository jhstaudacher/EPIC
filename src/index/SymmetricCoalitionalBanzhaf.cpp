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
		mCalculator->alloc_largeNumberArray(factorial, size);

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

	for (longUInt i = 0; i < mNbPart; i++) {
		int wi = mPartW[i];
		for (longUInt k = mGame.getQuota() + wi; k <= totalWeight; ++k) {
			mCalculator->plusEqual(cc[k - wi], cc[k]);
		}
	}

	ArrayOffset<lint::LargeNumber> cw(totalWeight + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(cw.getArrayPointer(), cw.getNumberOfElements());
	
	//replicate vector cc onto cw
	for (longUInt i = mGame.getQuota(); i < (totalWeight + 1); i++){
		mCalculator->assign(cw[i], cc[i]);
	}

	bigFloat ExternalMultiplier = 1 / (pow(2, (mNbPart)-1));
	for (longUInt i = 0; i < mNbPart; i++){
		log::out << "partition: " << i << log::endl;
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
		for (longUInt iii = mGame.getQuota(); iii < (mGame.getQuota() + mPartW[i]); iii++){
			mCalculator->plusEqual(banzhafsExternalGame[i], cw[iii]);
		}

		int nbPlayersInParti = mGame.getPrecoalitions()[i].size();
		Array2dOffset<lint::LargeNumber> cw2;
		cw2.alloc(mGame.getWeightSum() + 1, nbPlayersInParti + 1, mGame.getQuota(), 0);
		mCalculator->allocInit_largeNumberArray(cw2.getArrayPointer(), cw2.getNumberOfElements());

		//replicate vector cw onto cw2[]
		for (longUInt i = mGame.getQuota(); i < (totalWeight + 1); i++){
			//cw2(i, nbPlayersInParti) = cw[i];
			mCalculator->assign(cw2(i, nbPlayersInParti), cw[i]);
		}

		if (nbPlayersInParti > 1){
			//winternal = mGame.getWeights
			//std::vector<lint::LargeNumber> shapleysInternal(mGame.getPrecoalitions()[i].size());
			auto shapleysInternal = new lint::LargeNumber[mGame.getPrecoalitions()[i].size()];
			mCalculator->allocInit_largeNumberArray(shapleysInternal, mGame.getPrecoalitions()[i].size());
			//std::vector<lint::LargeNumber> rawShapleysInternal(mGame.getPrecoalitions()[i].size());

			//cw2(mGame.getWeightSum(), nbPlayersInParti).uint = 1;
			mCalculator->assign_one(cw2(mGame.getWeightSum(), nbPlayersInParti));
			for (int ii = 0; ii < nbPlayersInParti; ii++){
				longUInt x = mGame.getQuota() + mGame.getWeights()[mGame.getPrecoalitions()[i][ii]];
				while (x <= mGame.getWeightSum()){
					int m = nbPlayersInParti;
					while (m>1){
						mCalculator->plus(cw2(x - mGame.getWeights()[mGame.getPrecoalitions()[i][ii]], m-1), cw2(x, m), cw2(x-mGame.getWeights()[mGame.getPrecoalitions()[i][ii]], m-1));	
						m = m - 1;
					}
					//log::out << "x gets + 1" << log::endl;
					x = x + 1;
				}
				//log::out << "--------------------" << log::endl;

			}
			//log::out << "cw2 after: " << log::endl;
			//for (longUInt iii = mGame.getQuota(); iii < (mGame.getWeightSum() + 1); iii++){
			//	log::out << cw2(iii, nbPlayersInParti-1).uint << log::endl;			
			//}
			Array2dOffset<lint::LargeNumber> cwi;
			cwi.alloc(mGame.getWeightSum() + 1, nbPlayersInParti + 1, mGame.getQuota(), 0);
			mCalculator->allocInit_largeNumberArray(cwi.getArrayPointer(), cwi.getNumberOfElements());

			for (int ii = 0; ii < nbPlayersInParti; ii++){
				//replicate cw2 onto cwi
				for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); x++){
					for (int xx = 0; xx <= nbPlayersInParti; xx++){
						//cwi(x, xx) = cw2(x, xx);
						mCalculator->assign(cwi(x, xx), cw2(x, xx));
					}
				}
				//if ((sum(w)-winternal[ii]) >= q){
				if ((mGame.getWeightSum()-mGame.getWeights()[mGame.getPrecoalitions()[i][ii]]) >= mGame.getQuota()){
					//for (x in seq(from = (sum(w)-winternal[ii]), to=q, by=-1)){
					for(longUInt x = (mGame.getWeightSum() - mGame.getWeights()[mGame.getPrecoalitions()[i][ii]]); x >= mGame.getQuota(); x = x - 1){
						int sinternal = 1;
						//log::out << "x: " << x << log::endl;
						while (sinternal < nbPlayersInParti){
							//cwi(x, nbPlayersInparti-sinternal) = cw2(x, nbPlayersInParti-sinternal) - cwi(x+mGame.getWeights()[mGame.getPrecoalitions()[i][ii]-1], nbPlayersInParti-sinternal+1);
							mCalculator->minus(cwi(x, nbPlayersInParti-sinternal), cw2(x, nbPlayersInParti-sinternal), cwi(x+mGame.getWeights()[mGame.getPrecoalitions()[i][ii]], nbPlayersInParti-sinternal+1));
							sinternal = sinternal + 1;
						}
					}
				}
				//test cwi
				//log::out << "cwi: " << log::endl;
				//for (longUInt iii = mGame.getQuota(); iii < (mGame.getWeightSum() + 1); iii++){
				//	log::out << cwi(iii, nbPlayersInParti-1).uint << log::endl;			
				//}
				for (int s = 0; s <= (nbPlayersInParti-1); s++){
					lint::LargeNumber factor;
					mCalculator->alloc_largeNumber(factor);

					mCalculator->mul(factor, factorial[nbPlayersInParti-s-1], factorial[s]);
					lint::LargeNumber sum;
					mCalculator->alloc_largeNumber(sum);
					mCalculator->assign_zero(sum);
					for (longUInt iii = mGame.getQuota(); iii < (mGame.getQuota() + mGame.getWeights()[mGame.getPrecoalitions()[i][ii]]); iii++){
						mCalculator->plusEqual(sum, cwi(iii, s+1));
					}
					mCalculator->mulEqual(sum, factor);

					mCalculator->plusEqual(shapleysInternal[ii], sum);
					//rawShapleysInternal[ii] = shapleysInternal[ii];
					mCalculator->free_largeNumber(factor);
					mCalculator->free_largeNumber(sum);
				}
				bigInt shapleys_Internal;
				bigInt factor;

				//debug
				mCalculator->to_bigInt(&factor, factorial[nbPlayersInParti]);
				mCalculator->to_bigInt(&shapleys_Internal, shapleysInternal[ii]);
				//log::out << "factor: " << factor << log::endl;
				//log::out << "shapley_insternal: " << shapleys_Internal << log::endl;

				solution[mGame.getPrecoalitions()[i][ii]] = ExternalMultiplier * shapleys_Internal / factor;
				log::out << "--------------------" << log::endl;
			}

			mCalculator->free_largeNumberArray(shapleysInternal);
			delete[] shapleysInternal;

			//raw shapley
			//lint::LargeNumber sumRawShapleysInternal;
			//mCalculator->alloc_largeNumber(sumRawShapleysInternal);
			//for (const auto& value: rawShapleysInternal){
			//	//sumRawShapleysInternal = sumRawShapleysInternal + value;
			//	mCalculator->plusEqual(sumRawShapleysInternal, value);
			//}
			mCalculator->free_largeNumberArray(cwi.getArrayPointer());
		}
		else{
			solution[mGame.getPrecoalitions()[i][0]] = ExternalMultiplier * banzhafsExternalGame[i].uint;
		}
		mCalculator->free_largeNumberArray(cw2.getArrayPointer());
	}

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
