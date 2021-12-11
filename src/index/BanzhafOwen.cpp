#include "index/BanzhafOwen.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

#include <cmath>

epic::index::BanzhafOwen::BanzhafOwen() : PowerIndexWithPrecoalitions() {
	gCalculator->alloc_largeNumber(mTmp);
}

epic::index::BanzhafOwen::~BanzhafOwen() {
	gCalculator->free_largeNumber(mTmp);
}

std::vector<epic::bigFloat> epic::index::BanzhafOwen::calculate(Game& g) {
	std::vector<bigFloat> solution(g.getNumberOfPlayers());
	longUInt totalWeight = g.getWeightSum();
	longUInt quota = g.getQuota();

	ArrayOffset<lint::LargeNumber> c(totalWeight + 1, quota);
	gCalculator->allocInit_largeNumberArray(c.getArrayPointer(), c.getNumberOfElements());
	gCalculator->assign_one(c[totalWeight]);
	generalizedBackwardCountingPerWeight(g, c, g.getPrecoalitionWeights(), g.getNumberOfPrecoalitions());

	ArrayOffset<lint::LargeNumber> cw(totalWeight + 1, quota);
	gCalculator->allocInit_largeNumberArray(cw.getArrayPointer(), cw.getNumberOfElements());

	ArrayOffset<lint::LargeNumber> cw2(totalWeight + 1, quota);
	gCalculator->alloc_largeNumberArray(cw2.getArrayPointer(), cw2.getNumberOfElements());

	ArrayOffset<lint::LargeNumber> cwi(totalWeight + 1, quota);
	gCalculator->allocInit_largeNumberArray(cwi.getArrayPointer(), cwi.getNumberOfElements());

	auto banzhafsInternal = new lint::LargeNumber[g.getMaxPrecoalitionSize()];
	gCalculator->alloc_largeNumberArray(banzhafsInternal, g.getMaxPrecoalitionSize());

	bigFloat ExternalMultiplier;
	{
		mpz_ui_pow_ui(mBigTmp.get_mpz_t(), 2, g.getNumberOfPrecoalitions() - 1);
		ExternalMultiplier = 1 / bigFloat(mBigTmp);
	}

	std::vector<longUInt> winternal(g.getMaxPrecoalitionSize());

	// Work out number of swings on level of precoalitions
	for (longUInt i = 0; i < g.getNumberOfPrecoalitions(); i++) {
		coalitionsContainingPlayerFromAbove(g, cw, c, g.getPrecoalitionWeights()[i]);

		//replicate vector c onto cw
		for (longUInt ii = quota; ii <= totalWeight; ii++) {
			//cw2[i] = cw[i];
			gCalculator->assign(cw2[ii], cw[ii]);
		}

		longUInt nbPlayersInParti = g.getPrecoalitions()[i].size();
		if (nbPlayersInParti > 1) {
			for (longUInt x = 0; x < nbPlayersInParti; ++x) {
				winternal[x] = g.getWeights()[g.getPrecoalitions()[i][x]];
				gCalculator->assign_zero(banzhafsInternal[x]);
			}

			generalizedBackwardCountingPerWeight(g, cw2, winternal, nbPlayersInParti);

			for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
				coalitionsContainingPlayerFromAbove(g, cwi, cw2, winternal[ii]);

				longUInt min = std::min(quota + winternal[ii] - 1, totalWeight);
				for (longUInt iii = quota; iii <= min; iii++) {
					gCalculator->plusEqual(banzhafsInternal[ii], cwi[iii]);
				}

				bigFloat InternalMultiplier;
				{ // InternalMultiplier = 2^{nbPlayerInParti - 1}
					mpz_ui_pow_ui(mBigTmp.get_mpz_t(), 2, nbPlayersInParti - 1);
					InternalMultiplier = 1 / bigFloat(mBigTmp);
				}
				gCalculator->to_bigInt(&mBigTmp, banzhafsInternal[ii]);
				solution[g.getPrecoalitions()[i][ii]] = ExternalMultiplier * InternalMultiplier * mBigTmp;
			}
		} else {
			//get sum of vector
			gCalculator->assign_zero(mTmp);
			longUInt min = std::min(quota + g.getPrecoalitionWeights()[i] - 1, totalWeight);
			for (longUInt ii = quota; ii <= min; ++ii) {
				gCalculator->plusEqual(mTmp, cw[ii]);
			}
			bigInt banzhafs_external;
			gCalculator->to_bigInt(&banzhafs_external, mTmp);
			solution[g.getPrecoalitions()[i][0]] = ExternalMultiplier * banzhafs_external;
		}
	}

	gCalculator->free_largeNumberArray(banzhafsInternal);
	delete[] banzhafsInternal;

	gCalculator->free_largeNumberArray(cw2.getArrayPointer());
	gCalculator->free_largeNumberArray(cwi.getArrayPointer());
	gCalculator->free_largeNumberArray(c.getArrayPointer());
	gCalculator->free_largeNumberArray(cw.getArrayPointer());

	return solution;
}

std::string epic::index::BanzhafOwen::getFullName() {
	return "BanzhafOwen";
}

epic::longUInt epic::index::BanzhafOwen::getMemoryRequirement(Game& g) {
	bigInt memory = (g.getWeightSum() + 1 - g.getQuota()) * gCalculator->getLargeNumberSize() * 4; // c, cw, cw2, cwi
	memory += g.getMaxPrecoalitionSize() * gCalculator->getLargeNumberSize();											   // banzhafInternal
	memory += g.getMaxPrecoalitionSize() * c_sizeof_longUInt;															   // winternal
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}
	return ret;
}

epic::bigInt epic::index::BanzhafOwen::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_totalNumberOfSwingPlayer();
}

epic::lint::Operation epic::index::BanzhafOwen::getOperationRequirement() {
	return lint::Operation::addition;
}
