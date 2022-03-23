#include "index/OwenBelow.h"

#include "../include/FastUpperBoundApproximation.h"
#include "../include/index/ShapleyShubik.h"
#include "Array.h"
#include "Game.h"

#include <Logging.h>
#include <numeric>
#include "lint/GlobalCalculator.h"

epic::index::OwenBelow::OwenBelow()
	: PowerIndexWithPrecoalitions() {}

std::vector<epic::bigFloat> epic::index::OwenBelow::calculate(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);
	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	gCalculator->alloc_largeNumber(mTmp);
	longUInt quota = g->getQuota();

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

	Array2d<lint::LargeNumber> cc;
	cc.alloc(quota, g->getNumberOfPrecoalitions());

	gCalculator->allocInit_largeNumberArray(cc.getArrayPointer(), cc.getNumberOfElements());
	generalizedForwardCountingPerWeightCardinality(g, cc, g->getPrecoalitionWeights(), g->getNumberOfPrecoalitions(), (longUInt)1, (longUInt)0, false);

	Array2d<lint::LargeNumber> cwo;
	cwo.alloc(quota, g->getNumberOfPrecoalitions());
	gCalculator->allocInit_largeNumberArray(cwo.getArrayPointer(), cwo.getNumberOfElements());
	auto shapleysInternal = new bigInt[g->getNumberOfPlayers()]();

	Array2d<lint::LargeNumber> cwo2;
	cwo2.alloc(quota, g->getMaxPrecoalitionSize() + 1);
	gCalculator->allocInit_largeNumberArray(cwo2.getArrayPointer(), cwo2.getNumberOfElements());

	Array2d<lint::LargeNumber> cwoi;
	cwoi.alloc(quota, g->getMaxPrecoalitionSize() + 1);
	gCalculator->allocInit_largeNumberArray(cwoi.getArrayPointer(), cwoi.getNumberOfElements());

	//auto winternal = new longUInt[g->getMaxPrecoalitionSize()];
	std::vector<longUInt> winternal(g->getMaxPrecoalitionSize());
	bigInt factor;

	for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); ++i) {
		longUInt nbPlayersInPartI = g->getPrecoalitions()[i].size();
		// initialize winternal
		for (longUInt z = 0; z < nbPlayersInPartI; ++z) {
			winternal[z] = g->getWeights()[g->getPrecoalitions()[i][z]];
		}

		coalitionsCardinalityWithoutPlayerFromBelow(g, cc, cwo, g->getNumberOfPrecoalitions() - 1, i, g->getPrecoalitionWeights(), 0, -1);
		for (int s = 0; s < ((int)g->getNumberOfPrecoalitions()); ++s) {
			s = s - 1;
			factor = factorial[s + 1] * factorial[g->getNumberOfPrecoalitions() - s - 2];
			if (nbPlayersInPartI > 1) {
				for (longUInt x = 0; x < quota; ++x) {
					for (longUInt y = 0; y < (g->getMaxPrecoalitionSize() + 1); ++y) {
						gCalculator->assign_zero(cwo2(x, y));
					}
					if (s >= 0) {
						gCalculator->assign(cwo2(x, 0), cwo(x, s));
					}
				}
				if (s == -1) {
					generalizedForwardCountingPerWeightCardinality(g, cwo2, winternal, nbPlayersInPartI, (longUInt)1, (longUInt)1, true);
				} else {
					generalizedForwardCountingPerWeightCardinality(g, cwo2, winternal, nbPlayersInPartI, (longUInt)1, (longUInt)1, false);
				}
				for (longUInt x = 0; x < quota; ++x) {
					for (longUInt y = 0; y < (g->getMaxPrecoalitionSize() + 1); ++y) {
						gCalculator->assign(cwoi(x, y), cwo2(x, y));
					}
				}
				for (longUInt ii = 0; ii < nbPlayersInPartI; ++ii) {
					coalitionsCardinalityWithoutPlayerFromBelow(g, cwo2, cwoi, nbPlayersInPartI, ii, winternal, (longUInt)1, s);
					updateInternalShapleyShubik(g, shapleysInternal, cwoi, i, ii, winternal, factorial, factor, s);
				}
			} else {
				gCalculator->assign_zero(mTmp);
				longUInt max = 0;
				if (quota > g->getPrecoalitionWeights()[i]) {
					max = quota - g->getPrecoalitionWeights()[i] - 1;
				}
				if (s >= 0) {
					for (longUInt z = max; z < quota - 1; ++z) {
						gCalculator->plusEqual(mTmp, cwo(z, s));
					}
				}

				if (s == -1) {
					if (g->getPrecoalitionWeights()[i] >= quota) {
						gCalculator->increment(mTmp);
					}
				}
				gCalculator->to_bigInt(&mBigTmp, mTmp);
				shapleysInternal[g->getPrecoalitions()[i][0]] += mBigTmp * factorial[s + 1] * factorial[g->getNumberOfPrecoalitions() - s - 2];
			}
			s++;
		}
	}
	gCalculator->free_largeNumberArray(cc.getArrayPointer());
	gCalculator->free_largeNumberArray(cwo.getArrayPointer());
	gCalculator->free_largeNumberArray(cwo2.getArrayPointer());
	gCalculator->free_largeNumberArray(cwoi.getArrayPointer());

	solution.resize(g->getNumberOfPlayers());
	{
		for (longUInt part = 0; part < g->getNumberOfPrecoalitions(); ++part) {
			longUInt nbPlayersInParti = g->getPrecoalitions()[part].size();
			factor = factorial[nbPlayersInParti] * factorial[g->getNumberOfPrecoalitions()];

			for (longUInt i = 0; i < nbPlayersInParti; ++i) {
				longUInt player = g->getPrecoalitions()[part][i];
				solution[player] = shapleysInternal[player];
				solution[player] /= factor;
			}
		}
	}
	delete[] factorial;
	delete[] shapleysInternal;
	gCalculator->free_largeNumber(mTmp);

	return solution;
}

