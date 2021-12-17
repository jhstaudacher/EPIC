#include "index/SymmetricCoalitionalBanzhaf.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

#include <cmath>

epic::index::SymmetricCoalitionalBanzhaf::SymmetricCoalitionalBanzhaf() : PowerIndexWithPrecoalitions() {}

std::vector<epic::bigFloat> epic::index::SymmetricCoalitionalBanzhaf::calculate(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);
	gCalculator->alloc_largeNumber(mTmp);

	/*
	 * Use g and gCalculator to calculate the power index and return the results.
	 */
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

	int nbPlayers = g->getNumberOfPlayers();
	longUInt totalWeight = g->getWeightSum();
	std::vector<bigFloat> solution(nbPlayers);
	auto banzhafsExternalGame = new lint::LargeNumber[g->getNumberOfPrecoalitions()];
	gCalculator->allocInit_largeNumberArray(banzhafsExternalGame, g->getNumberOfPrecoalitions());

	ArrayOffset<lint::LargeNumber> cc(totalWeight + 1, g->getQuota());
	gCalculator->allocInit_largeNumberArray(cc.getArrayPointer(), cc.getNumberOfElements());
	gCalculator->assign_one(cc[totalWeight]);

	ArrayOffset<lint::LargeNumber> cw(totalWeight + 1, g->getQuota());
	gCalculator->allocInit_largeNumberArray(cw.getArrayPointer(), cw.getNumberOfElements());

	Array2dOffset<lint::LargeNumber> cwi;
	cwi.alloc(totalWeight + 1, g->getMaxPrecoalitionSize(), g->getQuota(), 0);
	gCalculator->allocInit_largeNumberArray(cwi.getArrayPointer(), cwi.getNumberOfElements());

	std::vector<longUInt> winternal(g->getMaxPrecoalitionSize());

	generalizedBackwardCountingPerWeight(g, cc, g->getPrecoalitionWeights(), g->getNumberOfPrecoalitions());

	bigFloat ExternalMultiplier = 1 / (pow(2, (g->getNumberOfPrecoalitions())-1));
	for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); i++) {
		int nbPlayersInParti = g->getPrecoalitions()[i].size();
		//initialize winternal
		for (int z = 0; z < nbPlayersInParti; ++z) {
			winternal[z] = g->getWeights()[g->getPrecoalitions()[i][z]];
		}
		coalitionsContainingPlayerFromAbove(g, cw, cc, g->getPrecoalitionWeights()[i]);

		//get sum of vector
		longUInt min = std::min(g->getQuota() + g->getPrecoalitionWeights()[i] - 1, g->getWeightSum());

		for (longUInt iii = g->getQuota(); iii <= min; iii++) {
			gCalculator->plusEqual(banzhafsExternalGame[i], cw[iii]);
		}

		Array2dOffset<lint::LargeNumber> cw2;
		cw2.alloc(totalWeight + 1, g->getMaxPrecoalitionSize() + 1, g->getQuota(), 0);
		gCalculator->allocInit_largeNumberArray(cw2.getArrayPointer(), cw2.getNumberOfElements());
		//replicate vector cw onto cw2[]
		for (longUInt i = g->getQuota(); i < (totalWeight + 1); i++) {
			gCalculator->assign(cw2(i, nbPlayersInParti - 1), cw[i]);
		}

		if (nbPlayersInParti > 1) {
			auto shapleysInternal = new bigInt[g->getPrecoalitions()[i].size()];
			gCalculator->assign_one(cw2(totalWeight, nbPlayersInParti));

			generalizedBackwardCountingPerWeightCardinality(g, cw2, winternal, nbPlayersInParti);

			for (int ii = 0; ii < nbPlayersInParti; ii++) {
				if ((totalWeight - winternal[ii]) >= g->getQuota()) {
					coalitionsCardinalityContainingPlayerFromAbove(g, cwi, cw2, nbPlayersInParti, ii, winternal);
				} else {
					for (longUInt x = g->getQuota(); x <= g->getWeightSum(); ++x) {
						for (int y = 0; y < nbPlayersInParti; ++y) {
							gCalculator->assign(cwi(x, y), cw2(x, y));
						}
					}
				}
				updateInternalShapleyShubik(g, shapleysInternal, cwi, i, ii, winternal, factorial);
				solution[g->getPrecoalitions()[i][ii]] = ExternalMultiplier * shapleysInternal[ii] / factorial[nbPlayersInParti];
			}
			delete[] shapleysInternal;
		} else {
			bigInt banzhafs_External;
			gCalculator->to_bigInt(&banzhafs_External, banzhafsExternalGame[i]);
			solution[g->getPrecoalitions()[i][0]] = ExternalMultiplier * banzhafs_External;
		}
		gCalculator->free_largeNumberArray(cw2.getArrayPointer());
	}
	gCalculator->free_largeNumberArray(cwi.getArrayPointer());
	gCalculator->free_largeNumberArray(cc.getArrayPointer());
	gCalculator->free_largeNumberArray(cw.getArrayPointer());
	gCalculator->free_largeNumberArray(banzhafsExternalGame);
	delete[] banzhafsExternalGame;
	delete[] factorial;

	gCalculator->free_largeNumber(mTmp);

	return solution;
}

std::string epic::index::SymmetricCoalitionalBanzhaf::getFullName() {
	/*
	 * Return the full name of the implemented power index.
	 */
	return "SymmetricCoalitionalBanzhaf";
}

epic::longUInt epic::index::SymmetricCoalitionalBanzhaf::getMemoryRequirement(Game* g_) {
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

epic::bigInt epic::index::SymmetricCoalitionalBanzhaf::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_totalNumberOfSwingPlayer();
}

epic::lint::Operation epic::index::SymmetricCoalitionalBanzhaf::getOperationRequirement() {
	return lint::Operation::addition;
}

void epic::index::SymmetricCoalitionalBanzhaf::updateInternalShapleyShubik(PrecoalitionGame* g, bigInt* internal_ssi, Array2dOffset<lint::LargeNumber>& cwi, longUInt precoalition, longUInt player, std::vector<longUInt>& weights, bigInt* factorial) {
	longUInt n = g->getPrecoalitions()[precoalition].size();

	for (longUInt sinternal = 0; sinternal < n; ++sinternal) {
		gCalculator->assign_zero(mTmp);
		longUInt min = std::min(g->getQuota() + weights[player] - 1, g->getWeightSum());
		for (longUInt w = g->getQuota(); w <= min; ++w) {
			gCalculator->plusEqual(mTmp, cwi(w, sinternal));
		}

		gCalculator->to_bigInt(&mBigTmp, mTmp);
		internal_ssi[player] += (factorial[sinternal] * factorial[n - sinternal - 1]) * mBigTmp;
	}
}