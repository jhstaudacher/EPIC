#include "index/Owen.h"

#include "Array.h"
#include "lint/GlobalCalculator.h"


epic::index::Owen::Owen() : PowerIndexWithPrecoalitions() {
	gCalculator->alloc_largeNumber(mTmp);
}

epic::index::Owen::~Owen() {
	gCalculator->free_largeNumber(mTmp);
}

std::vector<epic::bigFloat> epic::index::Owen::calculate(Game& g) {
	std::vector<bigFloat> solution;

	if (g.getNumberOfVetoPlayers() > 0) {
		solution.resize(g.getNumberOfPlayers());
		for (longUInt i = 0; i < g.getNumberOfPlayers(); ++i) {
			solution[i] = g.getVetoPlayerVector()[i] ? 1 : 0;
		}
	} else {
		bigInt* factorial;
		{
			longUInt size = std::max(g.getMaxPrecoalitionSize(), g.getNumberOfPrecoalitions());
			factorial = new bigInt[size + 1];

			factorial[0] = 1;
			factorial[1] = 1;

			for (longUInt i = 2; i <= size; ++i) {
				factorial[i] = factorial[i - 1] * i;
			}
		}

		Array2dOffset<lint::LargeNumber> cc(g.getWeightSum() + 1, g.getNumberOfPrecoalitions(), g.getQuota(), 0);
		gCalculator->allocInit_largeNumberArray(cc.getArrayPointer(), cc.getNumberOfElements());
		gCalculator->assign_one(cc(g.getWeightSum(), g.getNumberOfPrecoalitions() - 1));
		generalizedBackwardCountingPerWeightCardinality(g, cc, g.getPrecoalitionWeights(), g.getNumberOfPrecoalitions());

		Array2dOffset<lint::LargeNumber> cw(g.getWeightSum() + 1, g.getNumberOfPrecoalitions(), g.getQuota(), 0);
		gCalculator->allocInit_largeNumberArray(cw.getArrayPointer(), cw.getNumberOfElements());

		auto shapleysInternal = new bigInt[g.getNumberOfPlayers()]();

		Array2dOffset<lint::LargeNumber> cw2(g.getWeightSum() + 1,g.getMaxPrecoalitionSize(), g.getQuota(), 0);
		gCalculator->allocInit_largeNumberArray(cw2.getArrayPointer(), cw2.getNumberOfElements());

		Array2dOffset<lint::LargeNumber> cwi(g.getWeightSum() + 1,g.getMaxPrecoalitionSize(), g.getQuota(), 0);
		gCalculator->alloc_largeNumberArray(cwi.getArrayPointer(), cwi.getNumberOfElements());

		std::vector<longUInt> winternal(g.getMaxPrecoalitionSize());

		bigInt factor;

		for (longUInt i = 0; i < g.getNumberOfPrecoalitions(); ++i) {
			longUInt nbPlayersInPartI = g.getPrecoalitions()[i].size();

			coalitionsCardinalityContainingPlayerFromAbove(g, cw, cc, g.getNumberOfPrecoalitions(), i, g.getPrecoalitionWeights());

			// initialize winternal
			for (longUInt z = 0; z < nbPlayersInPartI; ++z) {
				winternal[z] = g.getWeights()[g.getPrecoalitions()[i][z]];
			}

			for (longUInt s = 0; s < g.getNumberOfPrecoalitions(); ++s) {
				factor = factorial[s] * factorial[g.getNumberOfPrecoalitions() - s - 1];

				if (nbPlayersInPartI > 1) {
					for (longUInt x = g.getQuota(); x <= g.getWeightSum(); ++x) {
						for (longUInt y = 0; y < nbPlayersInPartI - 1; ++y) {
							gCalculator->assign_zero(cw2(x, y));
						}
						gCalculator->assign(cw2(x, nbPlayersInPartI - 1), cw(x, s));
					}

					generalizedBackwardCountingPerWeightCardinality(g, cw2, winternal, nbPlayersInPartI);

					for (longUInt x = g.getQuota(); x <= g.getWeightSum(); ++x) {
						for (longUInt y = 0; y < nbPlayersInPartI; ++y) {
							gCalculator->assign(cwi(x, y), cw2(x, y));
						}
					}

					for (longUInt ii = 0; ii < nbPlayersInPartI; ++ii) {
						coalitionsCardinalityContainingPlayerFromAbove(g, cwi, cw2, nbPlayersInPartI, ii, winternal);
						updateInternalShapleyShubik(g, shapleysInternal, cwi, i, ii, winternal, factorial, factor);
					}
				} else {
					// shapleysInternal[g.getPrecoalitions()[i][0] - 1] += factor * sum(cw[q:tmp_min, s])
					gCalculator->assign_zero(mTmp);
					longUInt tmp_min = std::min(g.getQuota() + g.getPrecoalitionWeights()[i] - 1, g.getWeightSum());
					for (longUInt z = g.getQuota(); z <= tmp_min; ++z) {
						gCalculator->plusEqual(mTmp, cw(z, s));
					}
					gCalculator->to_bigInt(&mBigTmp, mTmp);
					shapleysInternal[g.getPrecoalitions()[i][0]] += mBigTmp * factor;
				}
			}
		}

		gCalculator->free_largeNumberArray(cc.getArrayPointer());
		gCalculator->free_largeNumberArray(cw.getArrayPointer());
		gCalculator->free_largeNumberArray(cw2.getArrayPointer());
		gCalculator->free_largeNumberArray(cwi.getArrayPointer());

		solution.resize(g.getNumberOfPlayers());
		{
			for (longUInt part = 0; part < g.getNumberOfPrecoalitions(); ++part) {
				longUInt nbPlayersInParti = g.getPrecoalitions()[part].size();
				factor = factorial[nbPlayersInParti] * factorial[g.getNumberOfPrecoalitions()];

				for (longUInt i = 0; i < nbPlayersInParti; ++i) {
					// solution[player] = shapleysInternal[player] / (factorial[nbPlayersInParti] * factorial[nbPartitions]);
					longUInt player = g.getPrecoalitions()[part][i];
					solution[player] = shapleysInternal[player];
					solution[player] /= factor;
				}
			}
		}

		delete[] factorial;
		delete[] shapleysInternal;
	}

	return solution;
}