std::string epic::index::OwenBelow::getFullName() {
	return "OwenBelow";
}

epic::longUInt epic::index::OwenBelow::getMemoryRequirement(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);
	
	bigInt memory = g->getNumberOfPrecoalitions() * c_sizeof_longUInt; // g->getPrecoalitionWeights()
	longUInt max = std::max(g->getMaxPrecoalitionSize(), g->getNumberOfPrecoalitions());
	memory += max * GMPHelper::size_of_float(bigInt::factorial(max));												// factorial
	memory += (g->getWeightSum() + 1 - g->getQuota()) * g->getNumberOfPrecoalitions() * gCalculator->getLargeNumberSize() * 2;		// cc, cw
	memory += g->getNumberOfPlayers() * GMPHelper::size_of_int(bigInt::factorial(g->getMaxPrecoalitionSize()));					// shapleyInternal (only very rough approximation
	memory += (g->getWeightSum() + 1 - g->getQuota()) * g->getMaxPrecoalitionSize() * gCalculator->getLargeNumberSize() * 2; // cw2, cwi
	memory += g->getMaxPrecoalitionSize() * c_sizeof_longUInt;																		// winternal

	memory /= cMemUnit_factor;
	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

void epic::index::OwenBelow::updateInternalShapleyShubik(PrecoalitionGame* g, bigInt* internal_ssi, Array2d<lint::LargeNumber>& cwoi, longUInt precoalition, longUInt player, std::vector<longUInt>& weights, bigInt* factorial, bigInt& scale_factor, int s) {
	longUInt n = g->getPrecoalitions()[precoalition].size();

	for (longUInt sinternal = 0; sinternal < n; ++sinternal) {
		gCalculator->assign_zero(mTmp);
		// log::out << "max: " << g->getQuota()-weights[player] << log::endl;
		longUInt q = g->getQuota();
		longUInt max = std::max((longUInt)1, q - (longUInt)weights[player]);
		// TESTING replaced w=g->getQuota()-weights[player] with max
		for (longUInt w = max; w < g->getQuota(); ++w) {
			// log::out << "cwoi: " << cwoi(w-1, sinternal).uint << log::endl;
			gCalculator->plusEqual(mTmp, cwoi(w - 1, sinternal));
		}

		// for dictator
		if (s == -1) {
			if (sinternal == 0) {
				if (weights[player] >= g->getQuota()) {
					gCalculator->increment(mTmp);
				}
			}
		}
		gCalculator->to_bigInt(&mBigTmp, mTmp);
		internal_ssi[g->getPrecoalitions()[precoalition][player]] += (factorial[sinternal] * factorial[n - sinternal - 1]) * mBigTmp * scale_factor;
	}
}

epic::bigInt epic::index::OwenBelow::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_swingPlayerPerCardinality();
}

epic::lint::Operation epic::index::OwenBelow::getOperationRequirement() {
	return lint::Operation::addition;
}