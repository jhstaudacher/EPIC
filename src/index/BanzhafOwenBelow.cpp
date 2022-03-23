#include "index/BanzhafOwenBelow.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

#include <cmath>

epic::index::BanzhafOwenBelow::BanzhafOwenBelow()
	: PowerIndexWithPrecoalitions() {}

std::vector<epic::bigFloat> epic::index::BanzhafOwenBelow::calculate(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);

	gCalculator->alloc_largeNumber(mTmp);

	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	longUInt quota = g->getQuota();

	auto c = new lint::LargeNumber[quota - 1];
	gCalculator->allocInit_largeNumberArray(c, quota - 1);

	generalizedForwardCountingPerWeight(g, c, g->getPrecoalitionWeights(), g->getNumberOfPrecoalitions(), 1);

	auto cwo = new lint::LargeNumber[quota - 1];
	gCalculator->allocInit_largeNumberArray(cwo, quota - 1);

	auto cwo2 = new lint::LargeNumber[quota - 1];
	gCalculator->allocInit_largeNumberArray(cwo2, quota - 1);

	auto cwoi = new lint::LargeNumber[quota - 1];
	gCalculator->allocInit_largeNumberArray(cwoi, quota - 1);

	auto banzhafsInternal = new lint::LargeNumber[g->getMaxPrecoalitionSize()];
	gCalculator->alloc_largeNumberArray(banzhafsInternal, g->getMaxPrecoalitionSize());

	bigFloat ExternalMultiplier;
	{
		mpz_ui_pow_ui(mBigTmp.get_mpz_t(), 2, g->getNumberOfPrecoalitions() - 1);
		ExternalMultiplier = 1 / bigFloat(mBigTmp);
	}

	std::vector<longUInt> winternal(g->getMaxPrecoalitionSize());
	//auto winternal = new longUInt[g->getMaxPrecoalitionSize()];

	// Work out number of swings on level of precoalitions
	for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); i++) {
		coalitionsWithoutPlayerFromBelow(g, cwo, c, g->getPrecoalitionWeights()[i]);

		// replicate vector cwo onto cwo2
		for (longUInt ii = 0; ii < quota - 1; ii++) {
			gCalculator->assign(cwo2[ii], cwo[ii]);
		}

		longUInt nbPlayersInParti = g->getPrecoalitions()[i].size();
		if (nbPlayersInParti > 1) {
			for (longUInt x = 0; x < nbPlayersInParti; ++x) {
				winternal[x] = g->getWeights()[g->getPrecoalitions()[i][x]];
				gCalculator->assign_zero(banzhafsInternal[x]);
			}

			generalizedForwardCountingPerWeight(g, cwo2, winternal, nbPlayersInParti, 1);

			for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
				coalitionsWithoutPlayerFromBelow(g, cwoi, cwo2, winternal[ii]);

				longUInt max = std::max((longUInt)1, quota - winternal[ii]);
				for (longUInt iii = max; iii <= quota - 1; iii++) {
					gCalculator->plusEqual(banzhafsInternal[ii], cwoi[iii - 1]);
				}
				// for dictator player
				if (winternal[ii] >= quota) {
					gCalculator->increment(banzhafsInternal[ii]);
				}

				bigFloat InternalMultiplier;
				{
					mpz_ui_pow_ui(mBigTmp.get_mpz_t(), 2, nbPlayersInParti - 1);
					InternalMultiplier = 1 / bigFloat(mBigTmp);
				}

				gCalculator->to_bigInt(&mBigTmp, banzhafsInternal[ii]);
				solution[g->getPrecoalitions()[i][ii]] = ExternalMultiplier * InternalMultiplier * mBigTmp;
			}
		} else {
			// get sum of vector
			gCalculator->assign_zero(mTmp);
			longUInt max = std::max((longUInt)0, quota - g->getPrecoalitionWeights()[i]);
			for (longUInt ii = max; ii <= quota - 1; ++ii) {
				gCalculator->plusEqual(mTmp, cwo[ii - 1]);
			}
			// for dictator player
			if (g->getPrecoalitionWeights()[i] >= quota) {
				gCalculator->increment(mTmp);
			}
			bigInt banzhafs_external;
			gCalculator->to_bigInt(&banzhafs_external, mTmp);
			solution[g->getPrecoalitions()[i][0]] = ExternalMultiplier * banzhafs_external;
		}
	}

	gCalculator->free_largeNumberArray(banzhafsInternal);
	delete[] banzhafsInternal;

	gCalculator->free_largeNumberArray(cwo2);
	gCalculator->free_largeNumberArray(cwoi);
	gCalculator->free_largeNumberArray(c);
	gCalculator->free_largeNumberArray(cwo);

	gCalculator->free_largeNumber(mTmp);

	return solution;
}

std::string epic::index::BanzhafOwenBelow::getFullName() {
	return "BanzhafOwenBelow";
}

epic::longUInt epic::index::BanzhafOwenBelow::getMemoryRequirement(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);
	bigInt memory = (g->getWeightSum() + 1 - g->getQuota()) * gCalculator->getLargeNumberSize() * 4;
	memory += g->getMaxPrecoalitionSize() * gCalculator->getLargeNumberSize();
	memory += g->getMaxPrecoalitionSize() * c_sizeof_longUInt;
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}
	return ret;
}

epic::bigInt epic::index::BanzhafOwenBelow::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_totalNumberOfSwingPlayer();
}

epic::lint::Operation epic::index::BanzhafOwenBelow::getOperationRequirement() {
	return lint::Operation::addition;
}
