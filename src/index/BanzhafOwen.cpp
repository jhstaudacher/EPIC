#include "index/BanzhafOwen.h"
#include "Logging.h"
#include <cmath>

epic::index::BanzhafOwen::BanzhafOwen(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation) : PowerIndexWithPrecoalitions(g) {
	/*
	 * Initialize the protected ItfPowerIndex::mCalculator object. This gets used to do all large integer calculations later.
	 * For an approximation of the maximum value needed for the calculations the approx object can get used.
	 */
	bigInt max_value = approx->upperBound_totalNumberOfSwingPlayer();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);
}

epic::index::BanzhafOwen::~BanzhafOwen() {
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::BanzhafOwen::calculate() {
	/*
	 * Use mGame and mCalculator to calculate the power index and return the results.
	 */
	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	auto banzhafsExternalGame = new lint::LargeNumber[mNbPart];
	mCalculator->allocInit_largeNumberArray(banzhafsExternalGame, mNbPart);

	ArrayOffset<lint::LargeNumber> c(mGame.getWeightSum() + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(c.getArrayPointer(), c.getNumberOfElements());
	mCalculator->assign_one(c[mGame.getWeightSum()]);

	//backward counting per weight
	for (longUInt i = 0; i < mNbPart; i++) {
		longUInt wi = mPartW[i];
		for (longUInt k = mGame.getQuota() + wi; k <= mGame.getWeightSum(); ++k) {
			mCalculator->plusEqual(c[k - wi], c[k]);
		}
	}

	ArrayOffset<lint::LargeNumber> cw(mGame.getWeightSum() + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(cw.getArrayPointer(), cw.getNumberOfElements());

	//create cw2
	ArrayOffset<lint::LargeNumber> cw2(mGame.getWeightSum() + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(cw2.getArrayPointer(), cw2.getNumberOfElements());

	ArrayOffset<lint::LargeNumber> cwi(mGame.getWeightSum() + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(cwi.getArrayPointer(), cwi.getNumberOfElements());

	//replicate vector c onto cw
	for (longUInt i = mGame.getQuota(); i < (mGame.getWeightSum() + 1); i++){
		//cw[i] = c[i];
		mCalculator->assign(cw[i], c[i]);
	}

	// Work out number of swings on level of precoalitions
	for (longUInt i = 0; i < mNbPart; i++){
		for (longUInt ii = mGame.getQuota(); ii < (mGame.getWeightSum() + 1); ii++){
			//cw[ii] = c[ii];
			mCalculator->assign(cw[ii], c[ii]);
		}
		for(longUInt ii = (mGame.getWeightSum() - mPartW[i]); ii >= mGame.getQuota(); ii = ii - 1){
			mCalculator->minus(cw[ii], c[ii], cw[ii + mPartW[i]]);
		}

		//get sum of vector
		for (longUInt ii = mGame.getQuota(); ii < (mGame.getQuota() + mPartW[i]); ii++){
			mCalculator->plusEqual(banzhafsExternalGame[i], cw[ii]);
		}
		
		//replicate vector c onto cw
		for (longUInt ii = mGame.getQuota(); ii < (mGame.getWeightSum() + 1); ii++){
			//cw2[i] = cw[i];
			mCalculator->assign(cw2[ii], cw[ii]);
		}
		

		longUInt nbPlayersInParti = mGame.getPrecoalitions()[i].size();
		bigFloat ExternalMultiplier = 1 / (pow(2, (mNbPart)-1));
		if (nbPlayersInParti > 1){
			//std::vector<lint::LargeNumber> banzhafsInternal(mGame.getPrecoalitions()[i].size());
			auto banzhafsInternal = new lint::LargeNumber[nbPlayersInParti];
			mCalculator->allocInit_largeNumberArray(banzhafsInternal, nbPlayersInParti);
			
			for (longUInt ii = 0; ii < nbPlayersInParti; ii++){
				longUInt wii = mGame.getWeights()[mGame.getPrecoalitions()[i][ii]];
				for (longUInt iii = (mGame.getQuota() + wii); iii <= mGame.getWeightSum(); iii++){
					mCalculator->plusEqual(cw2[iii - wii], cw2[iii]);					
				}
			}

			for (longUInt ii = 0; ii < nbPlayersInParti; ii++){
				longUInt wii = mGame.getWeights()[mGame.getPrecoalitions()[i][ii]];
				//replicate vector cw2 onto cwi
				for (longUInt iii = mGame.getQuota(); iii < (mGame.getWeightSum() + 1); iii++){
					//cwi[i] = cw2[i];
					mCalculator->assign(cwi[iii], cw2[iii]);
				}
				for(longUInt iii = (mGame.getWeightSum() - wii); iii >= mGame.getQuota(); iii = iii - 1){
					mCalculator->minus(cwi[iii], cw2[iii], cwi[iii + wii]);
				}

				for (longUInt iii = mGame.getQuota(); iii <= (mGame.getQuota() + wii - 1); iii++){
					mCalculator->plusEqual(banzhafsInternal[ii], cwi[iii]);
				}

				bigFloat InternalMultiplier = 1/(pow(2, nbPlayersInParti-1));
				bigInt banzhafs_internal;
				mCalculator->to_bigInt(&banzhafs_internal, banzhafsInternal[ii]);
				solution[mGame.getPrecoalitions()[i][ii]] = ExternalMultiplier * InternalMultiplier * banzhafs_internal;
			}
			mCalculator->free_largeNumberArray(banzhafsInternal);
			delete[] banzhafsInternal;
		} else{
			bigInt banzhafs_external;
			mCalculator->to_bigInt(&banzhafs_external, banzhafsExternalGame[i]);
			solution[mGame.getPrecoalitions()[i][0]] = ExternalMultiplier * banzhafs_external;
		}
	}
	mCalculator->free_largeNumberArray(cw2.getArrayPointer());
	mCalculator->free_largeNumberArray(cwi.getArrayPointer());
	mCalculator->free_largeNumberArray(c.getArrayPointer());
	mCalculator->free_largeNumberArray(cw.getArrayPointer());
	mCalculator->free_largeNumberArray(banzhafsExternalGame);
	delete[] banzhafsExternalGame;

	return solution;
}

std::string epic::index::BanzhafOwen::getFullName() {
	/*
	 * Return the full name of the implemented power index.
	 */
	return "BanzhafOwen";
}

epic::longUInt epic::index::BanzhafOwen::getMemoryRequirement() {
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
