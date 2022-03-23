#include "index/SymmetricCoalitionalBanzhafBelow.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

#include <cmath>

epic::index::SymmetricCoalitionalBanzhafBelow::SymmetricCoalitionalBanzhafBelow()
	: PowerIndexWithPrecoalitions() {}

std::vector<epic::bigFloat> epic::index::SymmetricCoalitionalBanzhafBelow::calculate(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);
	gCalculator->alloc_largeNumber(mTmp);

	bigInt* factorial;
	{
		longUInt size = std::max(g->getMaxPrecoalitionSize(), g->getNumberOfPrecoalitions());
		factorial = new bigInt[size + 1];

		factorial[0] = 1;
		factorial[1] = 1;
		for (longUInt i = 2; i <= size; ++i) {
			factorial[i] = factorial[i - 1] * i;
		}
	}

	longUInt quota = g->getQuota();
	int nbPlayers = g->getNumberOfPlayers();
	// longUInt totalWeight = g->getWeightSum();
	std::vector<bigFloat> solution(nbPlayers);
	auto banzhafsExternalGame = new lint::LargeNumber[g->getNumberOfPrecoalitions()];
	gCalculator->allocInit_largeNumberArray(banzhafsExternalGame, g->getNumberOfPrecoalitions());

	auto cc = new lint::LargeNumber[quota];
	gCalculator->allocInit_largeNumberArray(cc, quota);

	auto cwo = new lint::LargeNumber[quota];
	gCalculator->allocInit_largeNumberArray(cwo, quota);

	Array2d<lint::LargeNumber> cwoi;
	cwoi.alloc(quota, g->getMaxPrecoalitionSize() + 1);
	gCalculator->allocInit_largeNumberArray(cwoi.getArrayPointer(), cwoi.getNumberOfElements());

	//auto winternal = new longUInt[g->getMaxPrecoalitionSize()];
	std::vector<longUInt> winternal(g->getMaxPrecoalitionSize());

	generalizedForwardCountingPerWeight(g, cc, g->getPrecoalitionWeights(), g->getNumberOfPrecoalitions(), 1);

	bigFloat ExternalMultiplier = 1 / (pow(2, (g->getNumberOfPrecoalitions())-1));
	for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); i++) {
		int nbPlayersInParti = g->getPrecoalitions()[i].size();
		// initialize winternal
		for (int z = 0; z < nbPlayersInParti; ++z) {
			winternal[z] = g->getWeights()[g->getPrecoalitions()[i][z]];
		}

		coalitionsWithoutPlayerFromBelow(g, cwo, cc, g->getPrecoalitionWeights()[i]);
		longUInt max = std::max((longUInt)1, quota - g->getPrecoalitionWeights()[i]);
		for (longUInt ii = max; ii < quota; ii++) {
			gCalculator->plusEqual(banzhafsExternalGame[i], cwo[ii - 1]);
		}
		// for dictator players
		if (g->getPrecoalitionWeights()[i] >= quota) {
			gCalculator->increment(banzhafsExternalGame[i]);
		}

		Array2d<lint::LargeNumber> cwo2;
		cwo2.alloc(quota, g->getMaxPrecoalitionSize() + 1);
		gCalculator->allocInit_largeNumberArray(cwo2.getArrayPointer(), cwo2.getNumberOfElements());
		for (longUInt ii = 0; ii < quota; ii++) {
			gCalculator->assign(cwo2(ii, 0), cwo[ii]);
		}

		if (nbPlayersInParti > 1) {
			auto shapleysInternal = new bigInt[g->getPrecoalitions()[i].size()];
			generalizedForwardCountingPerWeightCardinality(g, cwo2, winternal, nbPlayersInParti, (longUInt)1, 1, true);

			for (int ii = 0; ii < nbPlayersInParti; ii++) {
				coalitionsCardinalityWithoutPlayerFromBelow(g, cwo2, cwoi, nbPlayersInParti, ii, winternal, 1, -1);

				updateInternalShapleyShubik(g, shapleysInternal, cwoi, i, ii, winternal, factorial);
				solution[g->getPrecoalitions()[i][ii]] = ExternalMultiplier * shapleysInternal[ii] / factorial[nbPlayersInParti];
			}
			delete[] shapleysInternal;
		} else {
			bigInt banzhafs_External;
			gCalculator->to_bigInt(&banzhafs_External, banzhafsExternalGame[i]);
			solution[g->getPrecoalitions()[i][0]] = ExternalMultiplier * banzhafs_External;
		}
		gCalculator->free_largeNumberArray(cwo2.getArrayPointer());
	}
	gCalculator->free_largeNumberArray(cwoi.getArrayPointer());
	gCalculator->free_largeNumberArray(cc);
	gCalculator->free_largeNumberArray(cwo);
	gCalculator->free_largeNumberArray(banzhafsExternalGame);
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

epic::longUInt epic::index::SymmetricCoalitionalBanzhafBelow::getMemoryRequirement(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);
	bigInt memory = (g->getWeightSum() + 1 - g->getQuota()) * gCalculator->getLargeNumberSize() * 2; // cc, cw
	memory += g->getNumberOfPrecoalitions() * c_sizeof_longUInt;																   // g->getPrecoalitionWeights()
	longUInt max = std::max(g->getMaxPrecoalitionSize(), g->getNumberOfPrecoalitions());
	memory += max * GMPHelper::size_of_float(bigInt::factorial(max));												// factorial
	memory += g->getNumberOfPlayers() * GMPHelper::size_of_int(bigInt::factorial(g->getMaxPrecoalitionSize()));					// shapleyInternal (only very rough approximation
	memory += g->getNumberOfPlayers() * GMPHelper::size_of_int(bigInt::factorial(g->getNumberOfPrecoalitions()));						// banzhafExternal
	memory += (g->getWeightSum() + 1 - g->getQuota()) * g->getMaxPrecoalitionSize() * gCalculator->getLargeNumberSize() * 2; // cw2, cwi
	memory += g->getMaxPrecoalitionSize() * c_sizeof_longUInt;																		// winternal

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}
	return ret;
}

void epic::index::SymmetricCoalitionalBanzhafBelow::updateInternalShapleyShubik(PrecoalitionGame* g, bigInt* internal_ssi, Array2d<lint::LargeNumber>& cwoi, longUInt precoalition, longUInt player, std::vector<longUInt>& weights, bigInt* factorial) {
	longUInt n = g->getPrecoalitions()[precoalition].size();
	for (longUInt sinternal = 0; sinternal < n; ++sinternal) {
		gCalculator->assign_zero(mTmp);
		longUInt max = std::max((longUInt)1, g->getQuota() - weights[player]);
		for (longUInt w = max; w < g->getQuota(); ++w) {
			gCalculator->plusEqual(mTmp, cwoi(w - 1, sinternal));
		}
		// added for dictator players
		if (sinternal == 0) {
			if (weights[player] >= g->getQuota()) {
				gCalculator->increment(mTmp);
			}
		}
		gCalculator->to_bigInt(&mBigTmp, mTmp);
		internal_ssi[player] += (factorial[sinternal] * factorial[n - sinternal - 1]) * mBigTmp;
	}
}

epic::bigInt epic::index::SymmetricCoalitionalBanzhafBelow::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_totalNumberOfSwingPlayer();
}

epic::lint::Operation epic::index::SymmetricCoalitionalBanzhafBelow::getOperationRequirement() {
	return lint::Operation::addition;
}