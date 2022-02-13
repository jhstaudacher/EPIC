#include "index/BanzhafOwenBelow.h"

#include "Logging.h"

#include <cmath>

epic::index::BanzhafOwenBelow::BanzhafOwenBelow(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: PowerIndexWithPrecoalitions(g) {
	bigInt max_value = approx->upperBound_totalNumberOfSwingPlayer();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);

	mCalculator->alloc_largeNumber(mTmp);
}

epic::index::BanzhafOwenBelow::~BanzhafOwenBelow() {
	mCalculator->free_largeNumber(mTmp);
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::BanzhafOwenBelow::calculate() {
	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	longUInt quota = mGame.getQuota();

	auto c = new lint::LargeNumber[quota - 1];
	mCalculator->allocInit_largeNumberArray(c, quota - 1);

	generalizedForwardCountingPerWeight(c, mPartW, mNbPart, 1);

	auto cwo = new lint::LargeNumber[quota - 1];
	mCalculator->allocInit_largeNumberArray(cwo, quota - 1);

	auto cwo2 = new lint::LargeNumber[quota - 1];
	mCalculator->allocInit_largeNumberArray(cwo2, quota - 1);

	auto cwoi = new lint::LargeNumber[quota - 1];
	mCalculator->allocInit_largeNumberArray(cwoi, quota - 1);

	auto banzhafsInternal = new lint::LargeNumber[mMaxPartSize];
	mCalculator->alloc_largeNumberArray(banzhafsInternal, mMaxPartSize);

	bigFloat ExternalMultiplier;
	{
		mpz_ui_pow_ui(mBigTmp.get_mpz_t(), 2, mNbPart - 1);
		ExternalMultiplier = 1 / bigFloat(mBigTmp);
	}

	auto winternal = new longUInt[mMaxPartSize];

	// Work out number of swings on level of precoalitions
	for (longUInt i = 0; i < mNbPart; i++) {
		coalitionsWithoutPlayerFromBelow(cwo, c, mPartW[i]);

		// replicate vector cwo onto cwo2
		for (longUInt ii = 0; ii < quota - 1; ii++) {
			mCalculator->assign(cwo2[ii], cwo[ii]);
		}

		longUInt nbPlayersInParti = mGame.getPrecoalitions()[i].size();
		if (nbPlayersInParti > 1) {
			for (longUInt x = 0; x < nbPlayersInParti; ++x) {
				winternal[x] = mGame.getWeights()[mGame.getPrecoalitions()[i][x]];
				mCalculator->assign_zero(banzhafsInternal[x]);
			}

			generalizedForwardCountingPerWeight(cwo2, winternal, nbPlayersInParti, 1);

			for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
				coalitionsWithoutPlayerFromBelow(cwoi, cwo2, winternal[ii]);

				longUInt max = std::max((longUInt)1, quota - winternal[ii]);
				for (longUInt iii = max; iii <= quota - 1; iii++) {
					mCalculator->plusEqual(banzhafsInternal[ii], cwoi[iii - 1]);
				}
				// for dictator player
				if (winternal[ii] >= quota) {
					mCalculator->increment(banzhafsInternal[ii]);
				}

				bigFloat InternalMultiplier;
				{
					mpz_ui_pow_ui(mBigTmp.get_mpz_t(), 2, nbPlayersInParti - 1);
					InternalMultiplier = 1 / bigFloat(mBigTmp);
				}

				mCalculator->to_bigInt(&mBigTmp, banzhafsInternal[ii]);
				solution[mGame.getPrecoalitions()[i][ii]] = ExternalMultiplier * InternalMultiplier * mBigTmp;
			}
		} else {
			// get sum of vector
			mCalculator->assign_zero(mTmp);
			longUInt max = std::max((longUInt)0, quota - mPartW[i]);
			for (longUInt ii = max; ii <= quota - 1; ++ii) {
				mCalculator->plusEqual(mTmp, cwo[ii - 1]);
			}
			// for dictator player
			if (mPartW[i] >= quota) {
				mCalculator->increment(mTmp);
			}
			bigInt banzhafs_external;
			mCalculator->to_bigInt(&banzhafs_external, mTmp);
			solution[mGame.getPrecoalitions()[i][0]] = ExternalMultiplier * banzhafs_external;
		}
	}

	delete[] winternal;
	mCalculator->free_largeNumberArray(banzhafsInternal);
	delete[] banzhafsInternal;

	mCalculator->free_largeNumberArray(cwo2);
	mCalculator->free_largeNumberArray(cwoi);
	mCalculator->free_largeNumberArray(c);
	mCalculator->free_largeNumberArray(cwo);

	return solution;
}

std::string epic::index::BanzhafOwenBelow::getFullName() {
	return "BanzhafOwenBelow";
}

epic::longUInt epic::index::BanzhafOwenBelow::getMemoryRequirement() {
	bigInt memory = (mGame.getWeightSum() + 1 - mGame.getQuota()) * mCalculator->getLargeNumberSize() * 4;
	memory += mMaxPartSize * mCalculator->getLargeNumberSize();
	memory += mMaxPartSize * c_sizeof_longUInt;
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}
	return ret;
}