std::string epic::index::Owen::getFullName() {
	return "Owen";
}

epic::longUInt epic::index::Owen::getMemoryRequirement(Game& g) {
	bigInt memory = g.getNumberOfPrecoalitions() * c_sizeof_longUInt; // g.getPrecoalitionWeights()
	longUInt max = std::max(g.getMaxPrecoalitionSize(), g.getNumberOfPrecoalitions());
	memory += max * GMPHelper::size_of_float(bigInt::factorial(max));												// factorial
	memory += (g.getWeightSum() + 1 - g.getQuota()) * g.getNumberOfPrecoalitions() * gCalculator->getLargeNumberSize() * 2;		// cc, cw
	memory += g.getNumberOfPlayers() * GMPHelper::size_of_int(bigInt::factorial(g.getMaxPrecoalitionSize()));					// shapleyInternal (only very rough approximation
	memory += (g.getWeightSum() + 1 - g.getQuota()) *g.getMaxPrecoalitionSize() * gCalculator->getLargeNumberSize() * 2; // cw2, cwi
	memory +=g.getMaxPrecoalitionSize() * c_sizeof_longUInt;																		// winternal

	memory /= cMemUnit_factor;
	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

epic::bigInt epic::index::Owen::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_swingPlayerPerCardinality();
}

epic::lint::Operation epic::index::Owen::getOperationRequirement() {
	return lint::Operation::addition;
}

void epic::index::Owen::updateInternalShapleyShubik(Game& g, bigInt* internal_ssi, Array2dOffset<lint::LargeNumber>& cwi, longUInt precoalition, longUInt player, std::vector<longUInt>& weights, bigInt* factorial, bigInt& scale_factor) {
	longUInt n = g.getPrecoalitions()[precoalition].size();

	for (longUInt sinternal = 0; sinternal < n; ++sinternal) {
		gCalculator->assign_zero(mTmp);
		longUInt min = std::min(g.getQuota() + weights[player] - 1, g.getWeightSum());
		for (longUInt w = g.getQuota(); w <= min; ++w) {
			gCalculator->plusEqual(mTmp, cwi(w, sinternal));
		}

		gCalculator->to_bigInt(&mBigTmp, mTmp);
		internal_ssi[g.getPrecoalitions()[precoalition][player]] += (factorial[sinternal] * factorial[n - sinternal - 1]) * mBigTmp * scale_factor;
	}
}
