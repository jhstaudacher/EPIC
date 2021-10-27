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
	generalizedBackwardCountingPerWeight(c, mPartW, mNbPart);

	ArrayOffset<lint::LargeNumber> cw(mGame.getWeightSum() + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(cw.getArrayPointer(), cw.getNumberOfElements());

	//create cw2
	ArrayOffset<lint::LargeNumber> cw2(mGame.getWeightSum() + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(cw2.getArrayPointer(), cw2.getNumberOfElements());

	ArrayOffset<lint::LargeNumber> cwi(mGame.getWeightSum() + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(cwi.getArrayPointer(), cwi.getNumberOfElements());

	auto banzhafsInternal = new lint::LargeNumber[mMaxPartSize];
	mCalculator->alloc_largeNumberArray(banzhafsInternal, mMaxPartSize);

	bigFloat ExternalMultiplier;
	{
		bigInt tmp;
		mpz_ui_pow_ui(tmp.get_mpz_t(), 2, mNbPart - 1);
		ExternalMultiplier = 1 / bigFloat(tmp);
	}

	auto winternal = new longUInt[mMaxPartSize];

	// Work out number of swings on level of precoalitions
	for (longUInt i = 0; i < mNbPart; i++){
		coalitionsContainingPlayerFromAbove(cw, c, mPartW[i]);

		//get sum of vector
		longUInt min = std::min(mGame.getQuota() + mPartW[i] - 1, mGame.getWeightSum());
		for (longUInt ii = mGame.getQuota(); ii <= min; ++ii){
			mCalculator->plusEqual(banzhafsExternalGame[i], cw[ii]);
		}
		
		//replicate vector c onto cw
		for (longUInt ii = mGame.getQuota(); ii < (mGame.getWeightSum() + 1); ii++){
			//cw2[i] = cw[i];
			mCalculator->assign(cw2[ii], cw[ii]);
		}

		longUInt nbPlayersInParti = mGame.getPrecoalitions()[i].size();
		if (nbPlayersInParti > 1){

			for (longUInt x = 0; x < nbPlayersInParti; ++x) {
				winternal[x] = mGame.getWeights()[mGame.getPrecoalitions()[i][x]];
				mCalculator->assign_zero(banzhafsInternal[x]);
			}

			generalizedBackwardCountingPerWeight(cw2, winternal, nbPlayersInParti);

			for (longUInt ii = 0; ii < nbPlayersInParti; ii++){
				coalitionsContainingPlayerFromAbove(cwi, cw2, winternal[ii]);

				for (longUInt iii = mGame.getQuota(); iii <= (mGame.getQuota() + winternal[ii] - 1); iii++){
					mCalculator->plusEqual(banzhafsInternal[ii], cwi[iii]);
				}


				bigFloat InternalMultiplier;
				{
					bigInt tmp;
					mpz_ui_pow_ui(tmp.get_mpz_t(), 2, nbPlayersInParti - 1);
					InternalMultiplier = 1 / bigFloat(tmp);
				}
				bigInt banzhafs_internal;
				mCalculator->to_bigInt(&banzhafs_internal, banzhafsInternal[ii]);
				solution[mGame.getPrecoalitions()[i][ii]] = ExternalMultiplier * InternalMultiplier * banzhafs_internal;
			}
		} else{
			bigInt banzhafs_external;
			mCalculator->to_bigInt(&banzhafs_external, banzhafsExternalGame[i]);
			solution[mGame.getPrecoalitions()[i][0]] = ExternalMultiplier * banzhafs_external;
		}
	}

	delete[] winternal;
	mCalculator->free_largeNumberArray(banzhafsInternal);
	delete[] banzhafsInternal;

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
